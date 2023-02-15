#include "zipx.h"
#include <stdio.h>
#include <Shlwapi.h>


#pragma comment(lib,"shlwapi.lib")


void CZipx::PathAddBackslash2(xstring &strFilePath)
{
	int nLen = strFilePath.size();
	if(nLen > 0)
	{
		if(strFilePath[nLen-1] != _T('\\'))
		{
			strFilePath += _T('\\');
		}
	}
}

// �ж��ļ��Ƿ����
bool CZipx::IsFileExist(const xstring& strFilePath)
{
	DWORD dwAttrib = GetFileAttributes(strFilePath.c_str());
	return ((INVALID_FILE_ATTRIBUTES != dwAttrib) && (0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)));
}
// �ж��ļ����Ƿ����
bool CZipx::IsDirExist(const xstring & strFilePath)
{
	DWORD dwAttrib = GetFileAttributes(strFilePath.c_str());
	return ((INVALID_FILE_ATTRIBUTES != dwAttrib) && (0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)));
}
// �ж��ļ����ļ����Ƿ����
bool CZipx::IsPathExist(const xstring & strFilePath)
{
	DWORD dwAttrib = GetFileAttributes(strFilePath.c_str());
	return (INVALID_FILE_ATTRIBUTES != dwAttrib);
}

CZipx::CZipx(void)
{
}

CZipx::~CZipx(void)
{
}

bool CZipx::ZipFile(LPCTSTR lpszSrcFile, LPCTSTR lpszDstFile, const char *pszPassword)
{
	if(!lpszSrcFile) return false;

	//Ŀ���ļ��Ƿ����
	if(!IsFileExist(lpszSrcFile))
	{
		return false;
	}

	// ȡ���ļ���
	LPCTSTR pszFind = _tcsrchr(lpszSrcFile, _T('\\'));
	xstring strFileName = pszFind + 1;
	xstring strdstFileName;
	xstring strFileDir(lpszSrcFile, pszFind-lpszSrcFile);
	if(lpszDstFile == NULL)
	{
		//����Ŀ���ļ���׺
		string::size_type nFind = strFileName.find_last_of(_T('.'));
		if(nFind != string::npos)
		{
			strdstFileName = strFileDir +  + _T("\\") + strFileName.substr(0, nFind) + _T(".zip");
		}
		else
		{
			strdstFileName = strFileDir +  + _T("\\") + strFileName + _T(".zip");
		}
	}
	else
	{
		strdstFileName = lpszDstFile;
	}

	//�����ļ�Ŀ¼
	CreateDir(strFileDir.c_str());

	HZIP hz = CreateZip(strdstFileName.c_str(), pszPassword);
	ZipAdd(hz, strFileName.c_str(), lpszSrcFile);
	CloseZip(hz);

	return true;
}
bool CZipx::ZipFileList(vector<wstring>& srcFileList, const wstring& strDstFile, const char *pszPassword/*=NULL*/)
{
	wstring strDir,strName;
	int nPos = strDstFile.find_last_of(L"/\\");
	if (nPos != string::npos)
	{
		strDir = strDstFile.substr(0,nPos);
		strName = strDstFile.substr(nPos+1);
	}
	else
		return false;

	//�����ļ�Ŀ¼
	CreateDir(strDir.c_str());	

	HZIP hz = CreateZip(strDstFile.c_str(), pszPassword);

	for (vector<wstring>::iterator itr=srcFileList.begin();itr!=srcFileList.end();itr++)
	{
		if (IsFileExist(*itr))
		{
			ZipAdd(hz, this->GetFileNameFromPath(itr->c_str()).c_str(), itr->c_str());
		}
	}

	CloseZip(hz);
	return true;
}

//Ŀ���ļ�����ָ��
BOOL CZipx::ZipFiles(LPCTSTR lpszSrcDir, LPCTSTR lpszDstFile, const char *pszPassword) 
{ 
	if(lpszDstFile == NULL || lpszSrcDir == NULL)
	{
		return FALSE;
	}

	//���ԴĿ¼�Ƿ����
	if(!IsDirExist(lpszSrcDir))
	{
		return false;
	}

	// ȡ���ļ���
	LPCTSTR pszFind = _tcsrchr(lpszDstFile, _T('\\'));
	if(pszFind == NULL)
	{
		return FALSE;
	}

	xstring strDstFileName = pszFind+1;
	xstring strDstFileDir(lpszDstFile, pszFind - lpszDstFile+1);
	if(strDstFileName.size() == 0)
	{
		return FALSE;
	}	

	//�����ļ�Ŀ¼
	m_strDstFilePath = lpszDstFile;
	m_strSrcFileDir = lpszSrcDir;
	CreateDir(strDstFileDir.c_str());

	//����ZIP�ļ� 
	HZIP hz=CreateZip(lpszDstFile, pszPassword); 
	if(hz == 0) 
	{ 
		//����Zip�ļ�ʧ�� 
		return FALSE; 
	} 

	//�ݹ��ļ���,����ȡ���ļ�����ZIP�ļ� 	
	AddFileFromDir(hz, m_strSrcFileDir); 

	//�ر�ZIP�ļ� 
	CloseZip(hz); 

	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(lpszDstFile)) 
	{ 
		//ѹ��ʧ��(δ����ѹ������ļ�) 
		return FALSE; 
	} 
	return TRUE; 
} 

