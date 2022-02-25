//
// Created by Admin on 25.12.2021.
//

#ifndef ML_COURSEWORK_PARAMETERS_H
#define ML_COURSEWORK_PARAMETERS_H

#include "Direction.h"
#include <cmath>
#include <iostream>

#define LOG_J std::ceil(std::log2(J))
#define LOG_N std::ceil(std::log2(N))

#define FLAG_FALSE 0
#define FLAG_TRUE 1
#define NO_FLAG -1

//-------------------- инициализация общих переменных --------------------
const int SIZE = 3; //размер "поля"

const int N = 9; //Кол-во объектов, N <= SIZE*SIZE
const int M = 4; //Кол-во св-в у каждого объекта
const int J = N; //Кол-во значений для каждого свойства J>=N

const int Z = LOG_J; //Кол-во переменных для кодирования значения св-ва
const int T = LOG_J; //Максмальный номер бита в значении св-ва
const int log_N = LOG_N;

//пути к файлам
const std::string CONDITION1_PATH = "../conditions/cond1.txt";
const std::string CONDITION2_PATH = "../conditions/cond2.txt";
const std::string CONDITION3_PATH = "../conditions/cond3.txt";
const std::string CONDITION4_PATH = "../conditions/cond4.txt";
const std::string SOLUTION_OUT_PATH = "../out/main/Solution.txt";
const std::string TABLE_OUT_PATH = "../out/main/Table.txt";
const std::string NODES_OUT_PATH = "../out/main/NodeList.txt";


//----------- инициализация переменных, соответсвующих варианту -----------
//Кол-во ограничений каждого типа
const int NUM_OF_N1 = 11; //7 f10
const int NUM_OF_N2 = 6; //4 f5
const int NUM_OF_N3 = 6; //4
const int NUM_OF_N4 = 5; //5

//соесдские отношения (какая клетка относительно текущей считается ее соседом)
static const Direction directions[] = {
        Direction::UP,
        Direction::LEFT
};

//Вариант склейки
//Если вариант склейки указан по строке - значение по столбцу задать -1, и наоборот
//Значение должно быть в промежутке [1; SIZE]
const int COLUMN_GLU = -1;
const int ROW_GLU = 2;

//проверка корректности входных параметров
void param_check(){
    bool overN = N > SIZE * SIZE;
    bool bigGlue = ROW_GLU > SIZE || COLUMN_GLU > SIZE;
    bool overGlu = ROW_GLU > 0 && COLUMN_GLU > 0;
    bool noGlu = ROW_GLU <= 0 && COLUMN_GLU <= 0;
    bool lilJ = J < N;
    if(overN){
        std::cout << "Кол-во объектов превышает размер поля";
        exit(1);
    }
    if(overGlu){
        std::cout << "Выберите только один вариант склейки";
        exit(1);
    }
    if(noGlu){
        std::cout << "Не выбрана склейка";
        exit(1);
    }
    if(bigGlue){
        std::cout << "Строка или столбец выходит за пределы";
        exit(1);
    }
    if(lilJ){
        std::cout << "Число значений свойств меньше кол-ва объектов, противоречит 5 ограничению";
        exit(-1);
    }
}

#endif //ML_COURSEWORK_PARAMETERS_H
