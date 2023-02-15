#pragma once

#ifndef _STR_UTILS_H_
#define _STR_UTILS_H_

#include <xstring>


std::string	U2A(const std::wstring& str);

std::wstring A2U(const std::string& str);

std::string	U2Utf(const std::wstring& wstrUnicode) ;

std::wstring Utf2U(const std::string &str);

std::string	A2Utf(const std::string &str);

std::string Utf2A(const std::string &str);

std::wstring GetRunPathW();

std::wstring GetPCName();

bool	CreateGuid(std::wstring& strGuid);

std::wstring	StrLowerW(const std::wstring& str);
std::string	StrLowerA(const std::string& str);

std::wstring StrUpperW(const std::wstring& str);
std::string  StrUpperA(const std::string& str);

#endif  /* _STR_UTILS_H_ */