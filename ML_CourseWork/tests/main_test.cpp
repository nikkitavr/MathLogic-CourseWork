//
// Created by Admin on 27.12.2021.
//

#include <iostream>
#include <fstream>
#include <bdd.h>
#include "../Parameters.h"

std::ofstream outSats("..out/test/testSats.txt");
std::ofstream outTable("..out/test/testTable.txt");

void fun(char* varset, int size){
    for (int i = 0; i < size; i++) {
    outSats << (varset[i] < 0 ? 'X' : (char)('0' + varset[i]));
    }
    outSats << std::endl;
}


bdd p(int i, int k, int j);
bdd cond5();
bdd cond6();

int main(){
    bdd_init(10000000, 1000000);
    bdd_setvarnum(N * M * Z);

    bdd F = bddtrue;

    F &= p(1, 1, 1);
    F &= cond5();
    F &= cond6();

    std::cout << "node count: " << bdd_nodecount(F) << std::endl;

    outSats << bdd_satcount(F) << " sats:" << std::endl;
    bdd_allsat(F, fun);
    outTable << bddtable << F << std::endl;



    return 0;
}

bdd p(int i, int k, int j){

    bdd Mk = bddtrue;
    for(int t = 0; t < T; t++){
        Mk &= ((j >> t)&1) == 1 ? bdd_ithvar((M * T * i) + (T * k) + t) : bdd_nithvar((M * T * i) + (T * k) + t);
    }
    return Mk;
}

bdd cond5(){
    bdd temp = bddtrue;
    for(int k = 0; k < M; k++) { //проверяем каждое св-во
        for(int j = 0; j < J; j ++) {

            for (int i1 = 0; i1 < N - 1; i1++) { //и сравниваем по нему между собой все объекты
                for (int i2 = i1 + 1; i2 < N; i2++) {
                    temp &= p(i1, k, j) >> !(p(i2, k, j));
                }
            }
        }
    }
    return temp;
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