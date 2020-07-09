#define _GNU_SOURCE
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include "writeOutput.c"
#include <string>
#include <time.h>
#include <fcntl.h>

using namespace std;

vector<vector<unsigned int> > miners, smelters, foundries, transporters;
vector<sem_t *> minerEmpty;
vector<sem_t *> transPorter;
pthread_mutex_t mutexMiner = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexTrans = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexJo = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexProducer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mumumutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lulumutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tutumutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t smelterMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t smelterMut2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t foundryMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t foundryMut2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t copperMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t coalMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ironMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lastOne = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t smelterCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t foundryCond = PTHREAD_COND_INITIALIZER;

unsigned int allOre = 0, totalProducer = 0;
int cycle = -1;

typedef struct mS
{
    int ID;
    unsigned int oreType;
    unsigned int capacity;
    unsigned int interval;
    unsigned int totalOre;
    unsigned int currentOreCount;
} minerS;

typedef struct tS
{
    int ID;
    unsigned int interval;
    // vector<vector<unsigned int>> miners;
    // vector<vector<unsigned int>> smelters;
    // vector<vector<unsigned int>> foundries;
} transporterS;

typedef struct sS
{
    int ID;
    unsigned int oreType;
    unsigned int capacity;
    unsigned int interval;
} smelterS;

typedef struct fS
{
    int ID;
    unsigned int capacity;
    unsigned int interval;

} foundryS;

OreType oreTyper(unsigned int oreo)
{
    if (oreo == 0)
    {
        return IRON;
    }
    else if (oreo == 1)
    {
        return COPPER;
    }
    else
    {
        return COAL;
    }
}

void *miner(void *input)
{
    int pid = getpid();
    int ID = ((struct mS *)input)->ID;
    unsigned int oreType = ((struct mS *)input)->oreType;
    unsigned int capacity = ((struct mS *)input)->capacity;
    unsigned int interval = ((struct mS *)input)->interval;
    unsigned int totalOre = ((struct mS *)input)->totalOre;
    sem_t *empty;
    char emptyString[100];
    // char fullString[100];
    sprintf(emptyString, "empty-%d-%d", pid, ID);
    // sprintf(fullString, "full-%d-%d", pid, ID);
    // full = sem_open(fullString, O_CREAT, 0644, 0);
    empty = sem_open(emptyString, O_CREAT, 0644, capacity);
    minerEmpty.push_back(empty);
    // transPorter.push_back(full);

    pthread_mutex_unlock(&mutexJo);
    int i = 0;
    MinerInfo minerInfo;
    FillMinerInfo(&minerInfo, ID, oreTyper(oreType), capacity, miners[(ID)-1][5]);
    WriteOutput(&minerInfo, NULL, NULL, NULL, MINER_CREATED);
    while (i != totalOre)
    {
        sem_wait(minerEmpty[(ID)-1]);
        FillMinerInfo(&minerInfo, ID, oreTyper(oreType), capacity, miners[(ID)-1][5]);
        WriteOutput(&minerInfo, NULL, NULL, NULL, MINER_STARTED);
        usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
        pthread_mutex_lock(&mutexMiner);
        allOre++;
        miners[(ID)-1][5]++;
        pthread_mutex_unlock(&mutexMiner);
        i++;
        sem_post(transPorter[0]);
        FillMinerInfo(&minerInfo, ID, oreTyper(oreType), capacity, miners[(ID)-1][5]);
        WriteOutput(&minerInfo, NULL, NULL, NULL, MINER_FINISHED);
        usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
    }
    // miner stopped signal the transporter
    pthread_mutex_lock(&mutexProducer);
    totalProducer--;
    pthread_mutex_unlock(&mutexProducer);
    for (int i = 0; i < transporters.size(); i++)
    {
        sem_post(transPorter[0]);
    }
    FillMinerInfo(&minerInfo, ID, oreTyper(oreType), capacity, miners[(ID)-1][5]);
    WriteOutput(&minerInfo, NULL, NULL, NULL, MINER_STOPPED);
    pthread_exit(NULL); /// check
    return NULL;
}

