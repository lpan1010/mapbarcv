/*
 * valuedfa.h
 *
 *  Created on: 2014年12月5日
 *      Author: qin
 */

#ifndef VALUEDFA_H_
#define VALUEDFA_H_

#include <vector>
#include <map>
#include <stdlib.h>

#include "dfa.hpp"
//#include "objectdfa.hpp"
class NumberDFA;
class ArrayDFA;
class StringDFA;
class ObjectDFA;
class BoolDFA;
class NilDFA;

class ValueDFA: public DFA<char> {
public:
        ValueDFA();
        ~ValueDFA();
        Value* eat(stream<char>& foods);
        Value* shit();
        void wipe_ass();
private:
        NumberDFA* idfa;
        StringDFA* sdfa;
        ObjectDFA* odfa;
        ArrayDFA* adfa;
        BoolDFA* bdfa;
        NilDFA* ndfa;
};

#endif /* VALUEDFA_H_ */
