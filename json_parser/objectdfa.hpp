/*
 * objectdfa.h
 *
 *  Created on: 2014年12月8日
 *      Author: qin
 */

#ifndef OBJECTDFA_H_
#define OBJECTDFA_H_

#include "dfa.hpp"
#include "stringdfa.hpp"
#include "valuedfa.hpp"


class ObjectDFA: public DFA<char> {
public:
        ObjectDFA();
        ~ObjectDFA();
        Value* eat(stream<char>& foods, char& appetizer);
protected:
        Value* shit();
        void wipe_ass();
        void clear();
        map<string, Value*>* poo;
private:
        
};

#endif /* OBJECTDFA_H_ */
