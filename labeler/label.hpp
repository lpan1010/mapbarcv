#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <errno.h>


#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>

// 操作系统相关头文件
#ifdef _WIN32
#include <windows.h>
#include <strsafe.h>
#include <tchar.h>
#include <codecvt>

#else
#include <dirent.h>
#endif

using namespace cv;

#define UNICODE 1

// string和wstring的处理
#ifndef UNICODE
typedef std::string QString;
typedef std::stringstream StringStream;
typedef std::ifstream ifstream;
typedef std::ofstream ofstream;
typedef std::ostream ostream;
typedef std::cout cout;
#else
typedef std::wstring QString;
typedef std::wstringstream StringStream;
typedef std::wifstream ifstream;
typedef std::wofstream ofstream;
typedef std::wostream ostream;
#define cout std::wcout
#endif

/*
 操作系统文件接口相关
 */
#ifdef _WIN32
const QString path_separator = TEXT("\\");

// TODO tchar与string的转换问题
int ls(QString dir, vector<QString> &file_names){
        WIN32_FIND_DATA ffd;
        LARGE_INTEGER filesize;
        TCHAR szDir[MAX_PATH];
        size_t length_of_arg;
        HANDLE hFind = INVALID_HANDLE_VALUE;
        DWORD dwError=0;
        // Check that the input path plus 3 is not longer than MAX_PATH.
        // Three characters are for the "\*" plus NULL appended below.
        StringCchLength(dir.c_str(), MAX_PATH, &length_of_arg);

        if (length_of_arg > (MAX_PATH - 3)) {
                cout << "\nDirectory path is too long." << endl;
                return (-1);
        }
        // Prepare string for use with FindFile functions. First, copy the
        // string to a buffer, then append '\*' to the directory name.

        StringCchCopy(szDir, MAX_PATH, dir.c_str());
        StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

        // Find the first file in the directory.
        hFind = FindFirstFile(szDir, &ffd);

        if (INVALID_HANDLE_VALUE == hFind) {
                cout << "FindFirstFile" << endl;
                return dwError;
        }

        // List all the files in the directory with some info about them.
        do {
                if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                        // do nothing
                }
                else {
                        //cout <<ffd.cFileName << endl;
                        file_names.push_back(ffd.cFileName);
                }
        }
        while (FindNextFile(hFind, &ffd) != 0);

        dwError = GetLastError();
        if (dwError != ERROR_NO_MORE_FILES) {
                cout << "FindFirstFile" << endl;
        }
        FindClose(hFind);
        return 0;
}
#else
const QString path_separator = "/";

int ls(QString dir, vector<QString> &files) {
        DIR *dp;
        struct dirent *dirp;
        if ((dp = opendir(dir.c_str())) == NULL) {
                std::cout << "Error(" << errno << ") opening " << dir << std::endl;
                return errno;
        }

        while ((dirp = readdir(dp)) != NULL) {
                QString name = QString(dirp->d_name);
                if (name == "." || name == "..") {
                        continue;
                }
                files.push_back(name);
        }
        closedir(dp);
        return 0;
}
#endif

class Label;
void save_label(QString &dir, QString &file_name, vector<Label*> &labels);

