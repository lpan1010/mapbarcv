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

#include "value.hpp"

using namespace std;

class Parser {
        public:
                Value& parse(istream& file);
        private:
};

template<typename T> class stream {
        public:
                virtual bool next(T&) = 0;
                virtual void back(T&) = 0;
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

template<typename T> class DFA {
        public:
                //virtual bool eat(stream<T>& foods) = 0;
                virtual Value& eat(stream<T>& foods) = 0;

        protected:
                virtual Value& shit() = 0;
                virtual void wipe_ass() = 0;

                bool in_range(const T& a, const T& min, const T& max) {
                        return (a >= min && a < max);
                }

};

/**
 * Done. Have passed the naive test.
 */
class IntDFA: public DFA<char> {
        public:
                Value* eat(stream<char>& foods) {
                        char food;
                        foods.next(food);

                        if (food != '-' && !in_range(food, '0', '9' + 1)) {
                                foods.back(food);
                                return NULL;
                        } else {
                                poo.push_back(food);
                        }
                        while (foods.next(food)) {
                                if (in_range(food, '0', '9' + 1)) {
                                        poo.push_back(food);
                                } else {
                                        foods.back(food);
                                        break;
                                }
                        }
                        return shit();
                }

        protected:
                void wipe_ass() {
                        poo.clear();
                }

                Value* shit() {
                        Value* ret = new Value(atoi(poo.c_str()));
                        wipe_ass();
                        return ret;
                }
                string poo;
};

class StringDFA: public DFA<char> {
        public:
                StringDFA() {
                        control_chars['\"'] = '\"';
                        control_chars['\\'] = '\\';
                        control_chars['/'] = '/';
                        control_chars['b'] = '\b';
                        control_chars['f'] = '\f';
                        control_chars['n'] = '\n';
                        control_chars['r'] = '\r';
                        control_chars['t'] = '\t';
                        // TODO hex number is not supported yet.
                        //control_chars['u'] = '\f';
                }

                Value* eat(stream<char>& foods) {
                        char food;
                        bool in_control_char = false;

                        foods.next(food);

                        // Just in case....
                        if (food != '\"') {
                                foods.back(food);
                                return NULL;
                        }

                        while (foods.next(food)) {
                                if (!in_control_char && food == '\\') {
                                        in_control_char = true;
                                        continue;
                                }
                                if (!in_control_char && food == '\"') {
                                        return shit();
                                        //break;
                                }
                                if (in_control_char) {
                                        in_control_char = false;
                                        if (!insert_control_char(food)) {
                                                break;
                                        }
                                } else {
                                        poo.push_back(food);
                                }
                        }
                        return NULL;
                }



        protected:
                Value* shit() {
                        Value* ret = new Value(new string(poo));
                        wipe_ass();
                        return ret;
                }

                void wipe_ass() {
                        poo.clear();
                }

                bool insert_control_char(const char& c) {
                        map<char, char>::iterator it = control_chars.find(c);
                        if (it == control_chars.end()) {
                                return false;
                        } else {
                                poo.push_back(it->second);
                                return true;
                        }
                }

                map<char, char> control_chars;
                string poo;
};

class ValueDFA: public DFA<char> {
        public:
                Value* eat(stream<char>& foods) {
                        char food;
                        bool ret;
                        foods.next(food);
                        if (food == '\"') {
                                foods.back(food);
                                // String
                        } else if (in_range(food, '0', '9' + 1)
                                        || food == '-') {
                                // Int
                                foods.back(food);
                                IntDFA idfa;
                                return idfa.eat(foods);
                        } else if (food == '{') {
                                foods.back(food);
                                // Object
                        } else if (food == '[') {
                                foods.back(food);
                                // Array
                        } else if (food == 'f' || food == 't') {
                                foods.back(food);
                                // bool
                        } else if (food == 'n') {
                                foods.back(food);
                                // null
                        }else{
                                // Error
                        }
                }


                void wipe_ass() {
                }

                Value* shit(){
                        return &Value::nil;
                }
};

class ObjectDFA: public DFA<char> {

};
class ArrayDFA: public DFA<char> {
        public:
                Value* eat(stream<char>& foods) {
                        ValueDFA vdfa;
                        char food;
                        wipe_ass();

                        foods.next(food);
                        if (food != '[') {
                                foods.back(food);
                                return false;
                        }
                        while (foods.next(food)) {
                                // TODO value dfa
                                Value* v = vdfa.eat(foods);
                                if (v == NULL){
                                        return NULL;
                                }else{

                                }
                                if (food == ','){
                                        continue;
                                }else if(food == ']'){
                                        // TODO we are done?
                                        return NULL;
                                }else {
                                        // TODO handle error
                                }
                        }
                        return false;
                }

        private:
                Value* shit() {
                        //TODO err
                        return new Value(poo);
                }

                void wipe_ass() {
                        poo = NULL;
                        //poo.clear();
                }

                vector<Value>* poo;

};

int main(int argc, char **argv) {
        strstream ssi("199999");
        IntDFA id;
        StringDFA sd;
        Value* v;
        v = id.eat(ssi);
        if (v != NULL){
                cout << *v << endl;
        }
        delete v;
        strstream sss("\"abckdf\nkk\"");
        v = sd.eat(sss);
        if (v != NULL){
                cout << *v << endl;
        }
}
