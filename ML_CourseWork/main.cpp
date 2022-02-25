/// Курсовая работа по МатЛогике за 5 семестр
/// Вариант: 6-3-2
/// Выполнена студентом группы 3530904/90104
/// Воротников Н.П.

#include <iostream>
#include <bdd.h>
#include <cmath>
#include <fstream>
#include <vector>
#include "Direction.h"
#include "Parameters.h"
#include "Utils.h"

//функция p, задает св-во k с десятичным значением j для объекта i
bdd p(int i, int k, int j);

//функции ограничений
//1 тип: объект i имеет св-во k со значением j
bdd cond1(int i, int k, int j);
//2 тип: объект имеет св-во K1j1 <-> этот же объект имеет св-во K2j2
bdd cond2(int k1, int j1, int k2, int j2);
//3 тип: объект по заданному направлению от объекта со св-ми K1j1 имеет св-ва K2j2
bdd cond3(int k1, int j1, Direction direction, int k2, int j2);
//4 тип: объект со св-ом K1j1 сосед объекту со св-ом K2j2
bdd cond4(int k1, int j1, int k2, int j2);
//5 тип: у каждого объекта занчение св-ва уникально
void cond5F(bdd& F);
//6 тип: параметры принимают значения только из заданных множ-в
bdd cond6();

//служебные функции для записи значений ограничений из файла и последующей конъюнкции с F
void addCond1(bdd& F, std::string path);
void addCond2(bdd& F, std::string path);
void addCond3(bdd& F, std::string path);
void addCond4(bdd& F, std::string path);

//возвращает номер соседа объекта i по направлению direction
int get_neighbour(int i, Direction direction, int flag = NO_FLAG);

//служебные
char* var;
int arr[N][M];

void print();
void build(char *varset, unsigned n, unsigned I);
void fun(char *varset, int size);
void fun2(char *varset, int size);

std::ofstream outSln(SOLUTION_OUT_PATH);
std::ofstream outTable(TABLE_OUT_PATH);
std::ofstream outNodes(NODES_OUT_PATH);
std::ofstream  outIntrp("../out/interpretation/intr.txt");

int main() {
    param_check(); //проверка входных параметров

    //инициализация
    bdd_init(10000000, 1000000);
    bdd_setvarnum(N * M * Z);
    var = new char[N * M * log_N];

    //основная логика
    bdd F = bddtrue;
    //addCond1(F, CONDITION1_PATH);
    //addCond2(F, CONDITION2_PATH);
    //addCond3(F, CONDITION3_PATH);
    //addCond4(F, CONDITION4_PATH);
    //cond5F(F);
    F &= cond6();

    //Вывод решений
    outSln << bdd_satcount(F) << " solutions, " << bdd_nodecount(F) << " nodes.\nSolutions:\n\n";
    std::cout << "node count: " << bdd_nodecount(F) << std::endl;
    std::cout  << "solutions: " << bdd_satcount(F) << std::endl << std::endl;
    /*bdd_allsat(F, fun);

    outTable << bddtable << F << std::endl;
    bdd_allsat(F, fun2)*/;

    //Завершение работы
    bdd_done();
    return 0;
}


bdd p(int i, int k, int j){

    bdd Mk = bddtrue;
    for(int t = 0; t < T; t++){
        Mk &= ((j >> t)&1) == 1? bdd_ithvar((M * T * i) + (T * k) + t) : bdd_nithvar((M * T * i) + (T * k) + t);
    }
    return Mk;
}

