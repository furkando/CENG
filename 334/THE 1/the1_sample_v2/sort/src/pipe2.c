#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void sleeper(int n)
{
    if (n > 10)
    {
        sleep(1);

        if (n > 20)
        {
            sleep(1);

            if (n > 30)
            {
                sleep(1);

                if (n > 40)
                {
                    sleep(1);
                    if (n > 50)
                    {
                        sleep(1);
                    }
                }
            }
        }
    }
}

void mapper(int n, char *executable)
{
    int i = 0, j = 0, pid;
    int fd[n][2];
    char message[5000];
    char message2[5000];
    char ID[1000];
    char *starter;
    starter = "./";
    char *filePath = (char *)malloc(1 + strlen(starter) + strlen(executable));
    strcpy(filePath, starter);
    strcat(filePath, executable);
    while (1)
    {
        if (i == n)
        {
            i = 0;
        }
        if (j < n)
        {
            if (pipe(fd[i]) < 0)
            {
                printf("error!");
                exit(0);
            }
            pid = fork();
        }
        if (pid == 0)
        {
            sleep(1);
            sleeper(n);

            close(fd[i][1]);
            dup2(fd[i][0], 0);
            close(fd[i][0]);
            // scanf("%s", message2);
            // fgets(message, 80, stdin);
            sprintf(ID, "%d", i);
            char *arr[] = {filePath, ID, NULL};
            execvp(arr[0], arr);
            // read(0, message2, 50);
            // printf("CHILD (%d)(%d)(%d) %s", i, getpid(), getppid(), message2);
            // read(0, message2, 50);
            // printf("CHILD (%d)(%d)(%d) %s", i, getpid(), getppid(), message2);
            // read(0, message2, 50);
            // printf("CHILD (%d)(%d)(%d) %s", i, getpid(), getppid(), message2);
            fflush(stdout);
            close(1);
            break;
        }
        else if (pid > 0)
        {
            close(fd[i][0]);
            // dup2(fd[i][1], 1);
            // close(fd[i][1]);
            // fgets(message, 200, stdin);
            // scanf("%s",message);
            if (fgets(message, 1000, stdin) != NULL)
            {
                write(fd[i][1], message, strlen(message));
            }
            else
            {
                for (; j < n; j++)
                {
                    pid = fork();
                }
                if (pid > 0)
                {
                    sleep(2);
                    sleeper(n);
                    for (i = 0; i < n; i++)
                    {
                        close(fd[i][1]);
                    }
                    wait(NULL);
                    break;
                }
                else
                {
                    break;
                }
            }
            // printf("%s", message);
        }
        else
        {
            printf("error!!");
            exit(0);
        }
        i++;
        j++;
    }

    // if (pid > 0)
    // {
    //     wait(NULL);
    // }
}

int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        mapper(atoi(argv[1]), argv[2]);
    }
    wait(NULL);

    return 0;
}