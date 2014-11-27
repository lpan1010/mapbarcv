/*
 * frame_labeler.cpp
 *
 *  Created on: 2014年11月27日
 *      Author: qin
 */

#include "frame_labeler.hpp"

void FrameLabeler::init() {
        labels = new vector<Label*>();
        current_label = new Label();

}

bool FrameLabeler::label_frame(const Mat& frame, const String& video_file_name,
                const int& frame_num) {
        this->frame = &frame;

        clear_labels();

        return label_loop(frame, video_file_name, frame_num);
}

KeyAction FrameLabeler::get_key_seq() {
        char key = waitKey(0);
        KeyAction ret;

        switch (key) {
                // The Esc key value.
                case 27:
                        ret = CancelCurrentLabel;
                        break;
                case 's':
                        ret = SaveAndNext;
                        break;
                case 'q':
                        ret = Exit;
                        break;
                case 'd':
                        ret = DeletePreviousLabel;
                        break;
                case 'r':
                        ret = DeleteAllLabels;
                        break;
                default:
                        ret = NoOp;
        }
        return ret;
}

void FrameLabeler::clear_labels() {
        if (labels != NULL) {
                for (size_t i = 0; i < labels->size(); i++) {
                        delete (*labels)[i];
                }
        }

        delete labels;
        if (current_label != NULL) {
                delete current_label;
        }

        labels = new vector<Label*>();
        current_label = new Label();
}

void FrameLabeler::quit_prog(const QString& file_name, const int& frame_num) {
        /*(*meta_file_stream).close();
         ofstream last_file;
         last_file.open(last_pos_file.c_str(), std::ios::out);
         last_file << file_name << std::endl;
         last_file.close();
         exit(0);*/
}
QString FrameLabeler::labels_to_string(const vector<Label*> &labels) {
        StringStream ss;
        for (size_t i = 0; i < labels.size(); i++) {
                Label *l = labels[i];
                ss << " ";
                ss << l->to_string();
                /*Rect rect;
                rect.x = l->left_top_point.x;
                rect.y = l->left_top_point.y;
                rect.height = l->right_bottom_point.y - l->left_top_point.y;
                rect.width = l->right_bottom_point.x - l->left_top_point.x;
                Mat sm = Mat(*frame, rect);
                imshow("Rect", sm);
                waitKey();*/
        }
        return ss.str();
}

void FrameLabeler::save_label(const String& video_file_name,
                const int& frame_num, const vector<Label*> &labels) {
        if (labels.size() == 0) {
                return;
        }
        StringStream ss;
        ss << video_file_name << ':' << frame_num << labels_to_string(labels);
        (*meta_file_stream) << ss.str() << std::endl;

}

void FrameLabeler::delete_previous_label() {
        if (!labels->empty()) {
                delete labels->back();
                labels->pop_back();
        }
}
void FrameLabeler::cancel_label() {
        delete current_label;
        current_label = new Label();
}

void FrameLabeler::refresh() {
        Mat output = frame->clone();
        current_label->print_on(output);
        for (size_t i = 0; i < labels->size(); ++i) {
                (*labels)[i]->print_on(output);
        }
        line(output, Point(0, mouse_y), Point(output.cols, mouse_y),
                        Scalar(0, 255, 0));
        line(output, Point(mouse_x, 0), Point(mouse_x, output.rows),
                        Scalar(0, 255, 0));
        imshow("Label", output);
}

// TODO
bool FrameLabeler::label_loop(const Mat& frame, const String& video_file_name,
                const int& frame_num) {
        while (true) {
                KeyAction action = get_key_seq();
                switch (action) {
                        case SaveAndNext:
                                save_label(video_file_name, frame_num,
                                                (*labels));
                                return true;
                        case CancelCurrentLabel:
                                cancel_label();
                                break;
                        case Exit:
                                //quit_prog (video_file_name, frame_num);
                                return false;
                                break;
                        case DeleteAllLabels:
                                clear_labels();
                                break;
                        case DeletePreviousLabel:
                                delete_previous_label();
                                break;
                        case NoOp:
                                break;
                }
                refresh();
        }
}
