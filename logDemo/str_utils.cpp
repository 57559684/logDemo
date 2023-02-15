

#include "str_utils.h"
#include <Windows.h>


std::string U2A(const std::wstring& str)
{
	std::string strDes;
	if ( str.empty() )
		return strDes;

	int nLen=::WideCharToMultiByte(CP_ACP, 0, str.c_str(), str.size(), NULL, 0, NULL, NULL);
	if ( 0==nLen )
		return strDes;

	char* pBuffer=new char[nLen+1];
	memset(pBuffer, 0, nLen+1);
	::WideCharToMultiByte(CP_ACP, 0, str.c_str(), str.size(), pBuffer, nLen, NULL, NULL);
	pBuffer[nLen]='\0';
	strDes.append(pBuffer);
	delete[] pBuffer;

	return strDes;
}

std::wstring A2U(const std::string& str)
{
	std::wstring strDes;
	if ( str.empty() )
		return strDes;

	int nLen=::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	if ( 0==nLen )
		return strDes;

	wchar_t* pBuffer=new wchar_t[nLen+1];
	memset(pBuffer, 0, nLen+1);
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), pBuffer, nLen);
	pBuffer[nLen]='\0';
	strDes.append(pBuffer);
	delete[] pBuffer;

	return strDes;
}

std::string U2Utf(const std::wstring& wstrUnicode)  
{  
	std::string strRet;
	if( wstrUnicode.empty() )
		return strRet;

	int nLen = WideCharToMultiByte(CP_UTF8, 0, wstrUnicode.c_str(), -1, NULL, 0, NULL, NULL);  
	char* pBuffer=new char[nLen+1];
	pBuffer[nLen] = '\0';
	nLen = WideCharToMultiByte(CP_UTF8, 0, wstrUnicode.c_str(), -1, pBuffer, nLen, NULL, NULL); 
	strRet.append(pBuffer);
	delete[] pBuffer;

	return strRet;  
}

std::wstring Utf2U(const std::string &str)
{
	int u16Len = ::MultiByteToWideChar(CP_UTF8, NULL,str.c_str(),(int)str.size(), NULL, 0);
	wchar_t* wstrBuf = new wchar_t[u16Len + 1];
	::MultiByteToWideChar(CP_UTF8, NULL, str.c_str(),(int)str.size(), wstrBuf, u16Len);
	wstrBuf[u16Len] = L'\0';
	std::wstring wStr;
	wStr.assign(wstrBuf, u16Len);
	delete [] wstrBuf;

	return wStr;
}

std::string	A2Utf(const std::string &str)
{
	std::wstring strTemp = A2U(str);
	return U2Utf(strTemp);
}

std::string Utf2A(const std::string &str)
{
	std::wstring strTemp = Utf2U(str);
	return U2A(strTemp);
}

std::wstring GetRunPathW()
{
	wchar_t szPath[MAX_PATH] = {0};
	GetModuleFileNameW(NULL, szPath, MAX_PATH);
	std::wstring strPath(szPath);
	int nPos = strPath.find_last_of(L"\\");
	strPath = strPath.substr(0, nPos+1);
	return strPath;
}

std::wstring GetPCName()
{
	std::wstring strName;
	wchar_t szName[MAX_PATH];
	DWORD dwSize = MAX_PATH;
	GetComputerName(szName, &dwSize);
	strName.append(szName);
	dwSize = MAX_PATH;
	GetUserName(szName, &dwSize);
	strName.append(L"/");
	strName.append(szName);
	return strName;
}


bool CreateGuid(std::wstring& strGuid)
{
	GUID id;
	WCHAR buff[40] = { '\0' };
	CHAR buff2[40] = { '\0' };
	if (FAILED(CoCreateGuid(&id)))
		return false;
	StringFromGUID2(id, buff, 40);
	strGuid.assign(buff);
	strGuid = strGuid.substr(1, strGuid.size() - 2);
	return true;
}

std::wstring StrLowerW(const std::wstring& str)
{
	std::wstring strOut;
	for (size_t i = 0; i < str.size(); ++i)
	{
		strOut += (wchar_t)towlower(str[i]);
	}
	return strOut;
}

std::string StrLowerA(const std::string& str)
{
	std::string strOut;
	for (size_t i = 0; i < str.size(); ++i)
	{
		strOut += (char)tolower(str[i]);
	}
	return strOut;
}

std::wstring StrUpperW(const std::wstring& str)
{
	std::wstring strOut;
	for (size_t i = 0; i < str.size(); ++i)
	{
		strOut += (wchar_t)towupper(str[i]);
	}
	return strOut;
}

std::string StrUpperA(const std::string& str)
{
	std::string strOut;
	for (size_t i = 0; i < str.size(); ++i)
	{
		strOut += (char)toupper(str[i]);
	}
	return strOut;
}


