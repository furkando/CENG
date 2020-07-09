
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include "writeOutput.c"
#include <semaphore.h>

// A normal C function that is executed as a thread
// when its name is specified in pthread_create()
pid_t miner_pid, smelter_pid, foundry_pid, transporter_pid;
pthread_t miner_thread_id, smelter_thread_id, foundry_thread_id, transporter_thread_id;
int miner_count, smelter_count, foundry_count, transporter_count, id_miner = 1, id_smelter = 1, id_foundry = 1, id_transporter = 1;
pthread_mutex_t lockm = PTHREAD_MUTEX_INITIALIZER, locks = PTHREAD_MUTEX_INITIALIZER, lockfo = PTHREAD_MUTEX_INITIALIZER, lockt = PTHREAD_MUTEX_INITIALIZER;
void *myThreadFun(void *vargp)
{
    int *check = vargp;

    pthread_mutex_lock(&lockt);
    printf("arg:%d\n", *check);
    if (*check == 1)
    {

        //printf("MINER Printing pid:%d parent:%d from Thread id:%lu \n",getpid(),getppid(),(int unsigned long)pthread_self());
        MinerInfo minerInfo;
        FillMinerInfo(&minerInfo, id_miner, IRON, 3, 0);
        WriteOutput(&minerInfo, NULL, NULL, NULL, MINER_CREATED);
        id_miner++;
    }
    else if (*check == 2)
    {

        //printf("SMELTER Printing pid:%d parent:%d from Thread id:%lu \n",getpid(),getppid(),(int unsigned long)pthread_self());
        SmelterInfo smelterInfo;
        FillSmelterInfo(&smelterInfo, id_smelter, IRON, 3, 1, 0);
        WriteOutput(NULL, NULL, &smelterInfo, NULL, SMELTER_CREATED);
        id_smelter++;
    }
    else if (*check == 3)
    {

        //printf("FOUNDRY Printing pid:%d parent:%d from Thread id:%lu \n",getpid(),getppid(),(int unsigned long)pthread_self());
        FoundryInfo foundryInfo;
        FillFoundryInfo(&foundryInfo, id_foundry, 3, 2, 1, 0);
        WriteOutput(NULL, NULL, NULL, &foundryInfo, FOUNDRY_CREATED);
        id_foundry++;
    }
    else if (*check == 4)
    {

        //printf("TRANSPORTER Printing pid:%d parent:%d from Thread id:%lu \n",getpid(),getppid(),(int unsigned long)pthread_self());
        TransporterInfo transporterInfo;
        OreType oretype;
        FillTransporterInfo(&transporterInfo, id_transporter, &oretype);
        WriteOutput(NULL, &transporterInfo, NULL, NULL, TRANSPORTER_CREATED);
        id_transporter++;
    }
    pthread_mutex_unlock(&lockt);

    return NULL;
}
void processCreator(pid_t main_pid)
{
    printf("Program Starts with pid:%d\n", main_pid);
    if (getpid() == main_pid)
    {
        if (fork() == 0)
        {
            miner_pid = getpid();
            //printf("Hello From Miner: ID:%d, parent:%d\n",miner_pid,getppid());
        }
        else
        {
            wait(NULL);
        }
    }
    if (getpid() == main_pid)
    {
        if (fork() == 0)
        {
            smelter_pid = getpid();
            //printf("Hello From Smelter: ID:%d, parent:%d\n",smelter_pid,getppid());
        }
        else
        {
            wait(NULL);
        }
    }
    if (getpid() == main_pid)
    {
        if (fork() == 0)
        {
            foundry_pid = getpid();
            //printf("Hello From Foundry: ID:%d, parent:%d\n",foundry_pid,getppid());
        }
        else
        {
            wait(NULL);
        }
    }
    if (getpid() == main_pid)
    {
        if (fork() == 0)
        {
            transporter_pid = getpid();
            //printf("Hello From Transporter: ID:%d, parent:%d\n",transporter_pid,getppid());
        }
        else
        {
            wait(NULL);
        }
    }

    if (getpid() == main_pid)
    {

        printf("Program Ends with pid:%d\n", getpid());
    }
    return;
}
void threadCreator(int miner_count, int smelter_count, int foundry_count, int transporter_count)
{
    int *arg;
    int i;
    if (miner_count)
    {
        *arg = 1;
        for (i = 0; i < miner_count; i++)
        {
            InitWriteOutput();
            if (pthread_create(&miner_thread_id, NULL, myThreadFun, arg) != 0)
            {
                fprintf(stderr, "error: Cannot create thread");
            }
        }
        for (int j = 0; j < i; j++)
        {
            pthread_join(miner_thread_id, NULL);
        }
    }
    if (smelter_count)
    {
        *arg = 2;
        for (i = 0; i < smelter_count; i++)
        {
            InitWriteOutput();
            if (pthread_create(&smelter_thread_id, NULL, myThreadFun, arg) != 0)
            {
                fprintf(stderr, "error: Cannot create thread");
            }
        }
        for (int j = 0; j < i; j++)
        {
            pthread_join(smelter_thread_id, NULL);
        }
    }
    if (foundry_count)
    {
        *arg = 3;
        for (i = 0; i < foundry_count; i++)
        {
            InitWriteOutput();
            if (pthread_create(&foundry_thread_id, NULL, myThreadFun, arg) != 0)
            {
                fprintf(stderr, "error: Cannot create thread");
            }
        }
        for (int j = 0; j < i; j++)
        {
            pthread_join(foundry_thread_id, NULL);
        }
    }
    if (transporter_count)
    {
        *arg = 4;
        for (i = 0; i < transporter_count; i++)
        {
            InitWriteOutput();
            if (pthread_create(&transporter_thread_id, NULL, myThreadFun, arg) != 0)
            {
                fprintf(stderr, "error: Cannot create thread");
            }
        }
        for (int j = 0; j < i; j++)
        {
            pthread_join(transporter_thread_id, NULL);
        }
    }
}
int main()
{
    pid_t main_pid = getpid();
    printf("How many miners?:");
    scanf("%d", &miner_count);
    printf("How many smelters?:");
    scanf("%d", &smelter_count);
    printf("How many foundry?:");
    scanf("%d", &foundry_count);
    printf("How many miners?:");
    scanf("%d", &transporter_count);
    //printf("%d miners\n",miner_count);
    printf("Main with id:%d\n", getpid());
    //processCreator(main_pid);
    threadCreator(miner_count, smelter_count, foundry_count, transporter_count);

    // pthread_t thread_id,thread_id2;
    // printf("Before Thread\n");

    // pthread_create(&thread_id2, NULL, myThreadFun, NULL);
    // pthread_join(thread_id2, NULL);
    // printf("After Thread\n");
    // exit(0);
}