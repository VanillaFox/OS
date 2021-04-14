#include <stdio.h>
#include "func.h"

int main(){
    int choice, k;
    long x;
    while(printf("Enter type of funtion: ") && scanf("%d", &choice) > 0){
        if(choice == 1){
            //printf("Enter row length: ");
            scanf("%d", &k);
            printf("Pi is %f\n", Pi(k));
        }
        else if(choice == 2){
            //printf("Enter x: ");
            scanf("%ld", &x);
            printf("x is %s\n", translation(x));
        }
        else{
            printf("Exit\n");
            break;
        }
    }
    return 0;
}
