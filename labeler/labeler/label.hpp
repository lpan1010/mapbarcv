/*
 * label.hpp
 *
 *  Created on: 2014,11,27
 *      Author: qin
 */

#ifndef LABEL_HPP_
#define LABEL_HPP_

#include "../common_header.hpp"
#include "../os.hpp"

class Label {
        private:
                bool is_first_pin;
                bool locked;
        public:

                CvPoint left_top_point;
                CvPoint right_bottom_point;
                Label() :
                                is_first_pin(true), locked(false) {
                }

                void print_on(Mat img) {
                        if (locked) {
                                rectangle(img, left_top_point,
                                                right_bottom_point,
                                                Scalar(0, 0, 255));
                        } else if(!is_first_pin){
                                circle(img, left_top_point, 2,
                                                Scalar(0, 0, 255));
                        }
                }

                QString to_string() {
                        StringStream ss;
                        int height = right_bottom_point.y - left_top_point.y;
                        int width = right_bottom_point.x - left_top_point.x;
                        ss << left_top_point.x << "," << left_top_point.y << ","
                                        << width << "," << height;
                        return ss.str();
                }

                PinStatus pin(int x, int y) {
                        // Two points are both set.
                        if (locked) {
                                return Locked;
                        }

                        if (is_first_pin) {
                                left_top_point.x = x;
                                left_top_point.y = y;
                                is_first_pin = false;
                                return First;
                        } else {
                                right_bottom_point.x = x;
                                right_bottom_point.y = y;
                                locked = true;
                                return Second;
                        }
                }
                //friend ostream& operator<<(ostream& os, const Label& dt);
};



#endif /* LABEL_HPP_ */
