//
// Created by Admin on 25.12.2021.
//

#ifndef ML_COURSEWORK_UTILS_H
#define ML_COURSEWORK_UTILS_H

#include "Direction.h"
#include <vector>
#include <string>

Direction stringToDirection(std::string dir);

std::string directionToString(Direction direction);

void failed_to_open(std::string cond);

void goToCond(std::ifstream& fin);


#endif //ML_COURSEWORK_UTILS_H
