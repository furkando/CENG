#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char **argv)
{
    char *n1, word[1024], word2[1024], word3[1024];
    n1 = argv[1];
    int N = atoi(n1);
    char *ex = "./", stri[1024], strk[1024];
    char *name = argv[2], *name2 = argv[3];
    char *exec, *exec2;
    asprintf(&exec, "%s%s", ex, name); //Ilk arguman icin
    asprintf(&exec2, "%s%s", ex, name2);
    int fd[N * 2], i = 0, k = 0, fd2[2 * ((3 * N) - 1)];
    pid_t pid = 1;
    if (argc == 3)
    {
        while (fgets(word, 1024, stdin))
        {
            i = i % N;
            if (k < N && pid > 0)
            {
                pipe(fd + 2 * k);
                pid = fork();
            }
            if (pid == 0)
            { // Child
                usleep(i + 1);
                close(fd[2 * i + 1]);
                dup2(fd[2 * i], 0);
                close(fd[2 * i]);
                sprintf(stri, "%d", i);
                char *array[] = {exec, stri, NULL};
                execv(name, array);
                break;
            }
            if (pid > 0)
            { //Parent
                close(fd[2 * i]);
                //dup2(fd[i][1],1);
                //close(fd[i][1]);
                write(fd[2 * i + 1], word, strlen(word));
            }
            i++;
            k++;
        }

        for (int j = 0; j < N; j++)
        {
            close(fd[2 * j + 1]);
        }
        if (pid > 0)
        {
            for (int j = 0; j < N; j++)
            {
                wait(NULL);
            }
        }
    }

    else if (argc == 4)
    {
        for (int x = 0; x < (3 * N) - 1; x++)
        {
            pipe(fd2 + 2 * x);
        }
        while (fgets(word, 1024, stdin))
        {
            i = i % N;
            if (k < N && pid > 0)
            {
                pid = fork();
            }
            if (k >= N && k < 2 * N)
            {
                pid = fork();
            }
            if (pid == 0 && k >= N)
            { // Reducer - Child
                // sleep(1);
                //usleep(i+1);

                if (k == N)
                { //en üst
                    for (int x = 0; x < 2 * (3 * N - 1); x++)
                    {
                        if (x != (4 * N) + 1 && x != (2 * k))
                        {
                            close(fd2[x]);
                        }
                    }
                    dup2(fd2[2 * k], 0);
                    close(fd2[2 * k]);
                    dup2(fd2[4 * N + 1], 1);
                    close(fd2[4 * N + 1]);
                }
                else if (k == 2 * N - 1)
                { //en alt
                    for (int x = 0; x < 2 * (3 * N - 1); x++)
                    {
                        if (x != 2 * (k + N) - 2 && x != 2 * k)
                        {
                            close(fd2[x]);
                        }
                    }
                    dup2(fd2[2 * k], 0);
                    close(fd2[2 * k]);
                    dup2(fd2[2 * (k + N) - 2], 2);
                    close(fd2[2 * (k + N) - 2]);
                }
                else
                { //orta
                    for (int x = 0; x < 2 * (3 * N - 1); x++)
                    {
                        if (x != 2 * (k + N) - 2 && x != 2 * k && x != 2 * (k + N) + 1)
                        {
                            close(fd2[x]);
                        }
                    }
                    dup2(fd2[2 * k], 0);
                    close(fd2[2 * k]);
                    dup2(fd2[2 * (k + N) - 2], 2);
                    close(fd2[2 * (k + N) - 2]);
                    dup2(fd2[2 * (k + N) + 1], 1);
                    close(fd2[2 * (k + N) + 1]);
                }

                sprintf(strk, "%d", k % N);
                char *array[] = {exec2, strk, NULL};
                execv(name2, array);
                //printf("geldi\n");
                // read(0,word3,1024);
                // printf("W:%s",word3);
                fflush(stdout);
                close(1);
                break;
            }
            if (pid == 0 && k < N)
            { // Child
                // usleep(i+1);
                for (int x = 0; x < ((3 * N) - 1); x++)
                {
                    if (x != ((2 * i) + 1) && x != (2 * i) && x != (2 * (i + N)) && x != (2 * (i + N) + 1))
                    {
                        close(fd2[x]);
                    }
                }
                close(fd2[2 * i + 1]);
                dup2(fd2[2 * i], 0);
                close(fd2[2 * i]);
                close(fd2[2 * (i + N)]);
                dup2(fd2[2 * (i + N) + 1], 1);
                close(fd2[2 * (i + N) + 1]);
                sprintf(stri, "%d", i);
                char *array[] = {exec, stri, NULL};
                execv(name, array);
                fflush(stdout);
                close(1);
                break;
            }
            if (pid > 0)
            { //Parent
                close(fd2[2 * i]);
                //dup2(fd[i][1],1);
                //close(fd[i][1]);
                write(fd2[2 * i + 1], word, strlen(word));
            }
            i++;
            k++;
        }

        if (pid > 0)
        {
            sleep(1);

            for (int j = 0; j < N; j++)
            {
                close(fd2[2 * j + 1]);
            }
            for (int j = 2 * N + 1; j < 2 * (3 * N - 1); j++)
            {
                close(fd2[j]);
            }
            wait(NULL);
        }
    }
    else
    {
        printf("Wrong number of argument.\n");
    }

    wait(NULL);

    return 0;
}
