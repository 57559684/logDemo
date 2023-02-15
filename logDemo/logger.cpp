
#ifdef _WIN32
#pragma warning(disable:4996)
#endif

#include <iostream>
#include <algorithm>

#include "logger.h"
#include <time.h>
#include "usecs_tick.h"
#include <Dbghelp.h>
#include "asynthread.h"
#include "zipx.h"
#include "str_utils.h"

#pragma comment(lib,"Dbghelp.lib")



namespace LOGGER
{

const int BufferSize = 65536;

LogStream::LogStream(CLogger *pLogger, LEVELS nLevel, int linenum, const char*filename, const char*functionname, bool bFlush)
	:mLevel(nLevel)
	,mLogger(pLogger)
	,mlinenum(linenum)
	,mfilename(filename)
	,mfunctionname(functionname)
	,mbFlush(bFlush)
{
}

LogStream::LogStream(LogStream&& other)
{
	mLogger = other.mLogger;
	mLevel = other.mLevel;
	mlinenum = other.mlinenum;
	mStream << other.mStream.str();
	mfilename = other.mfilename;
	mfunctionname = other.mfunctionname;
	mbFlush = other.mbFlush;
}

LogStream::~LogStream()
{
    mLogger->AddStream(mLevel, mlinenum, mfilename, mfunctionname, mStream.str());
	if(mbFlush) mLogger->Flush();
}

LogStream& LogStream::operator<<(const std::wstring &s)
{
	mStream << U2A(s);
	return *this;
}

LogStream& LogStream::operator<<(const wchar_t *s)
{
	mStream << U2A(s);
	return *this;
}

CLogger::CLogger(LEVELS nLevel, const std::string strLogPath, const std::string strLogName)
	:m_nLevel (nLevel)
	,m_strLogPath(strLogPath)
	,m_strLogName(strLogName)
	,m_flushTime(0)
	,m_bflushThreadRun(false)
{
	m_Levelstr.push_back("[NONE]");
	m_Levelstr.push_back("[FATAL]");
	m_Levelstr.push_back("[ERROR]");
	m_Levelstr.push_back("[WARNING]");
	m_Levelstr.push_back("[INFO]");
	m_Levelstr.push_back("[DEBUG]");
	m_Levelstr.push_back("[TRACE]");

	//初始化
	m_pFileStream = NULL;
	if (m_strLogPath.empty())
	{
		m_strLogPath = GetAppPathA();
	}
	if (m_strLogPath.back() != '\\')
	{
		m_strLogPath.append("\\");
	}
	//创建文件夹
	MakeSureDirectoryPathExists(m_strLogPath.c_str());

	//创建日志文件
	time_t curTime;
	time(&curTime);
	tm tm1;
	localtime_s(&tm1, &curTime);
	//日志的名称如：_2016-01-01.log
	m_strLogName += FormatString("_%04d-%02d-%02d.log", tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday);
	m_strLogFilePath = m_strLogPath + m_strLogName;

	//以追加的方式打开文件流
	m_pFileStream = _fsopen(m_strLogFilePath.c_str(), "a+", _SH_DENYNO);

	SetFlushTime(500);

	AsynThread::run([=](){
		const std::string pathfilename = m_strLogFilePath;
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];

		_splitpath( pathfilename.c_str(), drive, dir, fname, ext );
		std::string basename, extname;
		basename.append(drive);
		basename.append(dir);
		basename.append(fname);
		extname = ext;

		std::wstring filterExt = L"*";
		filterExt += A2U(ext);

		std::wstring scrfile = A2U(basename);
		int nPos = scrfile.find_last_of(L"/\\");
		if (nPos != string::npos)
		{
			scrfile = scrfile.substr(0, nPos);
		}
		else
		{
			return;
		}

		CZipx zip;
		std::string excludetemp = basename.substr(basename.length()-11) + ext;
		std::wstring excludefile = A2U(excludetemp);
		zip.ZipFiles2Part(scrfile.c_str(), filterExt.c_str(), excludefile.c_str());

	});
}

CLogger::~CLogger()
{
	m_bflushThreadRun = true;
    Flush ();

	//关闭文件流
	if (m_pFileStream)
	{
		MutexLock<Mutex> lock(m_mutex);
		fclose(m_pFileStream);
		m_pFileStream = NULL;
	}

}

void CLogger::AddStream(LEVELS nLevel, int linenum, const char*filename, const char*functionname, const std::string& inMessage)
{
    if (m_nLevel == None)
	{
        return;
    }
    
    if (nLevel <= m_nLevel)
	{
        const std::string message(m_Levelstr[nLevel] + GetTime() + FormatString("[%s:%d][%d][%s]", path_file(filename, '\\'), linenum, GetCurrentThreadId(), functionname) + inMessage);

		if(m_Messages.length() + message.length() >= BufferSize)
		{
			Flush();
		}

		{
			MutexLock<Mutex> lock(m_mutex);
			m_Messages.append(message);
			m_Messages.append("\n");
		}
    }
}

