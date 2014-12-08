/*
 * arraydfa.cpp
 *
 *  Created on: 2014年12月5日
 *      Author: qin
 */

#include "arraydfa.hpp"

Value* ArrayDFA::eat(stream<char>& foods, char& appetizer) {
        if (appetizer != '[') {
                foods.back(appetizer);
                return NULL;
        }
        
        // 潜在的内存泄漏问题
        poo = new vector<Value*>();
        ValueDFA vdfa;
        
        char food;
        while (foods.next(food)) {
                if (food == ',') {
                        continue;
                } else if (food == ']') {
                        return shit();
                }
                Value* v = vdfa.eat(foods, food);
                if (v == NULL) {
                        return NULL;
                } else {
                        poo->push_back(v);
                }
        }
        return NULL;
}

Value* ArrayDFA::shit() {
        Value* ret = new Value(poo);
        wipe_ass();
        return ret;
}

void ArrayDFA::wipe_ass() {
        poo = NULL;
}

