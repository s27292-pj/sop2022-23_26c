#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    FILE *file;
    int currentnumber,numbercount;
    char filename[100];
    char* digits;
    int i,n;
    if(argc < 2){
        printf("Podaj nazwe pliku do wczytania: ");
        scanf("%s", filename);
        file = fopen(filename,"r");
    } else if (argc > 2) {
        printf("Niepoprawna ilosc argumentow\n");
        return 0;
    } else {
        file = fopen(argv[1],"r");
        if(file == NULL){
            printf("Error reading file");
            return 0;
        }
    }
    fscanf(file,"%d",&numbercount);
    n = numbercount;
    digits = calloc(numbercount, sizeof(int));
    if ( digits == NULL){
        printf("Blad alokacji pamieci\n");
        return 0;
    }
    for(i = 0; i < n ; i++){
        fscanf(file,"%d", &currentnumber);
        digits[i] = currentnumber;
    }
    if(file){
        fclose(file);
    }
    for(i=numbercount;i>0;i--){
        printf("%d\n",digits[i-1]);
    }
    free(digits);

    return 0;
}
