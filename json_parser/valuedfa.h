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

#include "dfa.h"
#include "stringdfa.h"
#include "intdfa.h"
#include "arraydfa.h"

class ValueDFA: public DFA<char> {
        public:
                Value* eat(stream<char>& foods);
                Value* shit();
                void wipe_ass();
};

#endif /* VALUEDFA_H_ */
