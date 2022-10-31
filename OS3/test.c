#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h> 
#include <ctype.h>


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

    printf("%d", sum==num);
    return (sum == num);
}

int main(){

    uint64_t num=100;

    if(its_perfect(num)){
        printf("%ld", num);
    }

}