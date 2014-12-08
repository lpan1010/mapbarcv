/*
 * nildfa.h
 *
 *  Created on: 2014年12月8日
 *      Author: qin
 */

#ifndef NILDFA_H_
#define NILDFA_H_

#include "dfa.hpp"
class NilDFA :public DFA<char>{
        public:
                Value* eat(stream<char>& foods, char& appetizer);
        private:
                Value* shit(){return NULL;};
                void wipe_ass(){};
};

#endif /* NILDFA_H_ */
