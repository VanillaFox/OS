#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char *argv[]){
    if (argc < 3){
        printf("Usage: ./main filename1 filename2\n");
        exit(-1);
    }

    char *filename1, *filename2;
    printf("Name of the 1st child file: %s\n", argv[1]);
    printf("Name of the 2nd child file: %s\n", argv[2]);
    filename1 = argv[1], filename2 = argv[2];

    int fd1 = shm_open("File1", O_RDWR | O_CREAT, S_IRWXU);
    int fd2 = shm_open("File2", O_RDWR | O_CREAT, S_IRWXU);
    if (fd1 == -1 || fd2 == -1) {
        perror("Can't open shared memory object");
        exit(1);
    }

    if(ftruncate(fd1, getpagesize()) < 0){
        perror("ftruncate error\n");
        exit(1);
    }

    if(ftruncate(fd2, getpagesize()) < 0){
        perror("ftruncate error\n");
        exit(1);
    }

    char *file1 = (char*)mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);
    char *file2 = (char*)mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
   
    if(file1 == MAP_FAILED || file2 == MAP_FAILED){
        perror("mmap");
        exit(1);
    }
    memset(file1, '\0', getpagesize());
    memset(file2, '\0', getpagesize());

    int id = fork();
    if(id < 0){
        perror("fork!");
        exit(1);
    }
    if(id == 0){
        execl("./child", "child", filename1, "File1", (char*) NULL);
        perror("child");
        exit(1);
    }
    else{
        id = fork();
        if(id < 0){
            perror("fork!");
            exit(1);
        }
        if(id == 0){
            execl("./child", "child", filename2, "File2", (char*) NULL);
            perror("child");
            exit(1);
        }
        else{
            char str[257];
            int k = 1;
            int idx1 = 0;
            int idx2 = 0;
            int size;
            while(scanf("%s", str) > 0){
                size = strlen(str);
                str[size] = '\n';
                size++;
                str[size] = '\0';
                if(k % 2){
                    for(int i = 0; i < size; i++){
                        file1[idx1++] = str[i];
                    }
                }
                else{
                    for(int i = 0; i < size; i++){
                        file2[idx2++] = str[i];
                    }
                }
                k++;
                memset(str,'\0',257);
            }
            file1[idx1] = '$';
            file2[idx2] = '$';
        }

    }
    sleep(1);
    if(munmap(file1, getpagesize()) != 0 || munmap(file2, getpagesize()) != 0){
        perror("mumap");
        exit(1);
    }
    if(close(fd1) < 0 || close(fd2) < 0){
        perror("close");
        exit(1);
    }

    return 0;
}
