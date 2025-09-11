#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int main(void)
{
    int fd[2];
    char buffer[50];
    pipe(fd);

    if (fork() == 0) // Child
    {
		printf("A\n");
		sleep(1);
		printf("B\n");
        close(fd[0]); // okuma ucunu kapat
        char *msg = "Ben child, sana mesaj gönderiyorum!";
        write(fd[1], msg, strlen(msg) + 1);
        close(fd[1]);
    }
    else // Parent
    {
		printf("C\n");
		//sleep(1);
        close(fd[1]); // yazma ucunu kapat
        read(fd[0], buffer, sizeof(buffer));
        printf("Parent okudu: %s\n", buffer);
        close(fd[0]);
		printf("D\n");
    }
}
