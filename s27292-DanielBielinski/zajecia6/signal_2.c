#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>

volatile sig_atomic_t exit_flag = 1;

void signalUSR1(int signal) {
    if (signal == SIGUSR1) {
        printf("Wyrkyto probe uruchomienia drugiej instancji programu.\n");
    }
}

void signalSIGINT(int signal) {
    if (signal == SIGINT) {
        system("rm /tmp/lock.txt");
        printf("\nsigint has been capture, exiting program\n");
        exit_flag = 0;
    }
}

int main() {
    FILE* file;
    int pid = getpid(),kill();
    const char* filepath = "/tmp/lock.txt";

    if (access(filepath, F_OK) == 0) {
        signal(SIGUSR1, signalUSR1);
        file = fopen("/tmp/lock.txt", "r");
        kill(pid, SIGUSR1);

    } else {
        signal(SIGINT, signalSIGINT);
        file = fopen("/tmp/lock.txt", "w");

        if (file == NULL) {
            printf("Error opening the file.\n");
            return 1;
        }

        fprintf(file, "%d\n", pid);
        fclose(file);

        while (exit_flag) {
            sleep(1);
        }
    }

    return 0;
}
