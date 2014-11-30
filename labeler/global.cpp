/*
 * global.cpp
 *
 *  Created on: 2014年11月27日
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

VideoLabeler vl;



const string CONFIG_FILE = "./config.conf";
const string PROGRESS_FILE = "./last.conf";
string META_FILE = "./labels.conf";