void *transporter(void *input)
{
    int pid = getpid(), i = 0, flag = 0, smelFlag = 0, oneMore = 0, flagger = 0, twoMore = 0;
    OreType carriedOre;
    int ID = ((struct tS *)input)->ID;
    unsigned int interval = ((struct tS *)input)->interval;
    pthread_mutex_unlock(&mutexJo);
    TransporterInfo transporterInfo;
    MinerInfo minerInfo;
    SmelterInfo smelterInfo;
    FoundryInfo foundryInfo;
    FillTransporterInfo(&transporterInfo, ID, NULL);
    WriteOutput(NULL, &transporterInfo, NULL, NULL, TRANSPORTER_CREATED);
    while (totalProducer || allOre)
    {
        pthread_mutex_lock(&mutexTrans);
        for (i = cycle + 1; i < miners.size(); i++)
        {
            if (miners[i][5] > 0)
            {
                flag = 1;
                flagger = 1;
                FillMinerInfo(&minerInfo, miners[i][0], oreTyper(miners[i][3]), 0, 0);
                FillTransporterInfo(&transporterInfo, ID, NULL);
                WriteOutput(&minerInfo, &transporterInfo, NULL, NULL, TRANSPORTER_TRAVEL);
                usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
                allOre--;
                miners[i][5]--;
                carriedOre = oreTyper(miners[i][3]);
                transporters[ID - 1][2] = miners[i][3];
                FillMinerInfo(&minerInfo, miners[i][0], oreTyper(miners[i][3]), miners[i][2], miners[i][5]);
                FillTransporterInfo(&transporterInfo, ID, &carriedOre);
                WriteOutput(&minerInfo, &transporterInfo, NULL, NULL, TRANSPORTER_TAKE_ORE);
                usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
                sem_post(minerEmpty[i]);
                cycle = i;
                if (cycle + 1 == miners.size())
                {
                    cycle = -1;
                }
                break;
            }
            if (cycle != -1 && i == (miners.size() - 1))
            {
                cycle = -1;
                i = -1;
            }
        }
        pthread_mutex_unlock(&mutexTrans);
        if (flag == 0)
        {
            sem_wait(transPorter[0]);
            continue;
        }
        else
        {
            flag = 0;
            if (transporters[ID - 1][2] == 1) // copper
            {
                pthread_mutex_lock(&copperMut);
                for (int i = 0; i < smelters.size(); i++)
                {
                    if (smelters[i][0] != -1 && smelters[i][4] > 0 && smelters[i][5] > 0 && smelters[i][3] == 1)
                    {
                        FillSmelterInfo(&smelterInfo, smelters[i][0], oreTyper(smelters[i][3]), 0, 0, 0);
                        FillTransporterInfo(&transporterInfo, ID, &carriedOre);
                        WriteOutput(NULL, &transporterInfo, &smelterInfo, NULL, TRANSPORTER_TRAVEL);
                        usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
                        smelters[i][5]--;
                        smelters[i][4]++;
                        FillSmelterInfo(&smelterInfo, smelters[i][0], oreTyper(smelters[i][3]), smelters[i][2], smelters[i][4], smelters[i][6]);
                        FillTransporterInfo(&transporterInfo, ID, &carriedOre);
                        WriteOutput(NULL, &transporterInfo, &smelterInfo, NULL, TRANSPORTER_DROP_ORE);
                        smelFlag = 1;
                        usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
                        pthread_cond_broadcast(&smelterCond);
                        break;
                    }
                }
                if (smelFlag == 0)
                {
                    for (int i = 0; i < smelters.size(); i++)
                    {
                        if (smelters[i][0] != -1 && smelters[i][5] > 0 && smelters[i][3] == 1)
                        {
                            // cout << "aaaa" << endl;
                            FillSmelterInfo(&smelterInfo, smelters[i][0], oreTyper(smelters[i][3]), 0, 0, 0);
                            FillTransporterInfo(&transporterInfo, ID, &carriedOre);
                            WriteOutput(NULL, &transporterInfo, &smelterInfo, NULL, TRANSPORTER_TRAVEL);
                            usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
                            smelters[i][5]--;
                            smelters[i][4]++;
                            FillSmelterInfo(&smelterInfo, smelters[i][0], oreTyper(smelters[i][3]), smelters[i][2], smelters[i][4], smelters[i][6]);
                            FillTransporterInfo(&transporterInfo, ID, &carriedOre);
                            WriteOutput(NULL, &transporterInfo, &smelterInfo, NULL, TRANSPORTER_DROP_ORE);
                            smelFlag = 1;
                            usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
                            pthread_cond_broadcast(&smelterCond);
                            break;
                        }
                    }
                    if (smelFlag == 0)
                    {
                        for (int i = 0; i < smelters.size(); i++)
                        {
                            if (smelters[i][0] != -1)
                            {
                                oneMore = 1;
                                break;
                            }
                        }
                        if (oneMore)
                        {
                            oneMore = 0;
                            sem_wait(transPorter[1]); ////// chechk!!!!!
                        }
                    }
                    else
                    {
                        smelFlag = 0;
                    }
                }
                else
                {
                    smelFlag = 0;
                }
                pthread_mutex_unlock(&copperMut);
            }
            else if (transporters[ID - 1][2] == 2) // coal
            {
                pthread_mutex_lock(&coalMut);
                for (int i = 0; i < foundries.size(); i++)
                {
                    if (foundries[i][0] != -1 && foundries[i][3] > 0 && foundries[i][5] > 0)
                    {
                        FillFoundryInfo(&foundryInfo, foundries[i][0], oreTyper(foundries[i][3]), 0, 0, 0);
                        FillTransporterInfo(&transporterInfo, ID, &carriedOre);
                        WriteOutput(NULL, &transporterInfo, NULL, &foundryInfo, TRANSPORTER_TRAVEL);
                        usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
                        foundries[i][5]--;
                        foundries[i][4]++;
                        FillFoundryInfo(&foundryInfo, foundries[i][0], foundries[i][2], foundries[i][3], foundries[i][4], foundries[i][6]);
                        FillTransporterInfo(&transporterInfo, ID, &carriedOre);
                        WriteOutput(NULL, &transporterInfo, NULL, &foundryInfo, TRANSPORTER_DROP_ORE);
                        smelFlag = 1;
                        usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
                        // cout << "aaa" << endl;
                        pthread_cond_broadcast(&foundryCond);
                        break;
                    }
                }
                if (smelFlag == 0)
                {
                    for (int i = 0; i < foundries.size(); i++)
                    {
                        if (foundries[i][0] != -1 && foundries[i][5] > 0)
                        {
                            // cout << "aaaa" << endl;
                            FillFoundryInfo(&foundryInfo, foundries[i][0], oreTyper(foundries[i][3]), 0, 0, 0);
                            FillTransporterInfo(&transporterInfo, ID, &carriedOre);
                            WriteOutput(NULL, &transporterInfo, NULL, &foundryInfo, TRANSPORTER_TRAVEL);
                            usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
                            foundries[i][5]--;
                            foundries[i][4]++;
                            FillFoundryInfo(&foundryInfo, foundries[i][0], foundries[i][2], foundries[i][3], foundries[i][4], foundries[i][6]);
                            FillTransporterInfo(&transporterInfo, ID, &carriedOre);
                            WriteOutput(NULL, &transporterInfo, NULL, &foundryInfo, TRANSPORTER_DROP_ORE);
                            smelFlag = 1;
                            usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
                            // cout << "bbb" << endl;
                            pthread_cond_broadcast(&foundryCond);
                            break;
                        }
                    }
                    if (smelFlag == 0)
                    {
                        for (int i = 0; i < foundries.size(); i++)
                        {
                            if (foundries[i][0] != -1)
                            {
                                oneMore = 1;
                                break;
                            }
                        }
                        if (oneMore)
                        {
                            oneMore = 0;
                            pthread_mutex_lock(&lastOne);
                            pthread_mutex_lock(&lastOne);
                        }
                    }
                    else
                    {
                        smelFlag = 0;
                    }
                }
                else
                {
                    smelFlag = 0;
                }
                pthread_mutex_unlock(&coalMut);
            }
            else //iron
            {
                pthread_mutex_lock(&ironMut);
                for (int i = 0; i < smelters.size(); i++)
                {
                    if (smelters[i][0] != -1 && smelters[i][4] > 0 && smelters[i][5] > 0 && smelters[i][3] == 0)
                    {
                        FillSmelterInfo(&smelterInfo, smelters[i][0], oreTyper(smelters[i][3]), 0, 0, 0);
                        FillTransporterInfo(&transporterInfo, ID, &carriedOre);
                        WriteOutput(NULL, &transporterInfo, &smelterInfo, NULL, TRANSPORTER_TRAVEL);
                        usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
                        smelters[i][5]--;
                        smelters[i][4]++;
                        FillSmelterInfo(&smelterInfo, smelters[i][0], oreTyper(smelters[i][3]), smelters[i][2], smelters[i][4], smelters[i][6]);
                        FillTransporterInfo(&transporterInfo, ID, &carriedOre);
                        WriteOutput(NULL, &transporterInfo, &smelterInfo, NULL, TRANSPORTER_DROP_ORE);
                        smelFlag = 1;
                        usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
                        pthread_cond_broadcast(&smelterCond);
                        break;
                    }
                }
                if (smelFlag == 0)
                {
                    for (int i = 0; i < foundries.size(); i++)
                    {
                        if (foundries[i][0] != -1 && foundries[i][4] > 0 && foundries[i][5] > 0)
                        {
                            FillFoundryInfo(&foundryInfo, foundries[i][0], oreTyper(foundries[i][3]), 0, 0, 0);
                            FillTransporterInfo(&transporterInfo, ID, &carriedOre);
                            WriteOutput(NULL, &transporterInfo, NULL, &foundryInfo, TRANSPORTER_TRAVEL);
                            usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
                            foundries[i][5]--;
                            foundries[i][3]++;
                            FillFoundryInfo(&foundryInfo, foundries[i][0], foundries[i][2], foundries[i][3], foundries[i][4], foundries[i][6]);
                            FillTransporterInfo(&transporterInfo, ID, &carriedOre);
                            WriteOutput(NULL, &transporterInfo, NULL, &foundryInfo, TRANSPORTER_DROP_ORE);
                            smelFlag = 1;
                            usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
                            // cout << "ccc" << endl;
                            pthread_cond_broadcast(&foundryCond);
                            break;
                        }
                    }
                    if (smelFlag == 0)
                    {
                        for (int i = 0; i < foundries.size(); i++)
                        {
                            if (foundries[i][0] != -1 && foundries[i][5] > 0)
                            {
                                // cout << "aaaa" << endl;
                                FillFoundryInfo(&foundryInfo, foundries[i][0], oreTyper(foundries[i][3]), 0, 0, 0);
                                FillTransporterInfo(&transporterInfo, ID, &carriedOre);
                                WriteOutput(NULL, &transporterInfo, NULL, &foundryInfo, TRANSPORTER_TRAVEL);
                                usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
                                foundries[i][5]--;
                                foundries[i][3]++;
                                FillFoundryInfo(&foundryInfo, foundries[i][0], foundries[i][2], foundries[i][3], foundries[i][4], foundries[i][6]);
                                FillTransporterInfo(&transporterInfo, ID, &carriedOre);
                                WriteOutput(NULL, &transporterInfo, NULL, &foundryInfo, TRANSPORTER_DROP_ORE);
                                smelFlag = 1;
                                usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
                                // cout << "ddd" << endl;
                                pthread_cond_broadcast(&foundryCond);
                                break;
                            }
                        }
                        if (smelFlag == 0)
                        {
                            for (int i = 0; i < smelters.size(); i++)
                            {
                                if (smelters[i][0] != -1 && smelters[i][5] > 0 && smelters[i][3] == 0)
                                {
                                    // cout << "aaaa" << endl;
                                    FillSmelterInfo(&smelterInfo, smelters[i][0], oreTyper(smelters[i][3]), 0, 0, 0);
                                    FillTransporterInfo(&transporterInfo, ID, &carriedOre);
                                    WriteOutput(NULL, &transporterInfo, &smelterInfo, NULL, TRANSPORTER_TRAVEL);
                                    usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
                                    smelters[i][5]--;
                                    smelters[i][4]++;
                                    FillSmelterInfo(&smelterInfo, smelters[i][0], oreTyper(smelters[i][3]), smelters[i][2], smelters[i][4], smelters[i][6]);
                                    FillTransporterInfo(&transporterInfo, ID, &carriedOre);
                                    WriteOutput(NULL, &transporterInfo, &smelterInfo, NULL, TRANSPORTER_DROP_ORE);
                                    smelFlag = 1;
                                    usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
                                    pthread_cond_broadcast(&smelterCond);
                                    break;
                                }
                            }
                            if (smelFlag == 0)
                            {
                                for (int i = 0; i < smelters.size(); i++)
                                {
                                    if (smelters[i][0] != -1)
                                    {
                                        oneMore = 1;
                                        break;
                                    }
                                }
                                for (int i = 0; i < foundries.size(); i++)
                                {
                                    if (foundries[i][0] != -1)
                                    {
                                        twoMore = 1;
                                        break;
                                    }
                                }
                                if (oneMore)
                                {
                                    oneMore = 0;
                                    sem_wait(transPorter[1]);
                                }
                                else if (twoMore)
                                {
                                    oneMore = 0;
                                    pthread_mutex_lock(&lastOne);
                                    pthread_mutex_lock(&lastOne);
                                }
                            }
                            else
                            {
                                smelFlag = 0;
                            }
                        }
                        else
                        {
                            smelFlag = 0;
                        }
                    }
                    else
                    {
                        smelFlag = 0;
                    }
                }
                else
                {
                    smelFlag = 0;
                }
                pthread_mutex_unlock(&ironMut);
            }
        }
    }
    // cout << totalProducer << allOre << endl;
    pthread_mutex_lock(&tutumutex);

    FillTransporterInfo(&transporterInfo, ID, &carriedOre);
    WriteOutput(NULL, &transporterInfo, NULL, NULL, TRANSPORTER_STOPPED);

    for (int i = 0; i < transporters.size(); i++)
    {
        sem_post(transPorter[0]);
    }

    pthread_mutex_unlock(&tutumutex);
    // cout << "zzzz " << endl;
    pthread_exit(NULL);
    return NULL;
}

