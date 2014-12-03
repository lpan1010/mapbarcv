#include <string>
#include <fstream>
#include <stdlib.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace cv;
using namespace std;

double ratio = -1.0;

typedef std::string String;

void split(const std::string &s,
                char delim,
                vector<std::string> &elems) {
        stringstream ss(s);
        string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
}

/**
 * Parser the string format roi into Rect struct.
 */
Rect* parse_roi(String& roi_str){
        vector<String> pos;
        split(roi_str, ',', pos);
        vector<int> pos_int;
        for(size_t i = 0; i < pos.size(); ++i){
                pos_int.push_back(atoi(pos[i].c_str()));
        }
        if(pos_int[2]<0 || pos_int[3]<0){
                return NULL;
        }
        if (ratio < 0){
                return (new Rect(pos_int[0], pos_int[1], pos_int[2], pos_int[3]));
        }else{
                return (new Rect(pos_int[0], pos_int[1], pos_int[2], pos_int[2] * ratio));
        }
}

/**
 * Cut the roi region from img into small.
 */
int cut_pic(Mat& img, String& roi_str, Mat& small){
        Rect* roi= parse_roi(roi_str);
        if (roi == NULL){
                return -1;
        }
        int width;
        int height;
        Point roi_center;
        roi_center.x = roi->x+roi->width/2;
        roi_center.y = roi->y+roi->height/2;

        float min_height = roi->height*1.1;
        float min_height_width = min_height/4*5;

        float min_width = roi->width*1.1;
        float min_width_height = min_width/5*4;

        if (min_height < min_width_height){
                height = min_width_height;
                width = min_width;
        }
        if (min_width <  min_height_width){
                width = min_height_width;
                height = min_height;
        }
        roi->x = roi_center.x - width/2;
        roi->y = roi_center.y - height/2;
        roi->width = width;
        roi->height = height;
        cout << img.cols << ',' <<img.rows << endl;
        cout << roi->x + roi->width << endl;
        cout << roi->y + roi->height << endl;
        if (roi-> x < 0|| roi-> y < 0 || roi->x+width > img.cols || roi -> y+height > img.rows){
                return -1;
        }
        small= Mat(img, (*roi));
        delete roi;
        return 1;
}


inline Mat load_pic(const String& dir, const String & name){
        cout << "load file:" << dir+name<<endl;
        //TODO the path separate char.
        return imread(dir+'/'+name);
}


inline void write_pic(const Mat& img, const String& path){
        imwrite(path, img);
}


bool filter_out_full_path(string& line){
        size_t found = line.find_last_of("\\");
        if (found == string::npos){
                return false;
        }
        line = line.substr(found+1);
        return true;
}


void read_label_file(const string& label_file, vector<string>& lines){
        lines.clear();
        ifstream file;
        file.open(label_file.c_str(), ios::in);
        string line;

        int li_no = 0;
        while(std::getline(file, line)){
                if (filter_out_full_path(line)){
                        lines.push_back(line);
                }else{
                        cout << "Filter out full path failed at line " << li_no << " : " << line << endl;
                }
                ++li_no;
        }
}
void parse_label_line(string& line, string& dir, string& file, vector<Rect*>& rois){
        vector<string> segs;
        split(line, ' ', segs);
        string dir_and_file = segs[0];
        vector<string> locs;
        split(dir_and_file, '@', locs);
        dir = locs[0];
        file = locs[1];

        rois.clear();
        for(size_t i = 1; i < segs.size(); ++i){
                rois.push_back(parse_roi(segs[i]));
        }
}

string small_name(const string& output_dir, string& dir, string& file, int j){
        stringstream ss;
        ss << output_dir << '/' << dir << '.' << file << '.' << j << ".jpg";
        return ss.str();
}

void cut_imgs(const string& output_dir, vector<string> label_lines){
        string dir;
        string file;
        vector<Rect*> rois;

        for(size_t i = 0; i < label_lines.size(); ++i){
                parse_label_line(label_lines[i], dir, file, rois);
                Mat img = imread(dir + '/' + file);
                //cout << dir + '/' + file << endl;
                //cout <<rois.size() << endl;
                for(size_t j = 0; j < rois.size(); ++j){
                        Rect roi = *rois[j];
                        if (roi.y+roi.height > img.rows) continue;
                        Mat small= Mat(img, roi);

                        // TODO resize the roi
                        Mat resize;
                        cv::resize(small, small, Size(30, 30));

                        cout << small_name(output_dir, dir, file, j) << endl;
                        imwrite(small_name(output_dir, dir, file, j), small);
                }
        }
}


/**
 * Argv:
 * 1. meta file [From label]
 * 2. pics dir [Original Images]
 * 3. output dir
 */
int main(int argc, char **argv) {
//        assert(argc >= 4);
//        char number_buffer[33];
//
//        String meta_file_path = argv[1];
//        String pics_dir = argv[2];
//        String output_dir = argv[3];
//
//        vector<String> file_names;
//        ifstream meta_file(meta_file_path.c_str());
//        String line;
//        while(getline(meta_file, line)){
//                vector<string> tokens;
//                split(line, ' ', tokens);
//                String pic_name = tokens[0];
//                cout << "File:" << pic_name << endl;
//                Mat img = load_pic(pics_dir, pic_name);
//                for (size_t i = 1; i < tokens.size(); ++i) {
//                        sprintf(number_buffer, "%d", i);
//                        Mat small;
//                        if (cut_pic(img, tokens[i], small) < 0){
//                                continue;
//                        }
//                        write_pic(small, output_dir+'/'+pic_name+String(number_buffer)+".jpg");
//                }
//        }
//        cout << "Mission Completed" << endl;

        vector<string> lines;
        read_label_file(argv[1], lines);
        ratio = 1;
        cut_imgs(argv[2], lines);
}



