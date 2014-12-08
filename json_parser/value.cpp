/*
 * config.cpp
 *
 *  Created on: 2014年11月25日
 *      Author: qin
 */
#include "value.hpp"

Value Value::nil;

Value::Value() :
                type(NIL), s(NULL), a(NULL), obj(
                NULL) {
        init();
}

Value::Value(int vi) :
                type(INT), i(vi), s(NULL), a(NULL), obj(
                NULL) {
        f = 0.0;
}

Value::Value(bool vb) :
                type(BOOL), b(vb), s(NULL), a(NULL), obj(
                NULL){
}
Value::Value(double vf) :
                type(REAL), f(vf), s(NULL), a(NULL), obj(
                NULL) {
        i = 0;
}

Value::Value(string* vs) :
                type(STRING), s(vs), a(NULL), obj(
                NULL) {
        init();
}

Value::Value(map<string, Value*>* m) :
                type(OBJECT), s(NULL), a(
                NULL), obj(m) {
        init();
}

Value::Value(vector<Value*>* v) :
                type(ARRAY), s(NULL), a(v), obj(
                NULL) {
        init();
}

const Value& Value::operator[](std::size_t idx) {
        if (type == ARRAY) {
                return *((*a)[idx]);
        } else {
                return Value::nil;
        }
}

const Value& Value::operator[](std::string key) {
        if (type == OBJECT) {
                return *((*obj)[key]);
        } else {
                return Value::nil;
        }
}


void Value::init() {
        i = 0;
        f = 0.0;
}

// Dump函数
void Value::dump_object(std::ostream& os) {
        if (obj->size()==0) {os << "{}"; return;}
        
        std::map<std::string, Value*>::iterator iter = obj->begin();
        os << '{' << "\"" << iter->first << "\"" << " : " << *(iter->second);
        iter++;
        for (; iter != obj->end(); iter++) {
                os << ", ";
                os << "\"" << iter->first << "\"" << " : " << *(iter->second);
        }
        os << '}';
}

void Value::dump_array(std::ostream& os) {
        if (a->size() == 0){
                os << "[]";
                return;
        }
        os << '[';
        for (size_t var = 0; var < a->size() -1; ++var) {
                os << *((*a)[var]);
                os << ", ";
        }
        cout << *(a->back()) << ']';
}

void Value::dump_string(std::ostream& os) {
        os << "\"" << *s << "\"";
}

void Value::dump_real(std::ostream& os) {
        os << f;
}

void Value::dump_int(std::ostream& os) {
        os << i;
}

void Value::dump_nil(std::ostream& os) {
        os << "null";
}
void Value::dump_bool(std::ostream& os){
        if (b){
                os << "true";
        }else{
                os << "false";
        }

}

void Value::clear(){
        switch (type) {
                case OBJECT:
                        // TODO
                        break;
                case ARRAY:
                        for (size_t i; i < a->size(); ++i){
                                (*a)[i]->clear();
                        }
                        break;
                case STRING:
                        delete s;
                        break;
                case REAL:
                case INT:
                case NIL:
                case BOOL:
                        break;
        }
        delete this;
}

std::ostream& operator<<(std::ostream& os, Value& v) {
        switch (v.type) {
                case OBJECT:
                        v.dump_object(os);
                        break;
                case ARRAY:
                        v.dump_array(os);
                        break;
                case STRING:
                        v.dump_string(os);
                        break;
                case REAL:
                        v.dump_real(os);
                        break;
                case INT:
                        v.dump_int(os);
                        break;
                case NIL:
                        v.dump_nil(os);
                        break;
                case BOOL:
                        v.dump_bool(os);
                        break;
        }
        return os;
}

/*int main(int argc, char *argv[]) {
        string kk = "abcedfghijikl";
        map<string, Value> m;
        m["a"] = Value::nil;
        m["b"] = Value::nil;
        m["c"] = Value(123.120);
        m["d"] = Value::nil;
        m["d"] = Value(1);
        m["k"] = Value(&kk);
        Value obj(&m);

        std::cout << obj << std::endl;
        Value v(&kk);
        return 0;
}*/
