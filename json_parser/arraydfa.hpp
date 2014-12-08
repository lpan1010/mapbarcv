/*
 * arraydfa.h
 *
 *  Created on: 2014年12月5日
 *      Author: qin
 */

#ifndef ARRAYDFA_H_
#define ARRAYDFA_H_

#include "dfa.hpp"
#include "valuedfa.hpp"

class ArrayDFA: public DFA<char> {
        public:
                 Value* eat(stream<char>& foods, char& appetizer);
                 Value* shit();
                 void wipe_ass();
        private:
                 vector<Value*>* poo;
};


#endif /* ARRAYDFA_H_ */
