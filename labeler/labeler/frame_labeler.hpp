/*
 * frame_labeler.hpp
 *
 *  Created on: 2014年11月27日
 *      Author: qin
 */

#ifndef FRAME_LABELER_HPP_
#define FRAME_LABELER_HPP_

#include "../global.hpp"

#include "../common_header.hpp"
#include "../os.hpp"
#include "label.hpp"


// Global Vars
extern int mouse_x;
extern int mouse_y;
extern vector<Label*> * labels;
extern Label* current_label;
extern const string CONFIG_FILE;
extern const string PROGRESS_FILE;
extern string META_FILE;


class FrameLabeler {
        public:
                FrameLabeler(){}
                FrameLabeler(ofstream& meta_file_stream) {
                        frame = NULL;
                        labels = NULL;
                        this->meta_file_stream = &meta_file_stream;
                        current_label = new Label();
                }
                bool label_frame(const Mat& frame,
                                const String& video_file_name,
                                const int& frame_num);
                void refresh();
        private:
                // Vars
                const Mat* frame;
                ofstream* meta_file_stream;

                // Functions
                void clear_labels();
                bool label_loop(const Mat& frame, const String& video_file_name,
                                const int& frame_num);

                void save_label(const String& video_file_name,
                                const int& frame_num,
                                const vector<Label*> &labels);
                QString labels_to_string(const vector<Label*> &labels);

                KeyAction get_key_seq();
                void quit_prog(const QString& file_name, const int& frame_num);
                void cancel_label();
                void delete_previous_label();
                void label_mouse_callback(int event, int x, int y, int, void*);
                void init();
};


#endif /* FRAME_LABELER_HPP_ */
