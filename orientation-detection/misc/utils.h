//
// Created by reid on 2/27/18.
//

#ifndef ORIENTATION_DETECTION_UTILS_H
#define ORIENTATION_DETECTION_UTILS_H

#include <sstream>
#include <string>

inline std::string intToString(int number){
    std::stringstream ss;
    ss << number;
    return ss.str();
}

#endif //ORIENTATION_DETECTION_UTILS_H
