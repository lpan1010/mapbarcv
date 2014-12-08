/*
 * ConfigFileReader.h
 *
 *  Created on: 2014,11,27
 *      Author: qin
 */

#ifndef CONFIGFILEREADER_HPP_
#define CONFIGFILEREADER_HPP_

#include "../global.hpp"

class ConfigFileReader {
        public:
                static void read(string& file, vector<string>& video_files);
                static bool read(string& file, string& video, int& frame_num);
                static bool read_pics_progress(const string& file, string& dir, string& pic);
};



#endif /* CONFIGFILEREADER_HPP_ */
