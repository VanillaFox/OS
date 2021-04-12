#include <stdio.h>
#include <string.h>

int check(char str){
    char vowels[10] = {'e', 'u', 'i', 'o', 'a', 'E', 'U', 'I', 'O', 'A'};
    for(int i = 0; i < 10; i++){
        if(str == vowels[i]){
            return 0;
        }
    }
    return 1;
}

int main(){
    char str[257], res[257];
    while(scanf("%s", str) > 0){
        int j = 0;
        for(int i = 0; i < strlen(str); i++){
            if(check(str[i])){
                res[j] = str[i];
                j++;
            }
        }
        res[j] = '\0';
        printf("%s\n", res);
        fflush(stdout);
    }
    return 0;
}
