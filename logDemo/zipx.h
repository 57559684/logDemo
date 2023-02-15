#pragma once
#include <windows.h>
#include <tchar.h>

#include "zip.h"
#include "unzip.h"

#include <string>
#include <vector>
using namespace std;
using std::string;
using std::wstring;

#ifdef  _UNICODE
#define xstring wstring
#else
#define xstring string
#endif

class CZipx
{
public:
	CZipx(void);
	~CZipx(void);

private:
	xstring m_strSrcFileDir;
	xstring m_strDstFilePath;

public:
	
	//ѹ��һ���ļ�
	bool ZipFile(LPCTSTR lpszSrcFile, LPCTSTR lpszDstFile=NULL, const char *pszPassword=NULL);
	BOOL ZipFiles(LPCTSTR lpszSrcDir, LPCTSTR lpszDstFile, const char *pszPassword=NULL) ;
	bool ZipFileList(vector<wstring>& srcFileList, const wstring& strDstFile, const char *pszPassword=NULL);
	BOOL ZipFiles2Part(LPCTSTR lpszSrcDir, LPCTSTR lpszFilterExt, LPCTSTR lpszExcludeFile, const char *pszPassword=NULL) ;

	//ѹ��һ���ļ���
	bool UnZipFile(LPCTSTR lpszSrcFile, LPCTSTR lpszDstDir, const char *pszPassword=NULL);
	bool UnZipFile(LPCTSTR lpszSrcFile, LPCTSTR lpszFileName, LPCTSTR lpszDstDir, const char *pszPassword=NULL);
	bool UnZipFileFromMem(void* pData,DWORD dwDataSize,LPCTSTR lpszDstDir, const char *pszPassword=NULL);

public:
	void PathAddBackslash2(xstring &strFilePath);
	bool IsFileExist(const xstring& strFilePath);
	bool IsDirExist(const xstring & strFilePath);
	bool IsPathExist(const xstring & strFilePath);

private:
	bool CreateDir(const xstring& strDir);
	//bool IsFolderExist(LPCTSTR lpszPath) ;
	void GetRelativePath(const xstring& strFilePath, xstring& strSubPath) ;
	void AddFileFromDir(HZIP hz, const xstring &strFileDir) ;
	void GetFilterFileFromDir(xstring srcDir, xstring filterExt, vector<xstring>& filterList);
	
	wstring GetFileNameFromPath(const wstring& strPath);
};