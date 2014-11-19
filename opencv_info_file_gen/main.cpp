/*
 * main.cpp
 *
 *  Created on: 2014年11月4日
 *      Author: qin
 */
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>

using namespace std;

void split(const std::string &s,
                char delim,
                vector<string> &elems) {
        stringstream ss(s);
        string item;
        while (getline(ss, item, delim)) {
            elems.push_back(item);
        }
}

void replace(string& s, char i, char o){
        replace(s.begin(), s.end(), i, o);
}

int main(int argc, char **argv) {
        string intput_file(argv[1]);
        string output_file(argv[2]);
        ifstream intput;
        intput.open(intput_file.c_str(), ios::in);
        ofstream output;
        output.open(output_file.c_str(), ios::out);
        string line;
        while(getline(intput, line)){
               vector<string> items;
               split(line, ' ', items);
               int l_num = items.size() -1;
               stringstream s;
               s << items[0];
               s << "  ";
               s << l_num;
               for (size_t i = 1; i < items.size(); ++i){
                       replace(items[i], ',', ' ');
                       s << "  ";
                       s << items[i];
               }
               s << '\n';
               getline(s, line);
               cout << line << endl;
               output << line << endl;
        }
        output.close();
        cout << (int)(123 > 0) << endl;
}





