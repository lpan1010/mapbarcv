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
#include "stringdfa.hpp"
#include "intdfa.hpp"
#include "arraydfa.hpp"
#include "booldfa.hpp"
#include "nildfa.hpp"
#include "objectdfa.hpp"

class ValueDFA: public DFA<char> {
        public:
                Value* eat(stream<char>& foods);
                Value* shit();
                void wipe_ass();
};

#endif /* VALUEDFA_H_ */
