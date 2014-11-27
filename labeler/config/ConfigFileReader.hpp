/*
 * ConfigFileReader.h
 *
 *  Created on: 2014年11月27日
 *      Author: qin
 */

#ifndef CONFIGFILEREADER_HPP_
#define CONFIGFILEREADER_HPP_

#include "../global.hpp"

class ConfigFileReader {
        public:
                static void read(const string& file, vector<string>& video_files);
                static bool read(const string& file, string& video, int& frame_num);
};



#endif /* CONFIGFILEREADER_HPP_ */
