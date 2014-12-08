/*
 * valuedfa.cpp
 *
 *  Created on: 2014年12月5日
 *      Author: qin
 */

#include "valuedfa.hpp"

#include "stringdfa.hpp"
#include "number_dfa.hpp"
#include "arraydfa.hpp"
#include "booldfa.hpp"
#include "nildfa.hpp"
#include "objectdfa.hpp"

ValueDFA::ValueDFA():idfa(new NumberDFA()), sdfa(new StringDFA()), odfa(new ObjectDFA()), adfa(new ArrayDFA()), bdfa(new BoolDFA()), ndfa(new NilDFA()){
}

ValueDFA::~ValueDFA(){
        delete idfa;
        delete sdfa;
        delete odfa;
        delete adfa;
        delete bdfa;
        delete ndfa;
}

Value* ValueDFA::eat(stream<char>& foods) {
        char food;
        foods.next(food);

        if (food == '\"') {
                foods.back(food);
                return sdfa->eat(foods);
        } else if (in_range(food, '0', '9' + 1) || food == '-') {
                foods.back(food);
                return idfa->eat(foods);
        } else if (food == '{') {
                foods.back(food);
                return odfa->eat(foods);
        } else if (food == '[') {
                foods.back(food);
                return adfa->eat(foods);
        } else if (food == 'f' || food == 't' || food == 'T' || food == 'F') {
                foods.back(food);
                return bdfa->eat(foods);
        } else if (food == 'n' || food == 'N') {
                foods.back(food);
                return ndfa->eat(foods);
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

