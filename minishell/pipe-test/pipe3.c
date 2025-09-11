#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    int fd[2];
    pipe(fd);

    if (fork() == 0) // Child1
    {
        close(fd[0]);
        char *msg = "Child1'den Child2'ye selam!";
        write(fd[1], msg, strlen(msg) + 1);
        close(fd[1]);
    }
    else
    {
        if (fork() == 0) // Child2
        {
            close(fd[1]);
            char buffer[50];
            read(fd[0], buffer, sizeof(buffer));
            printf("Child2 okudu: %s\n", buffer);
            close(fd[0]);
        }
        else
        {
            // Parent her iki ucu da kapatıyor
            close(fd[0]);
            close(fd[1]);
        }
    }
}
