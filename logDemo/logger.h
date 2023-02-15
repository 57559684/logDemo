
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <Windows.h>
#include <string>
#include <queue>
#include <vector>
#include <sstream>
#include "synclock.h"


namespace LOGGER
{


/**
 * Supported log levels, in order of verbosity.
 */
enum LEVELS
{
    None,
    Fatal,
    Error,
    Warning,
    Info,
    Debug,
    Trace
};

class CLogger;
class LogStream
{
public:
    /**
     * Constructor
     */
    LogStream(CLogger *pLogger, LEVELS nLevel, int linenum, const char*filename, const char*functionname, bool bFlush);
	LogStream(LogStream&& other);
    
    /** 
     * Destructor. This class relies on always being called as r-value.
     * I.e. 
     *   LogBuffer (kInfo) << "This is a log line";
     */
    virtual ~LogStream();
    
    /**
     * Enable stringstream like operator <<
     */
	template<class T>
	LogStream &operator<<(const T &x) {
		mStream << x;
		return *this;
	}

	LogStream &operator<<(const std::wstring &s);
	LogStream &operator<<(const wchar_t *s);
    
private:
    std::stringstream mStream;
    LEVELS mLevel;
	CLogger *mLogger;
	int mlinenum;
	const char*mfilename;
	const char*mfunctionname;
	bool mbFlush;
};



class CLogger
{
public:
	//nLogLevel����־��¼�ĵȼ����ɿ�
	//strLogPath����־Ŀ¼���ɿ�
	//strLogName����־���ƣ��ɿ�
	CLogger(LEVELS nLevel = Trace, const std::string strLogPath = "", const std::string strLogName = "");
	//��������
	virtual ~CLogger();

    /**
     * Log a message to the logger
     */
    void AddStream(LEVELS nLevel, int linenum, const char*filename, const char*functionname, const std::string& inMessage);

	LogStream operator()(int linenum, const char*filename, const char*functionname, LEVELS nLevel = Info, bool bFlush = false);
    
    /**
     * Flush the logger. 
     */
    void Flush();

	void SetFlushTime(UINT millisecond);

private:

	//��ȡ��ǰϵͳʱ��
	std::string GetTime();
	//��ȡ��������·��
	std::string GetAppPathA();
	//��ʽ���ַ���
	std::string FormatString(const char *lpcszFormat, ...);
	//�ļ�ȫ·���õ��ļ���
	const char *path_file(const char *path, char splitter);

	void FlushRun();


private:
	Mutex m_mutex;
    std::string m_Messages;
    LEVELS m_nLevel;
    std::vector <std::string> m_Levelstr;
	UINT m_flushTime;
	UINT m_bflushThreadRun;

	//д��־�ļ���
	FILE * m_pFileStream;
	//��־Ŀ¼
	std::string m_strLogPath;
	//��־������
	std::string m_strLogName;
	//��־�ļ�ȫ·��
	std::string m_strLogFilePath;


};


#define pNONE_LOG(pInstance)       (*pInstance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::None)

#define pFATAL_LOG(pInstance)      (*pInstance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Fatal)

#define pERROR_LOG(pInstance)      (*pInstance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Error)

#define pWARNING_LOG(pInstance)    (*pInstance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Warning)

#define pINFO_LOG(pInstance)       (*pInstance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Info)

#define pDEBUG_LOG(pInstance)      (*pInstance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Debug)

#define pTrace_LOG(pInstance)      (*pInstance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Trace)

#define NONE_LOG(instance)         (instance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::None)

#define FATAL_LOG(instance)        (instance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Fatal)

#define ERROR_LOG(instance)        (instance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Error)

#define WARNING_LOG(instance)      (instance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Warning)

#define INFO_LOG(instance)         (instance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Info)

#define DEBUG_LOG(instance)        (instance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Debug)

#define Trace_LOG(instance)        (instance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Trace)


//����д�ļ�
#define pNONE_LOGF(pInstance)       (*pInstance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::None, true)

#define pFATAL_LOGF(pInstance)      (*pInstance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Fatal, true)

#define pERROR_LOGF(pInstance)      (*pInstance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Error, true)

#define pWARNING_LOGF(pInstance)    (*pInstance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Warning, true)

#define pINFO_LOGF(pInstance)       (*pInstance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Info, true)

#define pDEBUG_LOGF(pInstance)      (*pInstance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Debug, true)

#define pTrace_LOGF(pInstance)      (*pInstance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Trace, true)

#define NONE_LOGF(instance)         (instance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::None, true)

#define FATAL_LOGF(instance)        (instance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Fatal, true)

#define ERROR_LOGF(instance)        (instance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Error, true)

#define WARNING_LOGF(instance)      (instance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Warning, true)

#define INFO_LOGF(instance)         (instance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Info, true)

#define DEBUG_LOGF(instance)        (instance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Debug, true)

#define Trace_LOGF(instance)        (instance)(__LINE__, __FILE__, __FUNCTION__, LOGGER::Trace, true)


} // end namespace LOGGER

#endif // _LOGGER_H_
