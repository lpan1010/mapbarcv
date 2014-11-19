#include <iostream>
#include <fstream>

#include "label.hpp"


// TODO temp global vars
string meta_file, video_path, video_file, patch_dir;
int frame_num;


enum PinStatus {
        Locked, First, Second
};

class Label {
        private:
                bool is_first_pin;
                bool locked;
        public:
                CvPoint left_top_point;
                CvPoint right_bottom_point;
                Label() :is_first_pin(true), locked(false) {
                }

                void print_on(Mat img) {
                        if (locked) {
                                rectangle(img, left_top_point,
                                                right_bottom_point,
                                                Scalar(0, 0, 255));
                        } else {
                                circle(img, left_top_point, 2,
                                                Scalar(0, 0, 255));
                        }
                }

                String to_string() {
                        StringStream ss;
                        int height = right_bottom_point.y - left_top_point.y;
                        int width = right_bottom_point.x - left_top_point.x;
                        ss << left_top_point.x << "," << left_top_point.y << ","
                                        << width << "," << height;
                        return ss.str();
                }

                PinStatus pin(int x, int y) {
                        // Two points are both set.
                        if (locked) {
                                return Locked;
                        }

                        if (is_first_pin) {
                                left_top_point.x = x;
                                left_top_point.y = y;
                                is_first_pin = false;
                                return First;
                        } else {
                                right_bottom_point.x = x;
                                right_bottom_point.y = y;
                                locked = true;
                                return Second;
                        }
                }
                friend ostream& operator<<(ostream& os, const Label& dt);
};

// 不是很需要
std::ostream& operator<<(std::ostream& os, const Label& l) {
        // write obj to stream
        int height = l.right_bottom_point.y - l.left_top_point.y;
        int width = l.right_bottom_point.x - l.left_top_point.x;
        os << l.left_top_point.x << "," << l.left_top_point.y << "," << width
                        << "," << height;
        return os;
}

// 全局变量
Mat img;
int mouse_x;
int mouse_y;
ofstream meta_file_stream;
Label* current_label;
vector<Label*> * labels;

void clear_labels() {
        for (size_t i = 0; i < labels->size(); i++) {
                delete (*labels)[i];
        }
        delete labels;
        delete current_label;

        labels = new vector<Label*>();
        current_label = new Label();
}

void refresh() {
        Mat output = img.clone();
        current_label->print_on(output);
        for (size_t i = 0; i < labels->size(); ++i) {
                (*labels)[i]->print_on(output);
        }
        line(output, Point(0, mouse_y), Point(img.cols, mouse_y),
                        Scalar(0, 255, 0));
        line(output, Point(mouse_x, 0), Point(mouse_x, img.rows),
                        Scalar(0, 255, 0));
        imshow("Label", output);
}

enum KeyAction {
        SaveAndNext,
        CancelCurrentLabel,
        Exit,
        NoOp,
        DeletePreviousLabel,
        DeleteAllLabels
};

KeyAction get_key_seq() {
        char key = waitKey(0);
        KeyAction ret;

        switch (key) {
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

void cancel_label() {
        delete current_label;
        current_label = new Label();
}

void quit_prog() {
        meta_file_stream.close();
        exit(0);
}

void delete_previous_label() {
        if (!labels->empty()) {
                delete labels->back();
                labels->pop_back();
        }
}

void tag_img_file(string file_name) {
        clear_labels();
        refresh();
        while (true) {
                KeyAction action = get_key_seq();
                switch (action) {
                        case SaveAndNext:
                                save_label(file_name, (*labels));
                                return;
                        case CancelCurrentLabel:
                                cancel_label();
                                break;
                        case Exit:
                                quit_prog();
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
        return;
}

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
        refresh();
}


float ratio = 1/1.25;
String labels_to_string(vector<Label*> &labels) {
        StringStream ss;
        for (size_t i = 0; i < labels.size(); i++) {
                Label *l = labels[i];
                ss << " ";
                ss << l->to_string();

                // fixme error?
                float width = l->right_bottom_point.x - l->left_top_point.x;
                float height = width*ratio;
                cout << "debuging..." << endl;
                cout << width << endl << height << endl;

                Rect roi(l->right_bottom_point.x, l->right_bottom_point.y, width, height);
                if (roi.height+roi.y > img.rows || roi.width+roi.x > img.cols){
                        continue;
                }
                Mat smallImage = Mat(img, roi);
                stringstream ss;
                ss << patch_dir;
                ss << video_file;
                ss << frame_num;
                ss << '.';
                ss << i;
                ss << ".jpg";
                string s = ss.str();
                cout << "Writing to file:" << s << endl;
                imwrite(s, smallImage);
        }
        return ss.str();
}

void save_label(String& file_name, vector<Label*> &labels) {
        if (labels.size() == 0) {
                return;
        }
        String text_line = file_name + labels_to_string(labels);
        meta_file_stream << text_line << endl;
}

void init() {
        labels = new vector<Label*>();
        current_label = new Label();
        namedWindow("Label");
        setMouseCallback("Label", label_mouse_callback);
}



int main(int argc, char *argv[]) {
	VideoCapture video;
	meta_file = "/tmp/neg.info";
	video_path = "/home/qin/car_rear/video/141103/";
	video_file = "1414923207.mov";
	patch_dir = "/home/qin/tmp/negs/";

        meta_file_stream.open(meta_file.c_str(), ios::out | ios::app);
	namedWindow("output", 1);
	int jump = 50, c = 0;
	init();

	frame_num = 0;
	if(!video.open(video_path + video_file))
		return 1;
	for(;;c++,frame_num++){
		video>>img;
		if(img.empty()){break;}
		if (c == jump){
		        stringstream ss;
		        ss << video_file;
		        ss << '.';
		        ss << frame_num;
		        c = 0;
		        tag_img_file(ss.str());
		}
	}
        return 0;
}

