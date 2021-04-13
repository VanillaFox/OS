#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


int check(char str){
    char vowels[10] = {'e', 'u', 'i', 'o', 'a', 'E', 'U', 'I', 'O', 'A'};
    for(int i = 0; i < 10; i++){
        if(str == vowels[i]){
            return 0;
        }
    }
    return 1;
}


int main(int argc, char *argv[]){
    char* filename = argv[1], *file = argv[2];
    int fd1 = shm_open(file, O_RDWR | O_CREAT, S_IRWXU);

    if(fd1 < 0){
        perror("Can't open shared memory object");
        exit(1);
    }

    char* filemmap = (char*) mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);

    if(filemmap == MAP_FAILED){
        perror("mmap");
        exit(1);
    }

    int res = open(filename, O_RDWR | O_CREAT);
    if(res < 0){
        perror("open");
        exit(1);
    }

    if(ftruncate(res, getpagesize()) < 0){
        perror("ftruncate error\n");
        exit(1);
    }

    char* resmmap = (char*) mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, res, 0);
    if(resmmap == MAP_FAILED){
        perror("mmap");
        exit(1);
    }

    memset(resmmap, '\0', getpagesize());
    
    int i = 0, j = 0;
    while(filemmap[i] != '$'){
        if(filemmap[i] != '\0' && filemmap[i] != '$'){
            if(check(filemmap[i])){
                resmmap[j] = filemmap[i];
                j++;
            }
            i++;
        }
    }

    if(munmap(resmmap, getpagesize()) != 0 || munmap(filemmap, getpagesize()) != 0){
        perror("mumap");
        exit(1);
    }
    if(close(fd1) < 0){
        perror("close fd");
    }
    if(shm_unlink(file) != 0){
        perror("shm_unlink file");
        exit(1);
    }
    if(close(res) < 0){
        perror("close res");
        exit(1);
    }
    return 0;
}