void *smelter(void *input)
{
    int pid = getpid();
    int ID = ((struct sS *)input)->ID;
    unsigned int oreType = ((struct sS *)input)->oreType;
    unsigned int capacity = ((struct sS *)input)->capacity;
    unsigned int interval = ((struct sS *)input)->interval;
    pthread_mutex_unlock(&mutexJo);
    int producedIngot = 0;
    SmelterInfo smelterInfo;
    FillSmelterInfo(&smelterInfo, ID, oreTyper(oreType), capacity, smelters[ID - 1][4], producedIngot);
    WriteOutput(NULL, NULL, &smelterInfo, NULL, SMELTER_CREATED);
    struct timeval now;
    struct timespec timeout;
    int retcode;
    gettimeofday(&now, NULL);
    timeout.tv_sec = now.tv_sec + 5;
    timeout.tv_nsec = now.tv_usec * 1000;
    retcode = 0;
    while (1)
    {
        pthread_mutex_lock(&smelterMut);
        while (smelters[ID - 1][4] < 2 && retcode != ETIMEDOUT)
        {
            retcode = pthread_cond_timedwait(&smelterCond, &smelterMut, &timeout);
            // if (retcode != ETIMEDOUT)
            // {
            //     sem_wait(transPorter[1]);
            // }
        }
        if (retcode == ETIMEDOUT)
        {
            pthread_mutex_unlock(&smelterMut);
            break;
        }
        else
        {
            pthread_mutex_lock(&smelterMut2);
            smelters[ID - 1][4] -= 2;
            FillSmelterInfo(&smelterInfo, ID, oreTyper(oreType), capacity, smelters[ID - 1][4], producedIngot);
            WriteOutput(NULL, NULL, &smelterInfo, NULL, SMELTER_STARTED);
            usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
            producedIngot++;
            smelters[ID - 1][6]++;
            smelters[ID - 1][5] += 2; //// check
            sem_post(transPorter[1]); //signal transporters
            sem_post(transPorter[1]); //signal transporters
            FillSmelterInfo(&smelterInfo, ID, oreTyper(oreType), capacity, smelters[ID - 1][4], producedIngot);
            WriteOutput(NULL, NULL, &smelterInfo, NULL, SMELTER_FINISHED);
            pthread_mutex_unlock(&smelterMut2);
            pthread_mutex_unlock(&smelterMut);
        }
    }
    pthread_mutex_lock(&mumumutex);
    smelters[ID - 1][0] = -1; // smelter stopped
    FillSmelterInfo(&smelterInfo, ID, oreTyper(oreType), capacity, smelters[ID - 1][4], producedIngot);
    WriteOutput(NULL, NULL, &smelterInfo, NULL, SMELTER_STOPPED);
    for (int i = 0; i < transporters.size(); i++)
    {
        sem_post(transPorter[1]);
    }

    pthread_mutex_unlock(&mumumutex);
    pthread_exit(NULL);
    return NULL;
}

