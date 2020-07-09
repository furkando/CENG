#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char id[1024];
    char *execor = "./";
    size_t len = 1024;
    char *line = NULL;
    char *namer, *namer2;
    int pid;
    if (argc < 3)
    {
        printf("Please enter the needed arguments \n");
        exit(1);
    }

    else if (argc == 3)
    {
        int N = atoi(argv[1]);
        int k = 0;
        int **fd = (int **)malloc(N * sizeof(int *));

        for (int index = 0; index < N; index++)
        {
            fd[index] = (int *)malloc(2 * sizeof(int));
            pipe(fd[index]);
        }
        for (int i = 0; i < N; i++)
        {
            if (fork())
            {

                close(fd[i][0]);
                while ((getline(&line, &len, stdin)) != -1)
                {
                    k = k % N;
                    write(fd[k][1], line, strlen(line)); // check
                    k++;
                }
                for (int o = 0; o < N; o++)
                {
                    close(fd[o][1]);
                }
            }
            else
            {
                close(fd[i][1]);
                dup2(fd[i][0], 0);
                close(fd[i][0]);
                sprintf(id, "%d", i);
                asprintf(&namer, "%s%s", execor, argv[2]);
                char *args[] = {namer, id, NULL};
                execv(argv[2], args);
                fflush(stdout);
                close(1);
                exit(0);
            }
        }

        for (size_t i = 0; i < N; i++)
        {
            wait(NULL);
            free(fd[i]);
        }
        free(fd);
    }
    else if (argc == 4)
    {

        int N = atoi(argv[1]);
        int k = 0;
        int **fd = (int **)malloc((3 * N - 1) * sizeof(int *));

        for (int index = 0; index < 3 * N - 1; index++)
        {
            fd[index] = (int *)malloc(2 * sizeof(int));
            pipe(fd[index]);
        }
        for (int i = 0; i < 2 * N; i++)
        {
            pid = fork();
            if (pid > 0)
            {
                close(fd[i][0]);
                while ((getline(&line, &len, stdin)) != -1)
                {
                    k = k % N;
                    write(fd[k][1], line, strlen(line));
                    k++;
                }
                for (int o = 0; o < N; o++)
                {
                    close(fd[o][1]);
                }
            }
            else
            {
                if (i < N)
                {
                    for (int gg = 0; gg < 3 * N - 1; gg++)
                    {
                        if (gg != i && gg != i + N)
                        {
                            close(fd[gg][0]);
                            close(fd[gg][1]);
                        }
                    }
                    close(fd[i][1]);
                    dup2(fd[i][0], 0);
                    close(fd[i][0]);
                    close(fd[i + N][0]);
                    dup2(fd[i + N][1], 1);
                    close(fd[i + N][1]);
                    sprintf(id, "%d", i);
                    asprintf(&namer, "%s%s", execor, argv[2]);
                    char *args[] = {namer, id, NULL};
                    execv(argv[2], args);
                    fflush(stdout);
                    close(1);
                    exit(0);
                }
                else
                {

                    if (i == N)
                    {
                        for (int n = 0; n < 3 * N - 1; n++)
                        {
                            if (n != i && n != i + N)
                            {
                                close(fd[n][0]);
                                close(fd[n][1]);
                            }
                        }
                        close(fd[i][1]);
                        dup2(fd[i][0], 0);
                        close(fd[i][0]);
                        close(fd[i + N][0]);
                        dup2(fd[i + N][1], 1);
                        close(fd[i + N][1]);
                    }
                    else if (i == 2 * N - 1)
                    {
                        for (int n = 0; n < 3 * N - 1; n++)
                        {
                            if (n != i && n != i + N - 1)
                            {
                                close(fd[n][0]);
                                close(fd[n][1]);
                            }
                        }
                        close(fd[i][1]);
                        dup2(fd[i][0], 0);
                        close(fd[i][0]);
                        close(fd[i + N - 1][1]);
                        dup2(fd[i + N - 1][0], 2);
                        close(fd[i + N - 1][0]);
                    }
                    else
                    {
                        for (int n = 0; n < 3 * N - 1; n++)
                        {
                            if (n != i && n != i + N && n != i + N - 1)
                            {
                                close(fd[n][0]);
                                close(fd[n][1]);
                            }
                        }
                        close(fd[i][1]);
                        dup2(fd[i][0], 0);
                        close(fd[i][0]);
                        close(fd[i + N][0]);
                        dup2(fd[i + N][1], 1);
                        close(fd[i + N][1]);
                        close(fd[i + N - 1][1]);
                        dup2(fd[i + N - 1][0], 2);
                        close(fd[i + N - 1][0]);
                    }
                    sprintf(id, "%d", i % N);
                    asprintf(&namer2, "%s%s", execor, argv[3]);
                    char *args[] = {namer2, id, NULL};
                    execv(argv[3], args);
                    fflush(stdout);
                    close(1);
                    exit(0);
                }
            }
        }

        for (int r = N; r < 3 * N - 1; r++)
        {
            close(fd[r][0]);
            close(fd[r][1]);
        }
        for (int k = 0; k < 2 * N; k++)
        {
            wait(NULL);
        }
        for (size_t i = 0; i < 3 * N - 1; i++)
        {
            free(fd[i]);
        }
        free(fd);
    }
    else
    {
        printf("Too many argumants!! \n");
        exit(2);
    }

    return 0;
}
