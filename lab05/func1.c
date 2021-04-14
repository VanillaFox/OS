#include "func.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//Лейбниц
float Pi(int K){
    int coef = 1;
    float pi = 0;
    for(int i = 0; i < K; i++){
        pi += (pow(-1, (i + 2))) / coef;
        coef += 2;
    }
    pi *= 4;
    return pi;
}

//двоичная
char* translation(long x){
    char* result;
    int size = floorl(log2l(x)) + 1;
    result = (char*)malloc(sizeof(char) * (size + 1));
    result[size] = '\0';
    while(x > 0){
        size--;
        result[size] = (x % 2) + '0';
        x /= 2;
    }
    return result;

}
