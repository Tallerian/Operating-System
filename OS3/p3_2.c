#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h> 
#include <ctype.h>
#include <time.h>
#include <pthread.h>


//global variables
uint64_t low, upp;
int num = 1;

int err = 0;


static int

its_perfect(uint64_t num){
    uint64_t i, sum;

    if(num < 2){
        return 0;
    }

    for(i=2, sum = 1; i*i <= num; i++){
        if(num % i == 0){
            sum += (i*i == num) ? i : i + num / i;
        }
    }

    return (sum == num);
}



//finding the upper bound

void* find(void* arg){

    static uint64_t ret[10];
    int j =0; // will be used to store in the array

    uint64_t n_low, n_upp;
    int divisor = *(int*)arg;

    int diff = (upp - low) / num;  //908

    //just in case it is once
    if(diff > (0.5*upp)){
        diff = upp + 1;
    }

    // lower bound
    if(divisor == 1){  /// process -1
        n_low = low;
    }else{
        int temp = (divisor-2)*diff; //
        n_low = temp + diff;
        
    }

    if(divisor == 1){
        n_upp = diff-1;
    }else{
        n_upp = diff + ((divisor-1)*diff) - 1;
    }
    

    //check if n_upp has passed upper bound
    if(n_upp > upp){
        n_upp = upp;
    } 

    if(err){
        fprintf(stderr, "perfect: t%d searching [%ld, %ld]\n", divisor-1, n_low, n_upp);
    }

    for(uint64_t i = n_low; i <= n_upp; i++){
        if(its_perfect(i) != 0){
            ret[j] = i;
            j++;
            printf("%ld\n", i);
        }   
    }
    free(arg);
    

    if(err){
        fprintf(stderr, "perfect: t%d searching [%ld, %ld]\n", divisor-1, n_low, n_upp);
    }
    return NULL;
}


int main(int argc, char *argv[]){

    int opt;

    if(argc == 1){
        low = atoi("1");
        upp = atoi("10000");
    }
    else{
    while((opt= getopt(argc, argv, "s:e:t:v"))!= -1){
        switch (opt)
        {
        case 's':

            if (optarg != NULL)
            {

                low = atoi(optarg);
                
            }
            
            break;

        case 'e':

            if (optarg != NULL)
            {
                upp = atoi(optarg);
            }
            
            break;

        case 't':

            if(optarg != NULL){
                num = atoi(optarg);
            }

        case 'v':
            err = 1;
            break;
        default:
            break;
        }
    }
    }

    //now we check if we have obtained the low and upper bound

    if(low == 0){
        low = atoi("1");
    }
    if(upp == 0){
        upp = atoi("10000");
    }

    //check how many threads to run

    pthread_t thread;
    pthread_t arrthread[num];

    //starting to measure time
    clock_t start, end;
    double cpu_time_used;

    double before = time(NULL);
 
    for(int i=1; i<=num; i++){
        int* a = malloc(sizeof(int));
        *a = i;
        start = clock();
        pthread_create(&arrthread[i], NULL, find, a);
    }

    for(int i=1; i<=num; i++){
        pthread_join(arrthread[i], NULL);
    }

    long after = time(NULL);

    double differ = after - before;

    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    //we are gonna store the cpu time in an excel file to get the graph

    FILE *fp;
    fp = fopen("Graph.csv", "a");
    if(fp == 0){
        fprintf(stderr, "Error opening file");
    }
    
    fprintf(fp, "\n%d,%lf", num,  differ);

    fclose(fp);


    return 0;
}