BOOL CZipx::ZipFiles2Part(LPCTSTR lpszSrcDir, LPCTSTR lpszFilterExt, LPCTSTR lpszExcludeFile, const char *pszPassword)
{
	if(lpszSrcDir == NULL)
	{
		return FALSE;
	}

	//���ԴĿ¼�Ƿ����
	if(!IsDirExist(lpszSrcDir))
	{
		return FALSE;
	}

	vector<xstring> filterList;
	GetFilterFileFromDir(lpszSrcDir, lpszFilterExt, filterList);
	for (vector<wstring>::iterator itr=filterList.begin();itr!=filterList.end();itr++)
	{
		xstring srctemp = itr->c_str();
		xstring strdstFileName;
		//����Ŀ���ļ���׺
		string::size_type nFind = srctemp.find_last_of(_T('.'));
		if(nFind != string::npos)
		{
			strdstFileName = srctemp.substr(0, nFind) + _T(".zip");
		}
		else
		{
			strdstFileName = srctemp + _T(".zip");
		}

		xstring excludeFile = lpszExcludeFile;
		xstring temp = srctemp.substr(srctemp.length() - excludeFile.length());
		if(temp == excludeFile) continue;

		ZipFile(srctemp.c_str(), strdstFileName.c_str(), pszPassword);

		DeleteFileW(srctemp.c_str());
	}


	return TRUE; 
}

void CZipx::AddFileFromDir(HZIP hz, const xstring &strFileDir) 
{ 
	WIN32_FIND_DATA wfd = {0}; 
	xstring strDir = strFileDir; 
	PathAddBackslash2(strDir);

	HANDLE hFind = FindFirstFile((strDir+_T("*.*")).c_str(), &wfd); 

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		if (wfd.cFileName[0] == _T('.'))
		{
			if (wfd.cFileName[1] == _T('\0') ||
				(wfd.cFileName[1] == _T('.') &&	wfd.cFileName[2] == _T('\0')))
			{
				continue;
			}
		}

		xstring strFilePath = strDir + wfd.cFileName;

		//�����һ����Ŀ¼���õݹ��������һ���� 
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			xstring subPath; 
			GetRelativePath(strFilePath, subPath); 

			ZipAddFolder(hz, subPath.c_str()); 
			AddFileFromDir(hz, strFilePath); 
		} 
		else
		{ 
			//�ų���ѹ���ĵ�
			if(_tcsicmp(m_strDstFilePath.c_str(), strFilePath.c_str()) == 0)
			{
				continue;
			}

			//��ʾ��ǰ���ʵ��ļ�(����·��) 
			xstring subPath; 
			GetRelativePath(strFilePath, subPath); 

			//���ļ���ӵ�ZIP�ļ�
			ZipAdd(hz, subPath.c_str(), strFilePath.c_str()); 
		} 
	}
	while( 0 != FindNextFile(hFind, &wfd));

	FindClose(hFind);
} 

void CZipx::GetFilterFileFromDir(xstring srcDir, xstring filterExt, vector<xstring>& filterList)
{
	WIN32_FIND_DATA wfd = {0}; 
	xstring strDir = srcDir; 
	PathAddBackslash2(strDir);

	HANDLE hFind = FindFirstFile((strDir+filterExt).c_str(), &wfd); 

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		if (wfd.cFileName[0] == _T('.'))
		{
			if (wfd.cFileName[1] == _T('\0') ||
				(wfd.cFileName[1] == _T('.') &&	wfd.cFileName[2] == _T('\0')))
			{
				continue;
			}
		}

		xstring strFilePath = strDir + wfd.cFileName;

		//�����һ����Ŀ¼���õݹ��������һ���� 
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
		} 
		else
		{ 
			//�ų���ѹ���ĵ�
			if(_tcsicmp(m_strDstFilePath.c_str(), strFilePath.c_str()) == 0)
			{
				continue;
			}

			filterList.push_back(strFilePath);
		} 
	}
	while( 0 != FindNextFile(hFind, &wfd));

	FindClose(hFind);
}

