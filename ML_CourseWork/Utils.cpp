//
// Created by Admin on 25.12.2021.
//
#include "Utils.h"
#include <iostream>
#include <vector>
#include <fstream>

Direction stringToDirection(std::string dir){
    if(dir == "UP"){
        return Direction::UP;

    } else if (dir == "DOWN") {
        return Direction::DOWN;

    } else if (dir == "LEFT") {
        return Direction::LEFT;

    } else if (dir == "RIGHT") {
        return Direction::RIGHT;

    } else if (dir == "UP_LEFT") {
        return Direction::UP_LEFT;

    } else if (dir == "UP_RIGHT") {
        return Direction::UP_RIGHT;

    } else if (dir == "DOWN_LEFT") {
        return Direction::DOWN_LEFT;

    } else if (dir == "DOWN_RIGHT") {
        return Direction::DOWN_RIGHT;

    } else {
        return Direction::NONE;
    }
}

std::string directionToString(Direction direction){
    switch (direction) {
        case Direction::RIGHT:
            return "RIGHT";
        case Direction::LEFT:
            return "LEFT";
        case Direction::UP:
            return "UP";
        case Direction::DOWN:
            return "DOWN";
        case Direction::UP_RIGHT:
            return "UP_RIGHT";
        case Direction::UP_LEFT:
            return "UP_LEFT";
        case Direction::DOWN_RIGHT:
            return "DOWN_RIGHT";
        case Direction::DOWN_LEFT:
            return "DOWN_LEFT";
    }
}

void failed_to_open(std::string cond) {
    std::cout << "cant open file with " + cond + ". Continue? [Yes/No]";
    std::string tmp;
    while (true) {
        std::cin >> tmp;
        if (tmp == "Yes") {
            break;
        } else if (tmp == "No") {
            exit(1);
        } else {
            std::cout << "Cant recognize input. Please enter one of this: [Yes/No]";
        }
    }
}

void goToCond(std::ifstream& fin){
    char current = '0';
    char last = '0';
    while(current != '#' || last == '<'){
        last = current;
        fin >> current;

        if(fin.eof()){
            std::cerr << "Can't find parameters for condition. Terminated.";
            exit(1);
        }
    }
}


