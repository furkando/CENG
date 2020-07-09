#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int n = 0;
int (*pipes)[2];
char *mapper;
char *reducer;
int fds[2];
int (*mr)[2];
//int(*rr)[2];
int argnum = 0;

void createpipes()
{
	if (argnum == 3)
	{
		for (int i = 0; i < n; i++)
		{

			if ((pipe(pipes[i])) < 0)
			{
				perror("Failed to open pipe");
			}
		}
	}
	else if (argnum == 4)
	{
		for (int i = 0; i < n; i++)
		{
			if ((pipe(pipes[i])) < 0)
			{
				perror("Failed to open pipe");
			}
		}

		for (int i = 0; i < ((2 * n) - 1); i++)
		{

			if ((pipe(mr[i])) < 0)
			{
				perror("Failed to open pipe");
			}
		}
	}
}

void forkla()
{

	char *buffer;
	size_t bufsize = 1024;
	size_t line;
	int lineIndex = 0;
	int pIndex = 0;
	int pID = 0;

	buffer = (char *)malloc(bufsize * sizeof(char));

	if (argnum == 3)
	{
		for (int i = 0; i < n; i++)
		{

			if (buffer == NULL)
				exit(1);

			pID = fork();

			if (pID < 0)
			{

				exit(1);
			}

			else if (pID == 0)
			{
				char str[12];
				sprintf(str, "%d", i);
				dup2(pipes[i][0], 0);
				char *newargv[] = {stdin, str, NULL};
				close(pipes[i][1]);
				execv(mapper, newargv);
				exit(1);
			}
			else
			{
				close(pipes[i][0]);
				while (1)
				{

					line = getline(&buffer, &bufsize, stdin);
					if (line == -1)
					{
						break;
					}
					pIndex = (lineIndex % n);
					write(pipes[pIndex][1], buffer, line);
					lineIndex++;
				}
				close(pipes[i][1]);

				wait(NULL);
			}
		}
	}
	else if (argnum == 4)
	{
		for (int i = 0; i < 2 * n; i++)
		{
			pID = fork();
			if (i < n)
			{
				if (pID < 0)
				{
					exit(1);
				}
				else if (pID == 0)
				{
					for (int j = 0; j < n; j++)
					{
						if (j != i)
						{
							close(pipes[j][0]);
							close(pipes[j][1]);
						}
					}
					for (int j = 0; j < ((2 * n) - 1); j++)
					{
						if (j != i)
						{
							close(mr[j][0]);
							close(mr[j][1]);
						}
					}
					char str[12];
					sprintf(str, "%d", i);
					close(pipes[i][1]);
					dup2(pipes[i][0], 0);
					close(pipes[i][0]);
					close(mr[i][0]);
					dup2(mr[i][1], 1);
					close(mr[i][1]);
					char *newargv[] = {stdin, str, NULL};
					execv(mapper, newargv);
					exit(1);
				}

				else
				{

					close(pipes[i][0]);
					while (1)
					{
						line = getline(&buffer, &bufsize, stdin);
						if (line == -1)
						{
							break;
						}
						pIndex = (lineIndex % n);
						write(pipes[pIndex][1], buffer, line);
						lineIndex++;
					}
					close(pipes[i][1]);
				}
			}
			else
			{
				sleep(2);
				if (pID < 0)
				{
					exit(1);
				}
				else if (pID == 0)
				{
					for (int j = 0; j < n; j++)
					{
						close(pipes[j][0]);
						close(pipes[j][1]);
					}
					for (int j = 0; j < ((2 * n) - 1); j++)
					{
						if (i == n)
						{
							if ((j != i) && (j != (i - n)))
							{
								close(mr[j][0]);
								close(mr[j][1]);
							}
						}
						else if (i == ((2 * n) - 1))
						{
							if ((j != (i - n)) && (j != (i - 1)))
							{
								close(mr[j][0]);
								close(mr[j][1]);
							}
						}
						else
						{
							if (j != (i - n) && (j != i) && (j != (i - 1)))
							{
								close(mr[j][0]);
								close(mr[j][1]);
							}
						}
					}
					char str[50];
					sprintf(str, "%d", (i % n));
					close(mr[i - n][1]);
					dup2(mr[i - n][0], 0);
					close(mr[i - n][0]);
					char *newargv[] = {stdin, str, NULL};
					if (i == n)
					{
						close(mr[i][0]);
						dup2(mr[i][1], 1);
						// close(mr[i][1]);
						execv(reducer, newargv);
					}
					else if (i == ((2 * n) - 1))
					{
						close(mr[i - 1][1]);
						dup2(mr[i - 1][0], 2);
						close(mr[i - 1][0]);
						sleep(1);
						execv(reducer, newargv);
					}
					else
					{
						close(mr[i - 1][1]);
						dup2(mr[i - 1][0], 2);
						close(mr[i - 1][0]);
						close(mr[i][0]);
						dup2(mr[i][1], 1);
						close(mr[i][1]);
						execv(reducer, newargv);
					}
					exit(1);
				}
				else
				{
					// wait(NULL);
				}
			}
		}
		if (pID > 0)
		{
			for (int j = 0; j < (2 * n) - 1; j++)
			{
				close(mr[j][0]);
				close(mr[j][1]);
			}
			for (int j = 0; j < (2 * n); j++)
			{
				wait(NULL);
			}
		}
	}
}

int main(int argc, char **argv)
{

	n = strtol(argv[1], NULL, 10);

	pipes = (int *)malloc(sizeof(int) * n);
	argnum = argc;
	mapper = argv[2];
	if (argc == 3)
	{ // Mapper Process

		createpipes();
		forkla();
	}

	else
	{ // Reducer
		reducer = argv[3];
		mr = (int *)malloc(sizeof(int) * ((2 * n) - 1));
		//rr = (int*)malloc(sizeof(int)*(n-1));
		createpipes();
		forkla();
	}

	return 0;
}