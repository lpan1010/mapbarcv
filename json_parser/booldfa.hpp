/*
 * booldfa.h
 *
 *  Created on: 2014年12月8日
 *      Author: qin
 */

#ifndef BOOLDFA_H_
#define BOOLDFA_H_

#include "dfa.hpp"

class BoolDFA : public DFA<char>{
        public:
                Value* eat(stream<char>& foods, char& appetizer);
                Value* shit();
                void wipe_ass();
        private:
                bool n_bites(stream<char>& foods, int n);
                string buffer;
                bool t_or_f;
};

#endif /* BOOLDFA_H_ */
