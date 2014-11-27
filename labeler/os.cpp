/*
 * os.cpp
 *
 *  Created on: 2014年11月27日
 *      Author: qin
 */
#include <os.hpp>

/*
 操作系统文件接口相关
 */
#ifdef _WIN32
const string path_separator = "\\";

// TODO tchar与string的转换问题
int ls(QString dir, vector<QString> &file_names) {
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
                std::cout << "Error(" << errno << ") opening " << dir
                                << std::endl;
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


#ifdef _WIN32
std::wstring s2ws(const std::string& str)
{
        typedef std::codecvt_utf8<wchar_t> convert_typeX;
        std::wstring_convert<convert_typeX, wchar_t> converterX;

        return converterX.from_bytes(str);
}
std::string ws2s(const std::wstring& wstr)
{
        typedef std::codecvt_utf8<wchar_t> convert_typeX;
        std::wstring_convert<convert_typeX, wchar_t> converterX;

        return converterX.to_bytes(wstr);
}
#endif


