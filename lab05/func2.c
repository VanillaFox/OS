#include "func.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//Валлис
float Pi(int K){
    float dividend = 2;
    float divider = 1;
    float pi = 1;
    for(int i = 0, p = 1; i < K; i++, p++){
        pi *= dividend / divider;
        if(p % 2){
            divider += 2;
        }else{
            dividend += 2;
        }
    }
    pi *= 2;
    return pi;
}

//троичная
char* translation(long x){
    char* result;
    int size = floor(logl(x) / log(3)) + 1;
    result = (char*)malloc(sizeof(char) * (size + 1));
    result[size] = '\0';
    while(x > 0){
        size--;
        result[size] = (x % 3) + '0';
        x /= 3;
    }
    return result;
}
