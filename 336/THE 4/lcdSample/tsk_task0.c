#include "define.h"
#include "LCD.h"

/**********************************************************************
 * ----------------------- Local Variables ----------------------------
 **********************************************************************/

extern const char string_pool[2][16];

/**********************************************************************
 * ------------------------------ TASK0 -------------------------------
 * 
 * Writes various strings to LCD 
 * 
 **********************************************************************/
TASK(TASK0)
{
    SetRelAlarm(ALARM_TSK0, 100, 350);

    WaitEvent(ALARM_EVENT);
    ClearEvent(ALARM_EVENT);

    ClearLCDScreen();

    LcdPrintString(string_pool[0], 0, 0);
    LcdPrintString(string_pool[1], 0, 1);
    TerminateTask();
}

/* End of File : tsk_task0.c */