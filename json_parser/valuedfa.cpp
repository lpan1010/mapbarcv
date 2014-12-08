/*
 * valuedfa.cpp
 *
 *  Created on: 2014年12月5日
 *      Author: qin
 */

#include "valuedfa.h"

class IntDFA;
class ArrayDFA;
class StringDFA;

Value* ValueDFA::eat(stream<char>& foods) {
        char food;
        foods.next(food);
        if (food == '\"') {
                foods.back(food);
                StringDFA sdfa;
                return sdfa.eat(foods);
        } else if (in_range(food, '0', '9' + 1) || food == '-') {
                foods.back(food);
                IntDFA idfa;
                return idfa.eat(foods);
        } else if (food == '{') {
                foods.back(food);
                // TODO Object
        } else if (food == '[') {
                foods.back(food);
                ArrayDFA adfa;
                return adfa.eat(foods);
        } else if (food == 'f' || food == 't') {
                foods.back(food);
                // TODO bool
        } else if (food == 'n') {
                foods.back(food);
                // TODO null
        } else {
                // Error
        }
        return NULL;
}

void ValueDFA::wipe_ass() {
}

Value* ValueDFA::shit() {
        return &Value::nil;
}