int get_neighbour(int i, Direction direction, int flag){
    if(i == -1) return -1;

    int column = (i % SIZE) + 1;
    int row = (i / SIZE) + 1;
    bool is_glue;

    if(flag == FLAG_TRUE) {
        is_glue = true;
    } else if (flag == FLAG_FALSE){
        is_glue = false;
    } else {
        is_glue = (column == COLUMN_GLU || row == ROW_GLU);
    }

    int neighbour;
    switch (direction) {
        case Direction::UP:
            if(row == 1){
                if (is_glue){
                    neighbour = i + SIZE * (SIZE - 1);
                } else {
                    neighbour = -1;
                }
            } else {
                neighbour = i - SIZE;
            }
            break;
        case Direction::DOWN:
            if(row == SIZE){
                if (is_glue){
                    neighbour = i - SIZE * (SIZE - 1);
                } else {
                    neighbour = -1;
                }
            } else {
                neighbour = i + SIZE;
            }
            break;
        case Direction::LEFT:
            if(column == 1){
                if (is_glue){
                    neighbour = i + (SIZE - 1);
                } else {
                    neighbour = -1;
                }
            } else {
                neighbour = i -1;
            }
            break;
        case Direction::RIGHT:
            if(column == SIZE){
                if (is_glue){
                    neighbour = i - (SIZE - 1);
                } else {
                    neighbour = -1;
                }
            } else {
                neighbour = i + 1;
            }
            break;

        default:
            int flag2;
            if(is_glue){
                flag2 = FLAG_TRUE;
            } else {
                flag = FLAG_FALSE;
            }
            switch (direction) {
                case Direction::UP_LEFT:
                    neighbour = get_neighbour(i, Direction::UP, flag2);
                    neighbour = get_neighbour(neighbour, Direction::LEFT, flag2);
                    break;
                case Direction::UP_RIGHT:
                    neighbour = get_neighbour(i, Direction::UP, flag2);
                    neighbour = get_neighbour(neighbour, Direction::RIGHT, flag2);
                    break;
                case Direction::DOWN_LEFT:
                    neighbour = get_neighbour(i, Direction::DOWN, flag2);
                    neighbour = get_neighbour(neighbour, Direction::LEFT, flag2);
                    break;
                case Direction::DOWN_RIGHT:
                    neighbour = get_neighbour(i, Direction::DOWN, flag2);
                    neighbour = get_neighbour(neighbour, Direction::RIGHT, flag2);
                    break;
            }
    }
    return neighbour;
}


bdd cond1(int i, int k, int j){
    return p(i, k, j);
}

bdd cond2(int k1, int j1, int k2, int j2){
    bdd temp = bddtrue;
    for (int i = 0; i < N; i++) {
        temp &= (p(i, k1, j1) >> p(i, k2, j2)) & (p(i, k2, j2) >> p(i, k1, j1));
    }
    return temp;
}

bdd cond3(int k1, int j1, Direction direction, int k2, int j2){
    bdd temp = bddtrue;
    for(int i = 0; i < N; i++) {
        int neighbour = get_neighbour(i, direction);
        if(neighbour == -1){
            //std::cout << "Skipped one of cond3: obj " << i << " don't have neighbour by direction: " <<
              //directionToString(direction) << ".\n";
        } else{
            temp &= (p(i, k1, j1) >> p(neighbour, k2, j2)) & (p(neighbour, k2, j2) >> p(i, k1, j1));
        }
    }
    return temp;
}

bdd cond4(int k1, int j1, int k2, int j2){
    bdd temp = bddfalse;
    //std::vector<Direction> dirs(directions, directions + sizeof(directions) / sizeof(directions[0]));
    for(Direction direction : directions) {
        temp |= cond3(k1, j1, direction, k2, j2);
    }
    return  temp;
}

void cond5F(bdd& F){
    for(int k = 0; k < M; k++) { //проверяем каждое св-во
        for(int j = 0; j < J; j ++) {

            for (int i1 = 0; i1 < N - 1; i1++) { //и сравниваем по нему между собой все объекты
                for (int i2 = i1 + 1; i2 < N; i2++) {
                    F &= (!(p(i1, k, j))) | (!(p(i2, k, j)));
                }
            }
        }
    }
}

bdd cond6(){
    bdd temp1 = bddtrue;
    for(int i = 0; i < N; i++){ //проходим по всем объектам и св-вам
        for(int k = 0; k < M; k++){
            bdd temp2 = bddfalse;
            for(int j = 0; j < J; j++){ //и говорим что значение св-ва k для объекта i может быть в промежутке [0; J]
                temp2 |= p(i, k, j);
            }
            temp1 &= temp2;
        }
    }
    return temp1;
}

void addCond1(bdd& F, std::string path){
    outIntrp << "1 тип\n";
    std::ifstream fin(path);
    if(fin){
        for (int cond_num = 0; cond_num < NUM_OF_N1; cond_num++) {
            goToCond(fin);
            int i, k, j;
            fin >> i >> k >> j;
            outIntrp << "Объект под номером " << i << " обладает свойством " << k << " со значением " << k << "\n";
            F &= cond1(i, k, j);
        }
    } else {
        failed_to_open("condition 1");
    }
}

