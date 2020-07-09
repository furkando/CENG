#include "common.h"

char nextMove = 'S';
char nextMoveCount = 1;
char robot_x;
char robot_y;
char robot_r;
char keyFlag = 0;
char all_cells[16][4] = {0};

extern char state_system;  // current state of the system; _WAITING or _OPERATING
extern char transmitCount; // index to the transmit_buffer array; the current byte to be transmitted
extern const char pool_string[2][16];
extern char receive_buffer[64];
extern char receive_count;
extern char sendNumber;
extern char received;
extern char alertNumber;
extern char solved;
extern char N[2][64];
extern unsigned char length_zero;

void goDir(char list[], char abcd[])
{

    char l1 = list[0];
    char l2 = list[1];
    char l3 = list[2];
    char l4 = list[3];

    char i = 0;
    if (all_cells[robot_x][robot_y] != 0)
    {
        i = all_cells[robot_x][robot_y] % 4;
    }

    for (; i < 4; i++)
    {
        char dir;

        if (list[i] == 1)
        {
            if (abcd[0] == '1')
                continue;
            dir = 1;
        }

        if (list[i] == 2)
        {
            if (abcd[1] == '1')
                continue;
            dir = 2;
        }

        if (list[i] == 3)
        {
            if (abcd[2] == '1')
                continue;
            dir = 3;
        }

        if (list[i] == 4)
        {
            if (abcd[3] == '1')
                continue;
            dir = 4;
        }

        nextMoveCount = 10;
        switch (dir)
        {
        case 1:
            switch (robot_r)
            {
            case 0:
                nextMove = 'F';
                break;
            case 10:
                nextMove = 'R';
                break;
            case 20:
                nextMove = 'R';
                nextMoveCount = 20;
                break;
            case 30:
                nextMove = 'L';
                break;
            }
            break;

        case 2:
            switch (robot_r)
            {
            case 0:
                nextMove = 'R';
                break;
            case 10:
                nextMove = 'R';
                nextMoveCount = 20;
                break;
            case 20:
                nextMove = 'L';
                break;
            case 30:
                nextMove = 'F';
                break;
            }
            break;

        case 3:
            switch (robot_r)
            {
            case 0:
                nextMove = 'R';
                nextMoveCount = 20;
                break;
            case 10:
                nextMove = 'L';
                break;
            case 20:
                nextMove = 'F';
                break;
            case 30:
                nextMove = 'R';
                break;
            }
            break;

        case 4:
            switch (robot_r)
            {
            case 0:
                nextMove = 'L';
                break;
            case 10:
                nextMove = 'F';
                break;
            case 20:
                nextMove = 'R';
                break;
            case 30:
                nextMove = 'R';
                nextMoveCount = 20;
                break;
            }
            break;
        }
        break;
    }
}

void parseData()
{

    if (receive_buffer[1] == 'G')
    {
        state_system = _ACTIVE;
        SetEvent(TASK1_ID, ACTIVE_EVENT);
    }

    else if (receive_buffer[1] == 'D')
    {
        unsigned char i1 = 2;
        unsigned char i2 = 2;
        unsigned char x1, x0, y1, y0, r1, r0, a, b, c, d, p1, p0;
        char abcd[4];
        char listN[4] = {1, 2, 4, 3}; //  for north: up, right, left, down
        char listE[4] = {2, 3, 1, 3};
        char listS[4] = {3, 2, 4, 1};
        char listW[4] = {4, 3, 1, 2};
        char listNE[4] = {2, 1, 3, 4};
        char listSE[4] = {2, 3, 1, 4};
        char listNW[4] = {1, 4, 2, 3};
        char listSW[4] = {3, 4, 2, 1};

        while (receive_buffer[i2] != ',')
            i2++;
        x0 = receive_buffer[i1];
        robot_x = x0 - '0';
        if (i2 - i1 == 2)
        {
            x1 = receive_buffer[i1 + 1];
            robot_x = robot_x * 10 + (x1 - '0');
        }

        i2++;
        i1 = i2;
        while (receive_buffer[i2] != ',')
            i2++;
        y0 = receive_buffer[i1];
        robot_y = y0 - '0';
        if (i2 - i1 == 2)
        {
            y1 = receive_buffer[i1 + 1];
            robot_y = robot_y * 10 + (y1 - '0');
        }

        i2++;
        i1 = i2;
        while (receive_buffer[i2] != ',')
            i2++;
        r0 = receive_buffer[i1];
        robot_r = r0 - '0';
        if (i2 - i1 == 2)
        {
            r1 = receive_buffer[i1 + 1];
            robot_r = robot_r * 10 + (r1 - '0');
        }

        i2++;
        i1 = 0;
        abcd[i1++] = receive_buffer[i2++];
        abcd[i1++] = receive_buffer[i2++];
        abcd[i1++] = receive_buffer[i2++];
        abcd[i1++] = receive_buffer[i2++];

        i1 = i2;
        while (receive_buffer[i2] != ':')
            i2++;
        p0 = receive_buffer[i1];
        if (i2 - i1 == 2)
            p1 = receive_buffer[i1 + 1];

        if (p0 == 'K')
        {
            nextMove = 'P';
            nextMoveCount = 1;
            keyFlag = 1;
        }

        else if (keyFlag)
        { //key has taken
            if (robot_x == 15 && robot_y == 0)
            {
                nextMove = 'S';
                nextMoveCount = 1;
            }
            else if (robot_x == 15)
                goDir(listN, abcd);

            else if (robot_y == 0)
                goDir(listE, abcd);

            else
                goDir(listNE, abcd);
        }

        else if (p0 == 'N')
        {
            if (p1 == 'E')
                goDir(listNE, abcd);
            else if (p1 == 'W')
                goDir(listNW, abcd);
            else
                goDir(listN, abcd);
        }

        else if (p0 == 'S')
        {
            if (p1 == 'E')
                goDir(listSE, abcd);
            else if (p1 == 'W')
                goDir(listSW, abcd);
            else
                goDir(listS, abcd);
        }

        else if (p0 == 'E')
        {
            goDir(listE, abcd);
        }

        else if (p0 == 'W')
        {
            goDir(listW, abcd);
        }

        if (nextMove == 'F' || (nextMove == 'S' && nextMoveCount != 1))
            all_cells[robot_x][robot_y]++; // cell encounter count
    }

    else if (receive_buffer[1] == 'E')
    {
        state_system = _WAITING;
    }
}

/**********************************************************************
 * ------------------------------ TASK0 -------------------------------
 * 
 * Prepares the data to be transmitted
 * 
 **********************************************************************/
TASK(TASK0)
{
    while (1)
    {
        LATEbits.LATE3 = 1;
        PIE1bits.RC1IE = 1;
        if (received)
        {
            received = 0;
            LATEbits.LATE0 = 1;
            parseData();
        }
    }
    TerminateTask();
}

/* End of File : tsk_task0.c */