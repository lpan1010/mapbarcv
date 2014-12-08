/*
 * arraydfa.cpp
 *
 *  Created on: 2014年12月5日
 *      Author: qin
 */

#include "arraydfa.hpp"

Value* ArrayDFA::eat(stream<char>& foods) {
        poo = new vector<Value*>();

        ValueDFA vdfa;
        char food;

        foods.next(food);
        if (food != '[') {
                foods.back(food);
                return NULL;
        }
        while (foods.next(food)) {
                if (food == ',') {
                        continue;
                } else if (food == ']') {
                        return shit();
                } else {
                        foods.back(food);
                }
                Value* v = vdfa.eat(foods);
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

