#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void mapper(int n, char *executable)
{
    int i = 0, j = 0, pid;
    int fd[n][2];
    char message[4096];
    char ID[1024];
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
            close(fd[i][1]);
            dup2(fd[i][0], 0);
            close(fd[i][0]);
            sprintf(ID, "%d", i);
            char *arr[] = {filePath, ID, NULL};
            execvp(arr[0], arr);
            fflush(stdout);
            close(1);
            exit(EXIT_SUCCESS);
        }
        else if (pid > 0)
        {
            close(fd[i][0]);
            if (fgets(message, 1024, stdin) != NULL)
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
                    for (i = 0; i < n; i++)
                    {
                        close(fd[i][1]);
                    }
                    break;
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            printf("error!!");
            exit(0);
        }
        i++;
        j++;
    }
}

void reducer(int n, char *executable1, char *executable2)
{
    int i = 0, j = 0, pid, flag = 0, k, l;
    int fd[(3 * n) - 1][2];
    char message[4096];
    char ID[1024];
    char *starter;
    starter = "./";
    char *filePath1 = (char *)malloc(1 + strlen(starter) + strlen(executable1));
    char *filePath2 = (char *)malloc(1 + strlen(starter) + strlen(executable2));
    strcpy(filePath1, starter);
    strcat(filePath1, executable1);
    strcpy(filePath2, starter);
    strcat(filePath2, executable2);
    for (k = 0; k < (3 * n - 1); k++)
    {
        if (pipe(fd[k]) < 0)
        {
            printf("pipe error %d %d", k, n);
            exit(EXIT_FAILURE);
        }
    }
    while (1)
    {
        if (i == n)
        {
            i = 0;
        }
        if (j < n)
        {
            pid = fork();
            j++;
        }
        if ((j >= n && j < 2 * n) && flag == 1)
        {
            pid = fork();
            j++;
        }
        if (pid == 0 && flag == 1)
        {
            for (l = 0; l < (3 * n - 1); l++)
            {
                if ((j - 1) == n)
                {
                    if (l != (j - 1) && l != ((2 * n + ((j - 1) % n))))
                    {
                        close(fd[l][0]);
                        close(fd[l][1]);
                    }
                }
                else if (j == (2 * n))
                {
                    if ((l != (j - 1)) && (l != (2 * n - 1 + ((j - 1) % n))))
                    {
                        close(fd[l][0]);
                        close(fd[l][1]);
                    }
                }
                else
                {
                    if ((l != (j - 1)) && (l != ((2 * n + ((j - 1) % n)))) && (l != ((2 * n - 1 + ((j - 1) % n)))))
                    {
                        close(fd[l][0]);
                        close(fd[l][1]);
                    }
                }
            }
            close(fd[(j - 1)][1]);
            dup2(fd[(j - 1)][0], 0);
            close(fd[((j - 1))][0]);
            if ((j - 1) > n)
            {
                close(fd[(2 * n - 1 + ((j - 1) % n))][1]);
                dup2(fd[(2 * n - 1 + ((j - 1) % n))][0], 2);
                close(fd[(2 * n - 1 + ((j - 1) % n))][0]);
            }
            if (j != (2 * n))
            {
                close(fd[(2 * n + ((j - 1) % n))][0]);
                dup2(fd[(2 * n + ((j - 1) % n))][1], 1);
                close(fd[(2 * n + ((j - 1) % n))][1]);
            }

            sprintf(ID, "%d", ((j - 1) % n));
            char *arr[] = {filePath2, ID, NULL};
            execvp(arr[0], arr);
            fflush(stdout);
            close(1);
            exit(EXIT_SUCCESS);
        }

        if (pid == 0 && flag == 0)
        {
            for (l = 0; l < (3 * n - 1); l++)
            {
                if ((l != i) && (l != (i + n)))
                {
                    close(fd[l][0]);
                    close(fd[l][1]);
                }
            }
            close(fd[i][1]);
            dup2(fd[i][0], 0);
            close(fd[i][0]);
            close(fd[i + n][0]);
            dup2(fd[i + n][1], 1);
            close(fd[i + n][1]);
            sprintf(ID, "%d", i);
            char *arr[] = {filePath1, ID, NULL};
            execvp(arr[0], arr);
            fflush(stdout);
            close(1);
            exit(EXIT_SUCCESS);
        }
        else if (pid > 0)
        {
            close(fd[i][0]);
            if (fgets(message, 1024, stdin) != NULL)
            {
                write(fd[i][1], message, strlen(message));
            }
            else
            {
                for (l = 0; l < n; l++)
                {
                    close(fd[l][1]);
                }
                for (l = n; l < 3 * n - 1; l++)
                {
                    close(fd[l][0]);
                    close(fd[l][1]);
                }
                break;
            }
            if (j >= n)
            {
                flag = 1;
            }
        }
        else
        {
            printf("error!!");
            exit(0);
        }
        i++;
    }
}

int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        mapper(atoi(argv[1]), argv[2]);
        for (int i = 0; i < atoi(argv[1]); i++)
        {
            wait(NULL);
        }
    }
    else if (argc == 4)
    {
        reducer(atoi(argv[1]), argv[2], argv[3]);
        for (int i = 0; i < 2*atoi(argv[1]); i++)
        {
            wait(NULL);
        }
    }
    else
    {
        printf("This program needs at least 3 arguments.");
    }

    return 0;
}