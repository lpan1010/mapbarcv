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

Value* ValueDFA::eat(stream<char>& foods, char& appetizer) {
        if (appetizer == '\"') {
                return sdfa->eat(foods, appetizer);
        } else if (in_range(appetizer, '0', '9' + 1) || appetizer == '-') {
                return idfa->eat(foods, appetizer);
        } else if (appetizer == '{') {
                return odfa->eat(foods, appetizer);
        } else if (appetizer == '[') {
                return adfa->eat(foods, appetizer);
        } else if (appetizer == 'f' || appetizer == 't' || appetizer == 'T' || appetizer == 'F') {
                return bdfa->eat(foods, appetizer);
        } else if (appetizer == 'n' || appetizer == 'N') {
                return ndfa->eat(foods, appetizer);
        } else {
                cout << "Value:: can't handle "<< appetizer << endl;
        }
        return NULL;
}

void ValueDFA::wipe_ass() {
}

Value* ValueDFA::shit() {
        return &Value::nil;
}

