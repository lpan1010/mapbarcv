/*
 * stringdfa.h
 *
 *  Created on: 2014年12月5日
 *      Author: qin
 */

#ifndef STRINGDFA_H_
#define STRINGDFA_H_

#include "dfa.h"

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

#endif /* STRINGDFA_H_ */
