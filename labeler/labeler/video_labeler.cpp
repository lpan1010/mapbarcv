/*
 * video_labeler.cpp
 *
 *  Created on: 2014,11,27
 *      Author: qin
 */

#include "video_labeler.hpp"




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
        //setMouseCallback("Label", label_mouse_callback);
}

void VideoLabeler::open_meta_file(String& meta_file_path) {
        meta_file_stream.open(meta_file_path.c_str(), std::ios::out | std::ios::app);
}

void VideoLabeler::save_current_progress(String& video_name, int& frame){
        ofstream progress;
        progress.open(PROGRESS_FILE.c_str(), std::ios::out);
        progress << video_name << '@' << frame << endl;
        progress.close();
}


string int2str(int& i){
        stringstream ss;
        ss << i;
        return ss.str();
}

bool VideoLabeler::label_video(String& video_file_path,
                int& frame_num_start_from,
                string& meta_file) {
        if (!load_video(video_file_path)){
                return false;
        }
        jump_to_frame(frame_num_start_from);
        open_meta_file(meta_file);
        fl = FrameLabeler(meta_file_stream);

        video >> frame;
        for(int frame_num = frame_num_start_from;;){

		if (frame.empty()){
		        cout << "End of video: " << video_file_path << endl;
		        break;
		}
                
		if (!fl.label_frame(frame, video_file_path, int2str(frame_num))){
		        save_current_progress(video_file_path, frame_num);
		        return false;
		}
                frame_num += 24;
                jump_to_frame(frame_num);
                video >> frame;
                refresh();
        }
        return true;
}

void VideoLabeler::jump_to_frame(int& frame_num) {
        int frame_count= (int)video.get(CV_CAP_PROP_FRAME_COUNT);
        if (frame_count <= frame_num){
                frame_num = frame_count - 1;
        }
        video.set(CV_CAP_PROP_POS_FRAMES,frame_num);
}

