/*
 * nildfa.cpp
 *
 *  Created on: 2014年12月8日
 *      Author: qin
 */

#include "nildfa.hpp"
#include "algorithm"

Value* NilDFA::eat(stream<char>& foods){
        char food;
        string poo;

        if (!foods.next(food)){
                return NULL;
        }

        if (food != 'n' && food != 'N'){
                return NULL;
        }

        for (int var = 0; var < 3; ++var) {
                if (!foods.next(food)){
                     return NULL;
                }
                poo.push_back(food);
        }
        std::transform(poo.begin(), poo.end(), poo.begin(), ::tolower);
        if (poo == "ull"){
                return new Value();
        }else{
                return NULL;
        }
}

