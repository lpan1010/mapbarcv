/*
 * objectdfa.cpp
 *
 *  Created on: 2014年12月8日
 *      Author: qin
 */

#include "objectdfa.hpp"

Value* ObjectDFA::eat(stream<char>& foods){
        StringDFA sdfa;
        ValueDFA vdfa;
        char food;

        foods.next(food);
        if (food != '{') {

                foods.back(food);
                return NULL;
        }
        wipe_ass();
        while (foods.next(food)) {
                if (food == ',') {
                        continue;
                } else if (food == '}') {
                        return shit();
                } else {
                        foods.back(food);
                }

                // Parse the key part
                Value* key_value = sdfa.eat(foods);
                if (key_value == NULL){
                        return NULL;
                }
                string key(*(key_value->s));
                key_value->clear();

                foods.next(food);
                if (food != ':'){
                        return NULL;
                }

                // Parse the value part
                Value* value = vdfa.eat(foods);
                if (value == NULL) {
                        return NULL;
                } else {
                        //cout << key << " : " << *value << endl;
                        poo[key] = value;
                }
        }
        return NULL;
}

Value* ObjectDFA::shit(){
        map<string, Value*> *m = new map<string, Value*>();
        m->insert(poo.begin(), poo.end());
        wipe_ass();
        return new Value(m);
}

void ObjectDFA::wipe_ass(){
        poo.clear();
}


