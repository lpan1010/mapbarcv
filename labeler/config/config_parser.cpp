/*
 * config_parser.cpp
 *
 *  Created on: 2014年11月25日
 *      Author: qin
 */

#include <vector>
#include <map>

#include "value.hpp"

using namespace std;

class Parser {
        public:
                Value& parse(istream& file);
        private:
};

template<typename T> class stream {
        public:
                bool next(T&);
                void back(T);
};


template<typename T, typename State> class DFA {
        public:
                DFA(const map<State, map<T, State> > &jtable) :
                                jump_table(jtable) {
                }

                bool eat(stream<T>& foods) {
                        T t;
                        while (true) {
                                if (!foods.next(t)) {
                                        return false;
                                }
                                typename map<State, map<T, State> >::iterator current_state_jump_table_iter =
                                                jump_table.find(current_state);
                                if (current_state_jump_table_iter
                                                == jump_table.end()) {
                                        return false;
                                } else {
                                        typename map<T, State>::iterator next_state_iter =
                                                        (current_state_jump_table_iter->second).find(
                                                                        t);
                                        if (next_state_iter
                                                        == (next_state_iter->second).end()) {
                                                return false;
                                        }
                                        current_state = next_state_iter->second;
                                        if (current_state.is_end_state()) {
                                                return true;
                                        }
                                }
                        }
                        return false;
                }
        private:
                const map<State, map<T, State> >& jump_table;
                State current_state;
};

