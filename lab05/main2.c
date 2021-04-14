#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "func.h"

int main(){
    float (*Pi)(int k);
    char*(*translation)(long x);
    char* libs[] = {"libfunc1.so", "libfunc2.so"};
    int lib = 0;
    int choice;
    void* handler = NULL;

    while(printf("Enter type of funtion: ") && scanf("%d", &choice) > 0){
        if(choice == 0){
            if(handler){
                dlclose(handler);
            }
            lib = 1 - lib;
            printf("Implementation changed!\n");
        }
        else{
            handler = dlopen(libs[lib], RTLD_LAZY);
            if (!handler){
                fprintf(stderr, "dlopen() error: %s\n", dlerror());
                exit(1);
            }

            Pi = dlsym(handler, "Pi");
            translation = dlsym(handler, "translation");
            if(choice == 2){
                long x;
                scanf("%ld", &x);
                printf("x is %s\n", translation(x));
            }
            else if(choice == 1){
                int k;
                scanf("%d", &k);
                printf("Pi is %f\n", Pi(k));
            }
            else{
                printf("Exit\n");
                dlclose(handler);
                break;
            }
        }
        
    }

    return 0;
}
