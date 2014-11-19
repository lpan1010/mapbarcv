/*
 * logCplus.h
 *
 *  Created on: 2014年10月16日
 *      Author: kangyong
 */

#ifndef LOGCPLUS_H_
#define LOGCPLUS_H_

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <sstream>
using namespace std;

class LogCPlus {
private:
	bool bLogEnable; //是否开启日志记录
	ofstream logFile;
	template<typename T>
	string ValueToStr(T value) {

	}
public:
	bool openLogFile(string fileName);
	void closeLogFile();
	string getTimeStr();
	void setLogEnable();
	void setLogDisable();
	bool isLogEnable();
	template<typename T>
	void writeLog(const T& value) {
		logFile << value;
	}
	//运算符重载
//	LogCPlus& operator<<(ostream& (*_Pfn)(ostream&)) {
//		(*_Pfn)(logFile);
//		return *this;
//	}
//	template<typename T>
//	LogCPlus& operator<<(const T& value) {
//		logFile << value;
//		return *this;
//	}

	template<typename T>
	void writeLogLine(const T& value) {
		logFile << value;
		logFile << "\n\t";
	}

};

#endif /* LOGCPLUS_H_ */
