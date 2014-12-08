/*
 * dfa.h
 *
 *  Created on: 2014年12月5日
 *      Author: qin
 */

#ifndef DFA_H_
#define DFA_H_

#include "value.hpp"
using namespace std;

template<typename T> class stream {
        public:
                virtual bool next(T&) = 0;
                virtual void back(T&) = 0;
                //virtual ~stream() = 0;
};


template<typename T> class DFA {
        public:
                virtual Value* eat(stream<T>& foods) = 0;

                bool in_range(const T& a, const T& min, const T& max) {
                        return (a >= min && a < max);
                }

        protected:
                virtual Value* shit() = 0;
                virtual void wipe_ass() = 0;



};

#endif /* DFA_H_ */
