/*
 * config_parser.cpp
 *
 *  Created on: 2014年11月25日
 *      Author: qin
 *
 *      TODO 现在的问题是如何解决Array DFA一类的解析问题
 *      TODO 未来是否需要转换为不可变对象？
 *      String done Int done Double Object Array bool nil Value
 */

#include <vector>
#include <map>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "dfa.hpp"
#include "value.hpp"
#include "valuedfa.hpp"
#include "stringdfa.hpp"
#include "number_dfa.hpp"

using namespace std;

class ArrayDFA;
class ValueDFA;

class Parser {
        public:
                Value& parse(istream& file);
        private:
};

class strstream: public stream<char> {
        public:
                strstream(string in) {
                        s = in;
                        cur = in.size();
                        j_empty = true;
                }

                bool next(char& n) {
                        while (true) {
                                if (s.empty()) {
                                        return false;
                                }
                                n = s[0];
                                s.erase(0, 1);
                                if (j_empty && is_empty_char(n)){
                                        continue;
                                }
                                //cout << n;
                                return true;
                        }
                }

                void back(char& b) {
                        s.insert(0, 1, b);
                }

        private:
                bool is_empty_char(char& c) {
                        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
                                return true;
                        } else {
                                return false;
                        }
                }

                size_t cur;
                string s;
};

int main(int argc, char **argv) {
        ObjectDFA od;
        string s;
        ifstream ifs;
        ifs.open("/tmp/json.json");
        string line;
        while(std::getline(ifs, line)){
                s += line;
        }
        strstream ssa(s);
        Value* a = od.eat(ssa);
        cout << *a << endl;
        cout << "DONE" << endl;
}