void *foundry(void *input)
{
    int pid = getpid();
    int ID = ((struct fS *)input)->ID;
    unsigned int capacity = ((struct fS *)input)->capacity;
    unsigned int interval = ((struct fS *)input)->interval;
    pthread_mutex_unlock(&mutexJo);
    int producedIngot = 0;
    FoundryInfo foundryInfo;
    FillFoundryInfo(&foundryInfo, ID, capacity, foundries[ID - 1][3], foundries[ID - 1][4], producedIngot);
    WriteOutput(NULL, NULL, NULL, &foundryInfo, FOUNDRY_CREATED);
    struct timeval now;
    struct timespec timeout;
    int retcode;
    gettimeofday(&now, 0);
    timeout.tv_sec = now.tv_sec + 5;
    timeout.tv_nsec = now.tv_usec * 1000;
    retcode = 0;
    while (1)
    {
        pthread_mutex_lock(&foundryMut);
        while (((foundries[ID - 1][3] < 1) || (foundries[ID - 1][4] < 1)) && retcode != ETIMEDOUT)
        {
            retcode = pthread_cond_timedwait(&foundryCond, &foundryMut, &timeout);
        }
        if (retcode == ETIMEDOUT)
        {
            pthread_mutex_unlock(&foundryMut);
            break;
        }
        else
        {
            pthread_mutex_lock(&foundryMut2);
            foundries[ID - 1][3] -= 1;
            foundries[ID - 1][4] -= 1;
            FillFoundryInfo(&foundryInfo, ID, capacity, foundries[ID - 1][3], foundries[ID - 1][4], producedIngot);
            WriteOutput(NULL, NULL, NULL, &foundryInfo, FOUNDRY_STARTED);
            usleep(interval - (interval * 0.01) + (rand() % (int)(interval * 0.02)));
            producedIngot++;
            foundries[ID - 1][5] += 2;
            foundries[ID - 1][6]++;
            // sem_post(transPorter[2]); //signal transporters
            // sem_post(transPorter[2]); //signal transporters
            pthread_mutex_unlock(&lastOne);
            FillFoundryInfo(&foundryInfo, ID, capacity, foundries[ID - 1][3], foundries[ID - 1][4], producedIngot);
            WriteOutput(NULL, NULL, NULL, &foundryInfo, FOUNDRY_FINISHED);
            pthread_mutex_unlock(&foundryMut2);
            pthread_mutex_unlock(&foundryMut);
        }
    }

    pthread_mutex_lock(&lulumutex);
    foundries[ID - 1][0] = -1; // foundry stopped
    FillFoundryInfo(&foundryInfo, ID, capacity, foundries[ID - 1][3], foundries[ID - 1][4], producedIngot);
    WriteOutput(NULL, NULL, NULL, &foundryInfo, FOUNDRY_STOPPED);
    for (int i = 0; i < transporters.size(); i++)
    {
        pthread_mutex_unlock(&lastOne);
    }
    pthread_mutex_unlock(&lulumutex);
    pthread_exit(NULL);
    return NULL;
}