bool CZipx::UnZipFile(LPCTSTR lpszSrcFile, LPCTSTR lpszDstDir, const char *pszPassword)
{
	if(lpszSrcFile == NULL || lpszDstDir == NULL)
		return false;

	//���Դ�ļ��Ƿ����
	if(!IsFileExist(lpszSrcFile))
	{
		return false;
	}

	CreateDir(lpszDstDir);

	//��ZIP�ļ� 
	HZIP hz = OpenZip(lpszSrcFile, pszPassword); 
	if(hz == 0) 
	{ 
		//��Zip�ļ�ʧ�� 
		return false; 
	} 

	ZRESULT zr = SetUnzipBaseDir(hz, lpszDstDir); 
	if(zr != ZR_OK) 
	{ 
		//��Zip�ļ�ʧ�� 
		CloseZip(hz); 
		return FALSE;       
	} 

	ZIPENTRY ze;
	zr=GetZipItem(hz, -1, &ze); 
	if(zr != ZR_OK) 
	{ 
		//��ȡZip�ļ�����ʧ�� 
		CloseZip(hz); 
		return FALSE;       
	} 

	int nNumItems = ze.index; 
	for (int i=0; i<nNumItems; i++) 
	{ 
		zr = GetZipItem(hz, i, &ze); 
		zr = UnzipItem(hz, i, ze.name); 

		if(zr != ZR_OK) 
		{ 
			//��ȡZip�ļ�����ʧ�� 
			CloseZip(hz); 
			return false;       
		} 
	} 

	CloseZip(hz); 

	return true;
}

bool CZipx::UnZipFile(LPCTSTR lpszSrcFile, LPCTSTR lpszFileName, LPCTSTR lpszDstDir, const char *pszPassword)
{
	if(lpszSrcFile == NULL || lpszDstDir == NULL)
		return false;

	//���Դ�ļ��Ƿ����
	if(!IsFileExist(lpszSrcFile))
	{
		return false;
	}

	CreateDir(lpszDstDir);

	//��ZIP�ļ� 
	HZIP hz = OpenZip(lpszSrcFile, pszPassword); 
	if(hz == 0) 
	{ 
		//��Zip�ļ�ʧ�� 
		return false; 
	} 

	ZRESULT zr = SetUnzipBaseDir(hz, lpszDstDir); 
	if(zr != ZR_OK) 
	{ 
		//��Zip�ļ�ʧ�� 
		CloseZip(hz); 
		return FALSE;       
	} 

	int i = 0; 
	ZIPENTRY ze;
	zr = FindZipItem(hz, lpszFileName, true, &i, &ze);
	if(zr == ZR_OK) 
	{
		zr = UnzipItem(hz, i, ze.name);
	}
	CloseZip(hz);
	 
	return (zr == ZR_OK) ;
}

bool CZipx::UnZipFileFromMem(void* pData,DWORD dwDataSize,LPCTSTR lpszDstDir, const char *pszPassword/* =NULL */)
{
	if(pData == NULL || dwDataSize==0)
		return false;

	CreateDir(lpszDstDir);

	//��ZIP�ļ� 
	HZIP hz = OpenZip(pData,dwDataSize, pszPassword); 
	if(hz == 0) 
	{ 
		//��Zip�ļ�ʧ�� 
		return false; 
	} 

	ZRESULT zr = SetUnzipBaseDir(hz, lpszDstDir); 
	if(zr != ZR_OK) 
	{ 
		//��Zip�ļ�ʧ�� 
		CloseZip(hz); 
		return FALSE;       
	} 

	ZIPENTRY ze;
	zr=GetZipItem(hz, -1, &ze); 
	if(zr != ZR_OK) 
	{ 
		//��ȡZip�ļ�����ʧ�� 
		CloseZip(hz); 
		return FALSE;       
	} 

	int nNumItems = ze.index; 
	for (int i=0; i<nNumItems; i++) 
	{ 
		zr = GetZipItem(hz, i, &ze); 
		zr = UnzipItem(hz, i, ze.name); 

		if(zr != ZR_OK) 
		{ 
			//��ȡZip�ļ�����ʧ�� 
			CloseZip(hz); 
			return false;       
		} 
	} 

	CloseZip(hz); 

	return true;
}

bool CZipx::CreateDir(const xstring& strDir)
{
	if (PathIsDirectory(strDir.c_str()))
		return true;

	int nPos = strDir.find_last_of(_T("\\/"));
	if (nPos!=string::npos)
	{
		xstring strParentDir = strDir.substr(0,nPos);
		if (PathIsDirectory(strParentDir.c_str()) == FALSE)
		{
			if (CreateDir(strParentDir)==FALSE)
			{
				return false;
			}
		}

		if (CreateDirectory(strDir.c_str(), NULL) == FALSE)
		{
			return false;
		}		
	}
	else
		return false;

	return true;
}

void CZipx::GetRelativePath(const xstring& strFilePath, xstring& strSubPath) 
{ 
	strSubPath = strFilePath;
	if(_tcsnicmp(strFilePath.c_str(), m_strSrcFileDir.c_str(), m_strSrcFileDir.size()) == 0)
	{
		strSubPath = strFilePath.substr(m_strSrcFileDir.size());
		if(strSubPath[0] == '\\')
		{
			strSubPath = strSubPath.substr(1);
		}
	}
}

wstring CZipx::GetFileNameFromPath(const wstring& strPath)
{
	wstring strName = strPath;
	int nPos = strName.find_last_of(L"/\\");
	if (nPos != string::npos)
		strName.erase(0,nPos+1);
	return strName;
}
