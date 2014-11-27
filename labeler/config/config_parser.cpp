/*
 * config_parser.cpp
 *
 *  Created on: 2014年11月25日
 *      Author: qin
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
                virtual bool eat(stream<T>& foods) = 0;
                virtual Value shit() = 0;
                virtual void reset() = 0;

        protected:
                bool in_range(const T& a, const T& min, const T& max) {
                        return (a >= min && a < max);
                }

};

/**
 * Done. Have passed the naive test.
 */
class IntDFA: public DFA<char> {
        public:
                bool eat(stream<char>& foods) {
                        char food;
                        reset();

                        foods.next(food);
                        if (food != '-' && !in_range(food, '0', '9' + 1)) {
                                foods.back(food);
                                return false;
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
                        return true;
                }

                Value shit() {
                        Value ret(atoi(poo.c_str()));
                        return ret;
                }

                void reset() {
                        poo.clear();
                }

        private:
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

                bool eat(stream<char>& foods) {
                        char food;
                        bool in_control_char = false;
                        reset();

                        foods.next(food);
                        if (food != '\"') {
                                foods.back(food);
                                return false;
                        }

                        while (foods.next(food)) {
                                if (!in_control_char && food == '\\') {
                                        in_control_char = true;
                                        continue;
                                }
                                if (!in_control_char && food == '\"') {
                                        return true;
                                        //break;
                                }
                                if (in_control_char) {
                                        in_control_char = false;
                                        if (!insert_control_char(food)) {
                                                return false;
                                        }
                                } else {
                                        poo.push_back(food);
                                }
                        }
                        return false;
                }

                Value shit() {
                        string* s = new string();

                        Value ret(s);
                        return ret;
                }

                void reset() {
                        poo.clear();
                }

        private:
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
                bool eat(stream<char>& foods) {
                        char food;
                        foods.next(food);
                        if (food == '\"') {
                                foods.back(food);
                                // String
                        } else if (in_range(food, '0', '9' + 1)
                                        || food == '-') {
                                foods.back(food);
                                // Int
                        } else if (food == '{') {
                                foods.back(food);
                                // Object
                        } else if (food == '[') {
                                foods.back(food);
                                // Array
                        } else if (food == 'f' || food == 'true') {
                                foods.back(food);
                                // bool
                        } else if (food == 'n') {
                                foods.back(food);
                                // null
                        }
                }
                void reset() {
                }

                Value Shit(){
                        return Value::nil;
                }
};

class ObjectDFA: public DFA<char> {

};
class ArrayDFA: public DFA<char> {
        public:
                bool eat(stream<char>& foods) {
                        ValueDFA vdfa;
                        char food;
                        reset();

                        foods.next(food);
                        if (food != '[') {
                                foods.back(food);
                                return false;
                        }
                        while (foods.next(food)) {
                                // TODO value dfa
                                if (vdfa.eat(foods)){

                                }
                                if (food == ','){
                                        continue;
                                }else if(food == ']'){
                                        return false;
                                }else {
                                        // TODO handle error
                                }
                        }
                        return false;
                }

                Value shit() {
                        //TODO err
                        return Value(&poo);
                }

                void reset() {
                        poo.clear();
                }
        private:
                string poo;

};

/*int main(int argc, char **argv) {
        strstream ssi("199999");
        IntDFA id;
        StringDFA sd;
        Value v;
        if (id.eat(ssi)) {
                v = id.shit();
                cout << v << endl;
        }

        strstream sss("\"abckdf\nkk\"");

        if (sd.eat(sss)) {
                v = sd.shit();
                cout << v << endl;
        }
}*/
