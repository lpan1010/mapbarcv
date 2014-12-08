/*
 * video_labeler.hpp
 *
 *  Created on: 2014,11,27
 *      Author: qin
 */

#ifndef VIDEO_LABELER_HPP_
#define VIDEO_LABELER_HPP_

#include "frame_labeler.hpp"
#include "../common_header.hpp"
#include "../os.hpp"
#include "label.hpp"



class VideoLabeler {
        public:
                VideoLabeler();
                bool label_video(String& video_file_path,
                                int& frame_num,
                                String& meta_file);
                bool label_video(String& video_file_path, int& frame_num, String& meta_file,
                                String& output_dir);
                void jump_to_frame(int& frame_num);

                // FIXME this is a so fxxking ugly design.
                void refresh();

                void save_current_progress(String& video_name, int& frame);

        private:
                bool load_video(String&);
                void open_meta_file(String& meta_file_path);

                VideoCapture video;
                Mat frame;
                FrameLabeler fl;
                ofstream meta_file_stream;

};

extern int mouse_x;
extern int mouse_y;
extern VideoLabeler vl;
extern vector<Label*> * labels;
extern Label* current_label;


#endif /* VIDEO_LABELER_HPP_ */
