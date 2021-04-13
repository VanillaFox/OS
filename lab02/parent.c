#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
    if (argc < 3)
    {
        printf("Usage: ./main filename1 filename2\n");
        exit(-1);
    }

    char *filename1, *filename2;
    printf("Name of the 1st child file: %s\n", argv[1]);
    printf("Name of the 2nd child file: %s\n", argv[2]);
    filename1 = argv[1], filename2 = argv[2];
    int fd1[2];
    int file1 = open(filename1, O_WRONLY | O_CREAT);
    if(file1 == -1){
        perror("File of the first child is not open");
        exit(1);
    }

    pipe(fd1);
    int id;
    id = fork();
    if(id < 0){
        perror("fork!");
        exit(1);
    }
    if(id == 0){
        if(dup2(fd1[0], STDIN_FILENO) == -1 || dup2(file1, STDOUT_FILENO) == -1){
            perror("dup2!");
            exit(1);
        }
        close(fd1[1]);
        execl("./child", "child", (char*)NULL);
        perror("child");
        exit(1);
    }
    else{
        close(fd1[0]);
        int fd2[2];
        int file2 = open(filename2, O_WRONLY | O_CREAT);
        if(file2 == -1){
            perror("File of second child is not open");
            exit(1);
        }
        pipe(fd2);
        id = fork();
        if(id < 0){
            perror("fork!");
            exit(1);
        }
        if(id == 0){
            if(dup2(fd2[0], STDIN_FILENO) == -1 || dup2(file2, STDOUT_FILENO) == -1){
                perror("dup2!");
                exit(1);
            }
            close(fd2[1]);
            execl("./child", "child", (char*)NULL);
        }
        else{
            char str[257];
            int k = 1;
            while(scanf("%s", str) > 0){
                str[strlen(str)] = '\n';
                if(k % 2){
                    write(fd2[1], str, strlen(str));
                }
                else{
                    write(fd1[1], str, strlen(str));
                }
                k++;
                memset(str,'\0',257);
            }

        }
    }
}
