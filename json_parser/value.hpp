/*
 * value.hpp
 *
 *  Created on: 2014年11月25日
 *      Author: qin
 */

#ifndef VALUE_HPP_
#define VALUE_HPP_

#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

enum ValueType {
        INT, REAL, STRING, ARRAY, OBJECT, NIL, BOOL
};

class Value {
        public:
                static Value nil;

                Value();
                Value(int);
                Value(double);
                Value(string*);
                Value(map<string, Value*>* m);
                Value(vector<Value* >* v);
                Value(bool vb);
                const Value& operator[](std::size_t idx);
                const Value& operator[](std::string key);
                friend std::ostream& operator<<(std::ostream&, Value&);
                void clear();
        public:
                // 变量
                ValueType type;
                int i;
                double f;
                bool b;
                string* s;
                vector<Value*>* a;
                map<string, Value*>* obj;

                void dump_object(std::ostream& os);
                void dump_array(std::ostream& os);
                void dump_string(std::ostream& os);
                void dump_real(std::ostream& os);
                void dump_int(std::ostream& os);
                void dump_nil(std::ostream& os);
                void dump_bool(std::ostream& os);
                void init();
};

#endif /* VALUE_HPP_ */
