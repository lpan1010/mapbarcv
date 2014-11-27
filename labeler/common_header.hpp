/*
 * common_header.hpp
 *
 *  Created on: 2014年11月27日
 *      Author: qin
 */

#ifndef COMMON_HEADER_HPP_
#define COMMON_HEADER_HPP_


#include "global.hpp"

enum PinStatus {
        Locked, First, Second
};
enum KeyAction {
        SaveAndNext,
        CancelCurrentLabel,
        Exit,
        NoOp,
        DeletePreviousLabel,
        DeleteAllLabels
};

#endif /* COMMON_HEADER_HPP_ */
