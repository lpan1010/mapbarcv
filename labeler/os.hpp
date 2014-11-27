/*
 * os.h
 *
 *  Created on: 2014年11月27日
 *      Author: qin
 */

#ifndef OS_H_
#define OS_H_

#include <vector>
#include <string>
#include <iostream>
#include <errno.h>

// 操作系统相关头文件
#ifdef _WIN32
#include <windows.h>
#include <strsafe.h>
#include <tchar.h>
#include <codecvt>

#else
#include <dirent.h>
#endif

using namespace std;


// string和wstring的处理
#ifndef _WIN32
typedef std::string QString;
typedef std::stringstream StringStream;
typedef std::ifstream ifstream;
typedef std::ofstream ofstream;
typedef std::ostream ostream;
#else
typedef std::wstring QString;
typedef std::wstringstream StringStream;
typedef std::wifstream ifstream;
typedef std::wofstream ofstream;
typedef std::wostream ostream;
#define cout std::wcout
#endif



#endif /* OS_H_ */
