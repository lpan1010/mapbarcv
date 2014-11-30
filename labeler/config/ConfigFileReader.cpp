/*
 * ConfigFileReader.cpp
 *
 *  Created on: 2014年11月27日
 *      Author: qin
 */

#include "ConfigFileReader.hpp"


void split(const std::string &s,
                char delim,
                vector<std::string> &elems) {
        stringstream ss(s);
        string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
}

void ConfigFileReader::read(const string& file, vector<string>& video_files) {
        ifstream config_file;
        config_file.open(file.c_str(), std::ios::in);
        string line;
        video_files.clear();
        while (getline(config_file, line)) {
                video_files.push_back(line);
        }
        // sort
        std::sort(video_files.begin(), video_files.end());
}

bool ConfigFileReader::read(const string& file, string& video, int& frame_num) {
        ifstream config_file;
        vector<string> segs;
        config_file.open(file.c_str(), std::ios::in);
        string line;

        if (!getline(config_file, line)){
                return false;
        }

        if (line.empty()){
                return false;
        }
        split(line, '@', segs);
        video = segs[0];
        frame_num = atoi(segs[1].c_str());
        cout << "Last progress ----> " << video << ':' << frame_num << endl;
        return true;
}

/*int main(int argc, char **argv) {
 vector<string> fns;
 ConfigFileReader::read("/home/qin/rules/DoorUnwatcher", fns);
 for(vector<string>::iterator it = fns.begin(); it != fns.end(); it++){
 cout << *it << endl;
 }
 }*/
