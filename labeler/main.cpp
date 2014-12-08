#include <iostream>
#include <fstream>

#include "global.hpp"
#include "config/ConfigFileReader.hpp"
#include "common_header.hpp"
#include "labeler/frame_labeler.hpp"

extern string CONFIG_FILE;
extern string PROGRESS_FILE;
extern string PICS_FILE;
extern string META_FILE;
extern FrameLabeler fl;

void lbuttonup_action(int x, int y) {
        PinStatus ret = current_label->pin(x, y);
        switch (ret) {
                case Second:
                        labels->push_back(current_label);
                        current_label = new Label();
                        break;
                case First:
                        break;
                case Locked:
                        // TODO error ?
                        break;
        }
}

void tracking_ruler(int x, int y) {
        mouse_y = y;
        mouse_x = x;
}

void label_mouse_callback(int event, int x, int y, int, void*) {
        switch (event) {
                case EVENT_LBUTTONUP:
                        lbuttonup_action(x, y);
                        break;
                case EVENT_MOUSEMOVE:
                        tracking_ruler(x, y);
                        break;
        }
        fl.refresh();
}


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


void resume_progress(
        vector<string>& dirs, 
        string& dir, 
        vector<string>& pics, 
        string& pic, 
        vector<string>::iterator& last_dir, 
        vector<string>::iterator& last_pic){
        // 上次的文件夹
        last_dir = std::find(dirs.begin(), dirs.end(), dir);
        if (last_dir == dirs.end()){
                last_dir = dirs.begin();

                string pics_file = (*last_dir)+PICS_FILE;
                ConfigFileReader::read(pics_file, pics);

                last_pic = pics.begin();
        }else{
                string pics_file = (*last_dir)+PICS_FILE;
                ConfigFileReader::read(pics_file, pics);

                last_pic = std::find(pics.begin(), pics.end(), pic);
                if (last_pic == pics.end()){
                        last_pic = pics.begin();
                }
        }

        cout << "Start from ----> " << *last_dir << "\\" << *last_pic << endl;
}

void load_progress(vector<string>& dirs, vector<string>::iterator& last_dir, vector<string>& pics, vector<string>::iterator& last_pic){
        string dir, pic;
        if (!ConfigFileReader::read_pics_progress(PROGRESS_FILE, dir, pic)){
                cout << "Last progress loading failed" << endl;
        }
        ConfigFileReader::read(CONFIG_FILE, dirs);
        cout << dirs.size()<< endl;
        cout << dirs[0] << endl;
        cout << "Resuming" << endl;
        resume_progress(dirs, dir, pics, pic, last_dir, last_pic);
}

int main(int argc, char *argv[]) {

        cout << "Hello ...." << endl;
        vector<string> dirs;
        vector<string> pics;
        vector<string>::iterator dir_pos;
        vector<string>::iterator pic_pos;

        ConfigFileReader::read(CONFIG_FILE, dirs);
        load_progress(dirs, dir_pos, pics, pic_pos);

        fl.init();
        namedWindow("Label");
        setMouseCallback("Label", label_mouse_callback);

        while(dir_pos != dirs.end()) {
                string dir = *dir_pos;
                cout << "Video dir: " << dir << endl;

                for(; pic_pos != pics.end(); ++pic_pos){
                        string pic = *pic_pos;
                        string frame_file = dir + "\\" + pic;

                        cout << "Frame file: " << frame_file << endl;
                        Mat frame = imread(frame_file);
                        if (!fl.label_frame(frame, dir, pic)){
                                return 0;
                        }
                        fl.refresh();
                }
                ++dir_pos;
                if (dir_pos == dirs.end()){
                        return 0;
                }

                // TODO load next list.txt
                string pics_file = (*dir_pos)+PICS_FILE;
                ConfigFileReader::read(pics_file, pics);
                pic_pos = pics.begin();
                
        }
        return 0;
}

