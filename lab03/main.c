#include <stdio.h>
#include <pthread.h>
#include <malloc.h>
#include <stdlib.h> 
#include <time.h>
#include <string.h>

struct Args{
    int  start, end, resIndex;
};

pthread_mutex_t mutex;
pthread_t *tid;
struct Args *args;
int** polinoms, **results;
int maxSize, threads;

void multiply(int* first, int* second){
    int* res = (int*)malloc(sizeof(int) * (first[0] + second[0] + 3));
    memset(res, 0, (first[0] + second[0] + 3) * sizeof(int));
    for(int i = 1; i < first[0] + 2; i++){
        for(int j = 1; j < second[0] + 2; j++){
            res[i + j - 1] += first[i] * second[j];
        }
    }
    for(int i = 1; i < (first[0] + second[0] + 3); i++){
        first[i] = res[i];
    }
    first[0] += second[0];
}

void* threedFunc(void* ptrArg){
    struct Args *arg = (struct Args *)ptrArg;
    int steps = arg->end - arg->start + 1;
    int polIndex = arg->start;
    for(int i = 0; i < steps; i++, polIndex++){
        multiply(results[arg->resIndex], polinoms[polIndex]);
    }
    pthread_mutex_lock(&mutex);
    multiply(results[threads], results[arg->resIndex]);
    pthread_mutex_unlock(&mutex);

    return NULL;
}


int main(int argc, char *argv[]){
    if (pthread_mutex_init(&mutex, NULL) != 0){
        perror("Mutex init error");
        exit(1);
    }

    if(argc != 3){
        perror("./prog <number of threads> <number of polynomials>");
        exit(1);
    }

    threads = atoi(argv[1]);
    int polinomsCount = atoi(argv[2]);
    int degree, maxDegree;
    printf("Enter maximum degree of a polynomial: ");
    scanf("%d", &maxDegree);

    maxSize = polinomsCount * maxDegree;
    polinoms = (int**)malloc(sizeof(int*) * polinomsCount);

    for(int i = 0; i < polinomsCount; i++){
        printf("Enter degree of polynomial and coefficients: ");
        scanf("%d", &degree);
        polinoms[i] = (int*)malloc(sizeof(int) * (degree + 2));
        polinoms[i][0] = degree;
        for(int j = degree + 1; j > 0; j--){
            scanf("%d", &polinoms[i][j]);
        }
    }

    tid = (pthread_t*)malloc(sizeof(pthread_t) * threads);
    args = (struct Args*)malloc(threads * sizeof(struct Args));

    results = (int**)malloc(sizeof(int*) * (threads + 1));
    for(int i = 0; i < threads + 1; i++){
        results[i] = (int*)malloc((maxSize + 2) * sizeof(int));
        memset(results[i], 0, (maxSize + 2) * sizeof(int));
        results[i][0] = 0;
        results[i][1] = 1;
    }

    int start = 0, dif;
    dif = polinomsCount / threads;

    for(int i = 0; i < threads - 1; i++){
        args[i].start = start;
        args[i].resIndex = i;
        args[i].end = start + dif - 1;
        start += dif;
    }
    args[threads - 1].start = start;
    args[threads - 1].resIndex = threads - 1;
    args[threads - 1].end = polinomsCount - 1;

    clock_t time;
    time = clock();

    for(int i = 0; i < threads; i++){
        if (pthread_create(&tid[i], NULL, &threedFunc, &args[i]) != 0){
            perror("Can't create thread\n");
            exit(1);
        }
    }

    for(int i = 0; i < threads; i++){
        pthread_join(tid[i], NULL);
    }
    time = clock() - time;

    printf("Execution time %f ms\n", (double)time/CLOCKS_PER_SEC);
    printf("Result: ");
    for(int i = maxSize + 1, pow = maxSize; i > 1; i--, pow--){
        if(results[threads][i]){
            printf("%d*x^%d + ", results[threads][i], pow);
        }
    }

    printf("%d*x^0\n", results[threads][1]);
    if(pthread_mutex_destroy(&mutex) < 0){
        perror("Mutex destroy error");
        exit(1);
    }
    free(polinoms);
    free(tid);
    free(results);
    free(args);
}
