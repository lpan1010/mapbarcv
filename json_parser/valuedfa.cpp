/*
 * valuedfa.cpp
 *
 *  Created on: 2014年12月5日
 *      Author: qin
 */

#include "valuedfa.hpp"

class NumberDFA;
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
                NumberDFA idfa;
                return idfa.eat(foods);
        } else if (food == '{') {
                foods.back(food);
                ObjectDFA odfa;
                return odfa.eat(foods);
        } else if (food == '[') {
                foods.back(food);
                ArrayDFA adfa;
                return adfa.eat(foods);
        } else if (food == 'f' || food == 't' || food == 'T' || food == 'F') {
                foods.back(food);
                BoolDFA bdfa;
                return bdfa.eat(foods);
        } else if (food == 'n' || food == 'N') {
                foods.back(food);
                NilDFA ndfa;
                return ndfa.eat(foods);
        } else {
                cout << "Value:: can't handle "<< food << endl;
        }
        return NULL;
}

void ValueDFA::wipe_ass() {
}

Value* ValueDFA::shit() {
        return &Value::nil;
}

