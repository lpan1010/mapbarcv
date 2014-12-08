/*
 * stringdfa.h
 *
 *  Created on: 2014年12月5日
 *      Author: qin
 */

#ifndef STRINGDFA_H_
#define STRINGDFA_H_

#include "dfa.hpp"

class StringDFA: public DFA<char> {
        public:
                StringDFA();
                Value* eat(stream<char>& foods);
        protected:
                Value* shit();
                void wipe_ass();
                bool insert_control_char(const char& c);

                map<char, char> control_chars;
                string poo;
};

#endif /* STRINGDFA_H_ */
