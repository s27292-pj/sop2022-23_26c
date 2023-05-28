#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    FILE *file;
    int n,i,number;
    int numarray[512];
    file = fopen("input.txt","r");
    if(file == NULL){
        printf("Error reading file");
        return 0;
    }
    fscanf(file, "%d", &n);
    for(i = 0; i < n ; i++){
        fscanf(file, "%d", &number);
        numarray[i] = number;
    }
    for (i = 0; i < n; i++) {
        printf("%d\n", numarray[i]);
    }

    fclose(file);

    return 0;
}