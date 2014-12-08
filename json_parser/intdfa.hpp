/*
 * intdfa.h
 *
 *  Created on: 2014年12月5日
 *      Author: qin
 */

#ifndef INTDFA_H_
#define INTDFA_H_

#include "dfa.hpp"
#include "stdlib.h"

class IntDFA: public DFA<char> {
        public:
                Value* eat(stream<char>& foods) {
                        char food;
                        foods.next(food);

                        if (food != '-' && !in_range(food, '0', '9' + 1)) {
                                foods.back(food);
                                return NULL;
                        } else {
                                poo.push_back(food);
                        }
                        while (foods.next(food)) {
                                if (in_range(food, '0', '9' + 1)) {
                                        poo.push_back(food);
                                } else {
                                        foods.back(food);
                                        break;
                                }
                        }
                        return shit();
                }

        protected:
                void wipe_ass() {
                        poo.clear();
                }

                Value* shit() {
                        Value* ret = new Value(atoi(poo.c_str()));
                        wipe_ass();
                        return ret;
                }
                string poo;
};

#endif /* INTDFA_H_ */
