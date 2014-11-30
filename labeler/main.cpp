#include <iostream>
#include <fstream>

#include "global.hpp"
#include "config/ConfigFileReader.hpp"
#include "common_header.hpp"
#include "labeler/video_labeler.hpp"

extern const string CONFIG_FILE;
extern const string PROGRESS_FILE;
extern string META_FILE;
extern VideoLabeler vl;

bool resume_progress(vector<string>& names, vector<string>::iterator& it,
                string& last) {
        it = std::find(names.begin(), names.end(), last);
        if (it == names.end()) {
                it = names.begin();
                cout << "Not found ..." << endl;
                return false;
        }
        return true;
}

int main(int argc, char *argv[]) {
        cout << "Hello ...." << endl;
        vector<string> video_names;
        vector<string>::iterator it;

        ConfigFileReader::read(CONFIG_FILE, video_names);
        cout << "Configuration file loaded." << endl;
        string last_video;
        int frame_num = 0;

        if (ConfigFileReader::read(PROGRESS_FILE, last_video, frame_num)
                        && resume_progress(video_names, it, last_video)) {
                cout << "Resume last progress." << endl;
                cout << "Video: " << *it << endl;
                if (!vl.label_video((*it), frame_num, META_FILE)){
                        return 0;
                }
                it++;
        }else{
                cout << "Last progress not found. Start from the beginning." << endl;
                it = video_names.begin();
                cout << video_names.size()<< endl;
        }

        frame_num = 0;

        for (; it != video_names.end(); it++) {
                string name = *it;
                cout << "Video: " << *it << endl;
                if (!vl.label_video(name, frame_num, META_FILE)){
                        return 0;
                }
        }
        return 0;
}

