#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main() {
	int status = 123;

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        // Código del proceso hijo
        printf("Soy el proceso hijo. Mi PID es %d\t El valor de status = %d\n", getpid(), status);
        // Punto de depuración aquí
		int i = 0;
		while(i < 10)
			printf("valor i = %d\n", i++);
        // sleep(10); // Simulamos trabajo en el hijo
    } else {
        // Código del proceso padre
        printf("Soy el proceso padre. Mi PID es %d y el PID de mi hijo es %d\t El valor de status = %d\n", getpid(), pid, status);
        wait(NULL);
    }

    return 0;
}

// void replace_exit_status(char *line, int exit_status)
// {
// 	char *pos;
// 	char buffer = malloc(ft_strlen(line) + 10);
// 	char *exit_status_str;
// 	int len;

// 	exit_status_str = ft_itoa(exit_status);
// 	pos = ft_strnstr(line, "$?", ft_strlen(line)-1);
// 	if (!pos)
// 	{
// 		return;
// 	}
// 	len = pos - line;
// 	ft_strlcpy(buffer, line, len);


	


// }


// int main()
// {
//     int exit_status = 123;
//     char line[100] = "echo $?";

//     replace_exit_status(line, exit_status);
//     printf("line = %s\n", line);



// }