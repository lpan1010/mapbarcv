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

class NumberDFA: public DFA<char> {
public:
        NumberDFA(){
                is_float = false;
        }

        Value* eat(stream<char>& foods) {
                //TODO We need to implement the real type dfa.
                char food;
                foods.next(food);

                if (food != '-' && food != '+' && !in_range(food, '0', '9' + 1)) {
                        foods.back(food);
                        return NULL;
                } else {
                        poo.push_back(food);
                }
                while (foods.next(food)) {
                        if (food == '.' || food == 'e' || food == 'E'){
                                is_float = true;
                                poo.push_back(food);
                                continue;
                        }
                        if (in_range(food, '0', '9' + 1)
                            || food == '+'
                            || food == '-'
                            ) {
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
                is_float = false;
                poo.clear();
        }

        Value* shit() {
                Value* ret;
                if(is_float){
                        ret = new Value(atof(poo.c_str()));
                }else{
                        ret = new Value(atoi(poo.c_str()));
                }
                wipe_ass();
                return ret;
        }
        string poo;
        bool is_float;
};

#endif /* INTDFA_H_ */
