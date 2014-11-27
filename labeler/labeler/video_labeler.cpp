/*
 * video_labeler.cpp
 *
 *  Created on: 2014年11月27日
 *      Author: qin
 */

#include "video_labeler.hpp"

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
        vl.refresh();
}


void VideoLabeler::refresh() {
        fl.refresh();
}

bool VideoLabeler::load_video(String& video_file_path) {
        if (!video.open(video_file_path)){
                return false;
        }
        return true;
}

VideoLabeler::VideoLabeler() {
        namedWindow("Label");
        setMouseCallback("Label", label_mouse_callback);
}

void VideoLabeler::open_meta_file(String& meta_file_path) {
        meta_file_stream.open(meta_file_path.c_str(), std::ios::out | std::ios::app);
}

void VideoLabeler::save_current_progress(String& video_name, int& frame){
        ofstream progress;
        progress.open(PROGRESS_FILE.c_str(), std::ios::out);
        progress << video_name << ':' << frame << endl;
        progress.close();
}

bool VideoLabeler::label_video(String& video_file_path,
                int& frame_num_start_from,
                String& meta_file) {
        if (!load_video(video_file_path)){
                return false;
        }
        jump_to_frame(frame_num_start_from);
        open_meta_file(meta_file);
        fl = FrameLabeler(meta_file_stream);

        for(int frame_num = frame_num_start_from;; ++frame_num){
                video >> frame;
		if (frame.empty()){
		        cout << "End of video: " << video_file_path << endl;
		        break;
		}

		if (!fl.label_frame(frame, video_file_path, frame_num)){
		        save_current_progress(video_file_path, frame_num);
		        return false;
		}
        }
        return true;
}

void VideoLabeler::jump_to_frame(int& frame_num) {
        int frame_count= video.get(CV_CAP_PROP_FRAME_COUNT);
        if (frame_count <= frame_num){
                frame_num = frame_count - 1;
        }
        video.set(CV_CAP_PROP_POS_FRAMES,frame_num);
}

