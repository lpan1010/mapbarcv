/*
 * objectdfa.cpp
 *
 *  Created on: 2014年12月8日
 *      Author: qin
 */

#include "objectdfa.hpp"

ObjectDFA::ObjectDFA(){
}

ObjectDFA::~ObjectDFA(){
}

void ObjectDFA::clear(){
        if (poo == NULL) {
                return;
        }
        for (map<string, Value*>::iterator it = poo->begin(); it != poo->end(); ++it) {
                it->second->clear();
        }
        delete poo;
        poo = NULL;
}

Value* ObjectDFA::eat(stream<char>& foods, char& appetizer){
        if (appetizer != '{') {
                foods.back(appetizer);
                return NULL;
        }
        
        StringDFA sdfa;
        ValueDFA vdfa;
        wipe_ass();
        
        poo = new map<string, Value*>();
        char food;
        while (foods.next(food)) {
                if (food == ',') {
                        continue;
                } else if (food == '}') {
                        return shit();
                }
                // Parse the key part
                Value* key_value = sdfa.eat(foods, food);
                if (key_value == NULL){
                        clear();
                        return NULL;
                }
                string key(*(key_value->s));
                key_value->clear();

                foods.next(food);
                if (food != ':'){
                        clear();
                        return NULL;
                }

                // Parse the value part
                if (!foods.next(food)){ clear();return NULL;}
                Value* value = vdfa.eat(foods, food);
                if (value == NULL) {
                        clear();
                        return NULL;
                } else {
                        (*poo)[key] = value;
                }
        }
        clear();
        return NULL;
}

inline Value* ObjectDFA::shit(){
        Value* ret = new Value(poo);
        wipe_ass();
        return ret;
}

inline void ObjectDFA::wipe_ass(){
        poo = NULL;
}


