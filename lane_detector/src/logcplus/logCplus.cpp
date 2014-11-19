/*
 * logCplus.cpp
 *
 *  Created on: 2014年10月16日
 *      Author: kangyong
 */
#include "logCplus.h"

bool LogCPlus::openLogFile(string fileName) {
	logFile.open(fileName.c_str(), ios_base::out | ios_base::app);
	if (!logFile) {
		cerr << "open logFile false !" << endl;
		return false;
	}
	return true;
}
void LogCPlus::closeLogFile() {
	if (logFile.is_open()) {
		logFile.close();
	}
}
string LogCPlus::getTimeStr() {
	time_t tNowTime;
	time(&tNowTime);
	tm* tLocalTime = localtime(&tNowTime);
	string strDateTime = ValueToStr(tLocalTime->tm_year + 1900) + "-";

}

void  LogCPlus::setLogEnable(){
	bLogEnable=true;
}
void  LogCPlus::setLogDisable(){
	bLogEnable=false;
}
bool  LogCPlus::isLogEnable(){
	return bLogEnable;
}