LogStream CLogger::operator()(int linenum, const char*filename, const char*functionname, LEVELS nLevel, bool bFlush)
{
	return LogStream(this, nLevel, linenum, filename, functionname, bFlush);
}

void CLogger::Flush()
{
	MutexLock<Mutex> lock(m_mutex);
	//若文件流没有打开，则重新打开
	if (NULL == m_pFileStream)
	{
		m_pFileStream = _fsopen(m_strLogFilePath.c_str(), "a+", _SH_DENYNO);
		if (!m_pFileStream)
		{
			return;
		}
	}

	if(m_Messages.empty()) return;
	//fprintf(m_pFileStream, "%s", m_Messages.c_str());
	fwrite(m_Messages.c_str(), m_Messages.length(), 1, m_pFileStream);
	m_Messages.clear();
	fflush(m_pFileStream);

}

void CLogger::SetFlushTime(UINT millisecond)
{
	if(!millisecond) return;
	m_flushTime = millisecond;
	if(!m_bflushThreadRun)
	{
		m_bflushThreadRun = true;
		AsynThread::run([&]()
		{			
			FlushRun();
		});
	}
}

void CLogger::FlushRun()
{
	while(m_bflushThreadRun)
	{
		time_t curTime;
		time(&curTime);
		tm tm1;
		localtime_s(&tm1, &curTime);
		std::string strLogName("");
		if(m_strLogName.length() > 15)
		{
			strLogName += m_strLogName.substr(0, m_strLogName.length()-15);
		}

		//
		strLogName += FormatString("_%04d-%02d-%02d.log", tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday);
		if(m_strLogName != strLogName)
		{
			MutexLock<Mutex> lock(m_mutex);
			//关闭文件流
			if (m_pFileStream)
			{
				fclose(m_pFileStream);
				m_pFileStream = NULL;
			}

			const std::string oldpathfilename = m_strLogFilePath;
			m_strLogName = strLogName;
			m_strLogFilePath = m_strLogPath + m_strLogName;

			AsynThread::run([=]()
			{
				//压缩前一天的日志
				CZipx zip;
				std::wstring scrfile = A2U(oldpathfilename);

				char drive[_MAX_DRIVE];
				char dir[_MAX_DIR];
				char fname[_MAX_FNAME];
				char ext[_MAX_EXT];

				_splitpath( oldpathfilename.c_str(), drive, dir, fname, ext );
				std::string basename, extname;
				basename.append(drive);
				basename.append(dir);
				basename.append(fname);
				extname = ext;
				basename.append(".zip");
				std::wstring desfile = A2U(basename);
				zip.ZipFile(scrfile.c_str(), desfile.c_str());

				//删除前一天的日志
				DeleteFileW(scrfile.c_str());
			});

			m_pFileStream = _fsopen(m_strLogFilePath.c_str(), "a+", _SH_DENYNO);
		}

		if(!m_bflushThreadRun) break;

		Sleep(m_flushTime);

		if(!m_bflushThreadRun) break;

		Flush();
	}
}

//获取系统当前时间
std::string CLogger::GetTime()
{
	time_t curTime;
	time(&curTime);
	tm tm1;
	localtime_s(&tm1, &curTime);
	//2016-01-01 21:30:00
	std::string strTime = FormatString("[%04d-%02d-%02d %02d:%02d:%02d.%06d]", tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min, tm1.tm_sec, getmicroseconds());

	return strTime;
}

std::string CLogger::GetAppPathA()
{
	char szFilePath[MAX_PATH] = { 0 }, szDrive[MAX_PATH] = { 0 }, szDir[MAX_PATH] = { 0 }, szFileName[MAX_PATH] = { 0 }, szExt[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, sizeof(szFilePath));
	_splitpath_s(szFilePath, szDrive, szDir, szFileName, szExt);

	std::string str(szDrive);
	str.append(szDir);
	return str;
}

std::string CLogger::FormatString(const char *lpcszFormat, ...)
{
	std::string strResult;
	if (NULL != lpcszFormat)
	{
		va_list marker = NULL;
		va_start(marker, lpcszFormat); //初始化变量参数
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度
		std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
		if (nWritten > 0)
		{
			strResult = &vBuffer[0];
		}
		va_end(marker); //重置变量参数
	}
	return strResult;
}

//文件全路径得到文件名
const char *CLogger::path_file(const char *path, char splitter)
{
	return strrchr(path, splitter) ? strrchr(path, splitter) + 1 : path;
}

} // end namespace LOGGER