int main()
{
    // miners.clear();
    // smelters.clear();
    // foundries.clear();
    // transporters.clear();
    // minerEmpty.clear();
    // transPorter.clear();
    int pid = rand(), tid = rand();
    sem_t *full, *smell;
    char fullString[100], smellString[100];
    sprintf(fullString, "transporter-%d-%d", pid, tid);
    sprintf(smellString, "smelter-%d-%d", pid, tid);
    // sprintf(foundryString, "foundry-%d-%d", pid, tid);

    full = sem_open(fullString, O_CREAT, 0644, 0);
    smell = sem_open(smellString, O_CREAT, 0644, 2);
    // found = sem_open(foundryString, O_CREAT, 0644, 2);
    transPorter.push_back(full);
    transPorter.push_back(smell);
    // transPorter.push_back(found);

    unsigned int i = 0;
    minerS minerData;
    transporterS transporterData;
    smelterS smelterData;
    foundryS foundryData;
    unsigned int nM, iM, cM, tM, rM;
    unsigned int nT, iT;
    unsigned int nS, iS, cS, tS;
    unsigned int nF, iF, cF;
    vector<unsigned int> littleM, littleS, littleF, littleT;
    cin >> nM;
    totalProducer = nM;
    for (i = 0; i < nM; i++)
    {

        littleM.push_back(i + 1);
        cin >> iM;
        littleM.push_back(iM);
        cin >> cM;
        littleM.push_back(cM);
        cin >> tM;
        littleM.push_back(tM);
        cin >> rM;
        littleM.push_back(rM);
        littleM.push_back(0);
        miners.push_back(littleM);
        littleM.clear();
    }

    cin >> nT;

    for (i = 0; i < nT; i++)
    {
        littleT.push_back(i + 1);
        cin >> iT;
        littleT.push_back(iT);
        littleT.push_back(3);
        transporters.push_back(littleT);
        littleT.clear();
    }

    cin >> nS;
    for (i = 0; i < nS; i++)
    {
        littleS.push_back(i + 1);
        cin >> iS;
        littleS.push_back(iS);
        cin >> cS;
        littleS.push_back(cS);
        cin >> tS;
        littleS.push_back(tS);
        littleS.push_back(0);  // waiting ore count 4
        littleS.push_back(cS); //available capacity 5
        littleS.push_back(0);  //total produce 6
        smelters.push_back(littleS);
        littleS.clear();
    }

    cin >> nF;
    for (i = 0; i < nF; i++)
    {
        littleF.push_back(i + 1);
        cin >> iF;
        littleF.push_back(iF);
        cin >> cF;
        littleF.push_back(cF);
        littleF.push_back(0);  // waiting iron 3
        littleF.push_back(0);  // waiting coal 4
        littleF.push_back(cF); // avaliable capacity 5
        littleF.push_back(0);  // total produce 6
        foundries.push_back(littleF);
        littleF.clear();
    }
    pthread_t mtID[nM], stID[nS], ftID[nF], ttID[nT];
    InitWriteOutput();

    for (i = 0; i < nM; i++)
    {
        pthread_mutex_lock(&mutexJo);
        minerData.ID = miners[i][0];
        minerData.interval = miners[i][1];
        minerData.capacity = miners[i][2];
        minerData.oreType = miners[i][3];
        minerData.totalOre = miners[i][4];
        minerData.currentOreCount = miners[i][5];
        if (pthread_create(&mtID[i], NULL, miner, &minerData) != 0)
        {
            fprintf(stderr, "error: Cannot create thread");
        }
    }
    for (i = 0; i < nT; i++)
    {
        pthread_mutex_lock(&mutexJo);
        transporterData.ID = transporters[i][0];
        transporterData.interval = transporters[i][1];
        // transporterData.miners = miners;
        // transporterData.smelters = smelters;
        // transporterData.foundries = foundries;
        if (pthread_create(&ttID[i], NULL, transporter, &transporterData) != 0)
        {
            fprintf(stderr, "error: Cannot create thread");
        }
    }
    for (i = 0; i < nS; i++)
    {
        pthread_mutex_lock(&mutexJo);
        smelterData.ID = smelters[i][0];
        smelterData.interval = smelters[i][1];
        smelterData.capacity = smelters[i][2];
        smelterData.oreType = smelters[i][3];
        if (pthread_create(&stID[i], NULL, smelter, &smelterData) != 0)
        {
            fprintf(stderr, "error: Cannot create thread");
        }
    }
    for (i = 0; i < nF; i++)
    {
        pthread_mutex_lock(&mutexJo);
        foundryData.ID = foundries[i][0];
        foundryData.interval = foundries[i][1];
        foundryData.capacity = foundries[i][2];
        if (pthread_create(&ftID[i], NULL, foundry, &foundryData) != 0)
        {
            fprintf(stderr, "error: Cannot create thread");
        }
    }

    for (i = 0; i < nM; i++)
    {
        pthread_join(mtID[i], NULL);
    }
    for (i = 0; i < nT; i++)
    {
        pthread_join(ttID[i], NULL);
    }
    for (i = 0; i < nS; i++)
    {
        pthread_join(stID[i], NULL);
    }
    for (i = 0; i < nF; i++)
    {
        pthread_join(ftID[i], NULL);
    }
}