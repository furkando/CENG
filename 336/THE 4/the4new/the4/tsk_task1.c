#include "common.h"

extern char state_system;        // current state of the system; _WAITING or _OPERATING
extern char transmit_buffer[64]; // holds the bytes to be transmitted/displayed.
extern char transmitCount;       // index to the transmit_buffer array; the current byte to be transmitted
extern char receive_count;
extern char sendNumber;
extern char nextMove;
extern char nextMoveCount;

void sendMove()
{
    transmitCount = 0;
    transmit_buffer[0] = '$';
    transmit_buffer[1] = nextMove;
    transmit_buffer[2] = ':';
    sendNumber = 3;
    TXSTA1bits.TXEN = 1;
    if (!--nextMoveCount)
        nextMove = 'S';
}

/**********************************************************************
 * ------------------------------ TASK1 -------------------------------
 *
 * Prepares the data to be transmitted
 *
 **********************************************************************/
TASK(TASK1)
{

    WaitEvent(ACTIVE_EVENT);
    ClearEvent(ACTIVE_EVENT);
    SetRelAlarm(ALARM_TSK1, 50, 50);

    while (1)
    {
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
        PIE1bits.RC1IE = 1;
        sendMove();
    }
    TerminateTask();
}

/* End of File : tsk_task1.c */