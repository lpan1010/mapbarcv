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

#include "dfa.h"
#include "value.hpp"
#include "valuedfa.h"
#include "stringdfa.h"
#include "intdfa.h"

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
                }

                bool next(char& n) {
                        if (s.empty()) {
                                return false;
                        }
                        n = s[0];
                        s.erase(0, 1);
                        return true;
                }

                void back(char& b) {
                        s.insert(0, 1, b);
                }

        private:
                size_t cur;
                string s;
};

class ObjectDFA: public DFA<char> {

};


int main(int argc, char **argv) {
        strstream ssi("199999");
        IntDFA id;
        StringDFA sd;
        ArrayDFA ad;
//        Value* v;
//        v = id.eat(ssi);
//        if (v != NULL){
//                cout << *v << endl;
//        }
//        delete v;
//
//        strstream sss("\"abckdf\nkk\"");
//        v = sd.eat(sss);
//        if (v != NULL){
//                //cout << *(v->s)<<endl;
//                cout << *v << endl;
//        }
//
//        Value i(123);
//        string si = "asdf";
//        Value s(&si);
//        vector<Value*> vec;
//        vec.push_back(&s);
//        vec.push_back(&i);
//
//        string st = "fofo";
//        s.s = &st;
//        i.i = 321;
//        cout << *vec[0] << endl;
//        cout << *vec[1] << endl;
        cout << "Array:" << endl;
        strstream ssa("[\"123\",123]");

        Value* a =ad.eat(ssa);
        cout << *a << endl;
        cout << "DONE" << endl;
}
