/*
 * ConfigFileReader.cpp
 *
 *  Created on: 2014,11,27
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

void ConfigFileReader::read(string& file, vector<string>& video_files) {
        ifstream config_file;
        config_file.open(file.c_str(), std::ios::in);
        string line;

        cout << "in config file reader::read" << endl;
        video_files.clear();
        while (getline(config_file, line)) {
                video_files.push_back(line);
        }
        // sort
        //std::sort(video_files.begin(), video_files.end());
        cout << "out config file reader::read" << endl;
}

bool ConfigFileReader::read_pics_progress(const string& file, string& dir, string& pic){
        ifstream config_file;
        config_file.open(file.c_str(), std::ios::in);

        if (!getline(config_file, dir)){
                return false;
        }else if (dir.empty()) {
                return false;
        }

        if (!getline(config_file, pic)){
                return false;
        }else if (pic.empty()) {
                return false;
        }
        cout << "Last progress ---->" << dir << ":" << pic << endl;
}


bool ConfigFileReader::read(string& file, string& video, int& frame_num) {
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
