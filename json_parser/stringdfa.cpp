/*
 * stringdfa.cpp
 *
 *  Created on: 2014年12月5日
 *      Author: qin
 */

#include "stringdfa.hpp"

StringDFA::StringDFA() {
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

Value* StringDFA::eat(stream<char>& foods) {
        char food;
        bool in_control_char = false;

        foods.next(food);

        // TODO Just in case....
        if (food != '\"') {
                foods.back(food);
                return NULL;
        }

        foods.dont_jump_empty();
        while (foods.next(food)) {
                if (!in_control_char && food == '\\') {
                        in_control_char = true;
                        continue;
                }
                if (!in_control_char && food == '\"') {
                        // The success exit.
                        foods.jump_empty();
                        return shit();
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
        foods.jump_empty();

        // The error / can't handle / foods empty exit.
        return NULL;
}

Value* StringDFA::shit() {
        Value* ret = new Value(new string(poo));
        wipe_ass();
        return ret;
}

void StringDFA::wipe_ass() {
        poo.clear();
}

bool StringDFA::insert_control_char(const char& c) {
        map<char, char>::iterator it = control_chars.find(c);
        if (it == control_chars.end()) {
                return false;
        } else {
                poo.push_back(it->second);
                return true;
        }
}
