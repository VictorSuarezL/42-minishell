#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int status;

    pid = fork();

    if (pid < 0) { // Error al crear el proceso
        perror("fork failed");
        return 1;
    } else if (pid == 0) { // Proceso hijo
        printf("Soy el proceso hijo. Mi PID es %d\n", getpid());
        exit(132); // Termina el proceso hijo
    } else { // Proceso padre
        printf("Soy el proceso padre. Esperando a que termine mi hijo...\n");
        wait(&status); // Espera a que termine el hijo
        if (WIFEXITED(status)) {
            printf("El proceso hijo terminó con status %d\n", WEXITSTATUS(status));
        }
    }

    return 0;
}