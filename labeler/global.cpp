/*
 * global.cpp
 *
 *  Created on: 2014,11,27
 *      Author: qin
 */

#include "global.hpp"

#include "labeler/label.hpp"
#include "os.hpp"
#include "labeler/video_labeler.hpp"

// Global Vars
int mouse_x;
int mouse_y;
vector<Label*> * labels;
Label* current_label;

//VideoLabeler vl;

string CONFIG_FILE = "./config.txt";
string PROGRESS_FILE = "./last.txt";
string META_FILE = "./labels.txt";
string PICS_FILE = "/list.txt";
FrameLabeler fl(META_FILE);