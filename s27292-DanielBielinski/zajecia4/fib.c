#include <stdio.h>
#include <stdlib.h>

int fibrec(int n){
    if(n <= 1){
        return n;
    } else {
        return (fibrec(n-1)+fibrec(n-2));
    }
}

int fibiter(int n){
    int preprev , i , prev = 0 , current = 1;
    if (n<=1){
        return n;
    } else {
        for (i = 1 ; i < n ; i ++){
            preprev = prev;
            prev = current;
            current = preprev + prev;
        }
        return current;
    }
}

int main(int argc, char const *argv[]){
    int input = atoi(argv[1]);
    printf("wynik recursive = %d \n",fibrec(input));
    printf("wynik iterative = %d \n",fibiter(input));
    return 0;
}
