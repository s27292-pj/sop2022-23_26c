#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char const *argv[]){
    FILE *file;
    unsigned char buffer[16];
    unsigned int address = 0, read_bytes;
    int i;
    printf("Loading file: %s\n",argv[1]);
    file = fopen(argv[1],"r");
    if(file == NULL){
        printf("Error reading file");
        return 0;
    }
    while ((read_bytes = fread(buffer, sizeof(char), 16, file)) > 0) {
        printf("%08x ", address);
                for (i = 0; i < 16; i++) {
            if (i < read_bytes) {
                printf("%02x ", buffer[i]);
            } else {
                printf("   ");
            }

            if (i == (16 / 2) - 1) {
                printf(" ");
            }
        }
        printf("| ");
        for (i = 0; i < read_bytes; i++) {
            if (isprint(buffer[i])) {
                printf("%c", buffer[i]);
            } else {
                printf(" ");
            }
        }
        printf("\n");
        address += 16;
    }

    printf("Closing file: %s\n",argv[1]);
    fclose(file);
    return 0;
}
