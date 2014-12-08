/*
 * arraydfa.h
 *
 *  Created on: 2014年12月5日
 *      Author: qin
 */

#ifndef ARRAYDFA_H_
#define ARRAYDFA_H_

#include "dfa.h"
#include "valuedfa.h"

class ArrayDFA: public DFA<char> {
        public:
                 Value* eat(stream<char>& foods);
                 Value* shit();
                 void wipe_ass();
        private:
                 vector<Value*>* poo;
};


#endif /* ARRAYDFA_H_ */