void addCond2(bdd& F, std::string path){
    outIntrp << "2 тип\n";

    std::ifstream fin(path);
    if(fin){
        for (int cond_num = 0; cond_num < NUM_OF_N2; cond_num++) {
            goToCond(fin);
            int k1, j1, k2, j2;
            fin >> k1 >> j1 >> k2 >> j2;
            outIntrp << "Объекту характерно свойство " << k1 << " со значением " << j1 <<
            ", тогда и только тогда, когда ему же характерно свойство " << k2 << " со значением " << j2 <<  "\n";
            F &= cond2(k1, j1, k2, j2);
        }
    } else {
        failed_to_open("condition 2");
    }
}

void addCond3(bdd& F, std::string path){
    outIntrp << "3 тип\n";
    std::ifstream fin(path);
    if(fin){
        for (int cond_num = 0; cond_num < NUM_OF_N3; cond_num++) {
            goToCond(fin);
            int k1, j1, k2, j2;
            std::string dir;
            fin >> k1 >> j1 >> dir >> k2 >> j2;
            Direction direction = stringToDirection(dir);
            if(direction == Direction::NONE){
                std::cerr << "Cant recognize direction. Terminated.";
                exit(1);
            }
            outIntrp << "Объект, обладающий свойством " << k1 << " со значением " << j1 <<
                     ", находится " << dir << " от объекта, обладающим свойством " << k2 << " со значением " << j2 <<  "\n";
            F &= cond3(k1, j1, direction, k2, j2);
        }
    } else {
        failed_to_open("condition 3");
    }
}

void addCond4(bdd& F, std::string path){
    outIntrp << "4 тип\n";
    std::ifstream fin(path);
    if(fin){
        for (int cond_num = 0; cond_num < NUM_OF_N4; cond_num++) {
            goToCond(fin);
            int k1, j1, k2, j2;
            fin >> k1 >> j1 >> k2 >> j2;
            outIntrp << "Объект, обладающий свойством " << k1 << " со значением " << j1 <<
                     ", находится рядом с объектом, обладающим свойством " << k2 << " со значением " << j2 <<  "\n";
            F &= cond4(k1, j1, k2, j2);
        }
    } else {
        failed_to_open("condition 4");
    }
}


void print() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int Ji = i * M * LOG_N + j * LOG_N;
            int num = 0;

            for (int k = 0; k < LOG_N; k++) {
                num += (int) (var[Ji + k] << k);
            }

            arr[i][j] = num;
        }
    }

    for (int i = 0; i < N; i++) {
        std::cout << i << ": ";
        outSln << i << ": ";

        for (int j = 0; j < M; j++) {
            std::cout << arr[i][j] << "  ";
            outSln << arr[i][j] << "  ";
        }

        std::cout << "\n";
        outSln << "\n";
    }
    std::cout << "\n";
    outSln << "\n";
}

void build(char *varset, unsigned n, unsigned I) {
    if (I == n - 1) {
        if (varset[I] >= 0) {
            var[I] = varset[I];
            print();
            return;
        }
        var[I] = 0;
        print();
        var[I] = 1;
        print();
        return;
    }
    if (varset[I] >= 0) {
        var[I] = varset[I];
        build(varset, n, I + 1);
        return;
    }
    var[I] = 0;
    build(varset, n, I + 1);
    var[I] = 1;
    build(varset, n, I + 1);
}

void fun(char *varset, int size) {
    std::cout << "i  ";
    outSln << "i  ";
    for (int a = 0; a < M; a++) {
        std::cout << "k" << a << " ";
        outSln << "k" << a << " ";
    }
    std::cout << "\n";
    outSln << "\n";

    build(varset, size, 0);
}


void fun2(char *varset, int size){
    for(int i = 0; i < N; i++){
        for(int k = 0; k < M; k++){
            outNodes << "i" << i << "k" << k << "(";
            for(int t = 0; t < T; t++){
                outNodes << (varset[i * M * T + T * k + t]  < 0 ? 'X' : (char)('0' + varset[i * M * T + T * k + t]));
            }
            outNodes << ") ";
        }
    }

    outNodes << std::endl;
}
