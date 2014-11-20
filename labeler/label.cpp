#include <iostream>
#include <fstream>

#include "label.hpp"
enum PinStatus {
        Locked, First, Second
};

class Label {
        private:
                bool is_first_pin;
                bool locked;
                CvPoint left_top_point;
                CvPoint right_bottom_point;
        public:
                Label() :
                                is_first_pin(true), locked(false) {
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

                QString to_string() {
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
ostream & operator<<(ostream& os, const Label& l) {
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

// 保存最后位置的文件
const String last_pos_file = "./last.ini";

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

void quit_prog(QString& file_name) {
        meta_file_stream.close();
        ofstream last_file;
        last_file.open(last_pos_file.c_str(), std::ios::out);
        last_file << file_name << std::endl;
        last_file.close();
        exit(0);
}

void delete_previous_label() {
        if (!labels->empty()) {
                delete labels->back();
                labels->pop_back();
        }
}

#ifdef _WIN32
std::wstring s2ws(const std::string& str)
{
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}
std::string ws2s(const std::wstring& wstr)
{
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}
#endif

void tag_img_file(QString& dir, QString& file_name) {
        clear_labels();
        String path;
#ifdef _WIN32
        img = imread(ws2s(dir + file_name));
#else
        img = imread(dir + file_name);
#endif
        refresh();
        while (true) {
                KeyAction action = get_key_seq();
                switch (action) {
                        case SaveAndNext:
                                save_label(dir, file_name, (*labels));
                                return;
                        case CancelCurrentLabel:
                                cancel_label();
                                break;
                        case Exit:
                                quit_prog(file_name);
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

QString labels_to_string(vector<Label*> &labels) {
        StringStream ss;
        for (size_t i = 0; i < labels.size(); i++) {
                Label *l = labels[i];
                ss << " ";
                ss << l->to_string();
        }
        return ss.str();
}

void save_label(QString& dir, QString& file_name, vector<Label*> &labels) {
        if (labels.size() == 0) {
                return;
        }

        QString text_line = file_name + labels_to_string(labels);
        meta_file_stream << text_line << std::endl;
}

void init() {
        labels = new vector<Label*>();
        current_label = new Label();
        namedWindow("Label");
        setMouseCallback("Label", label_mouse_callback);
}

int main(int argc, char *argv[]) {

#ifdef _WIN32
        QString dir = L"pics/";
        QString meta_file = L"label.txt";
#else
        QString dir = "pics/";
        QString meta_file = "label.txt";
#endif
        if (argc == 3) {
#ifdef _WIN32
                dir = s2ws(String(argv[1]) + path_separator);
                meta_file = s2ws(String(argv[2]));
#else
                dir = QString(argv[1]) + path_separator;
                meta_file = QString(argv[2]);
#endif
        }

        // 获得上次保存数据
        std::cout << "Reading the last label pic..." << std::endl;
        ifstream last_file;
        QString last;
        last_file.open(last_pos_file.c_str(), std::ios::in);
        getline(last_file, last);
        last_file.clear();

        meta_file_stream.open(meta_file.c_str(), std::ios::out | std::ios::app);
        vector<QString> file_names;
        ls(dir, file_names);
        sort(file_names.begin(), file_names.end());

        std::cout << "Initializing..." << std::endl;
        init();

        std::cout << "Finding the last pic..." << std::endl;
        vector<QString>::iterator it = find(file_names.begin(), file_names.end(),
                        last);
        if (it == file_names.end()){
                std::cout << "Last pic not found, start from first pic." << std::endl;
                it = file_names.begin();
        }else{
                std::cout << "Last pic found." << std::endl;
        }
        for (; it != file_names.end(); it++) {
                init();
                QString qs = (*it);
                std::cout << qs << std::endl;
                tag_img_file(dir, qs);
        }
        return 0;
}

