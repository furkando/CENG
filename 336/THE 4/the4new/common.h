#ifndef _COMMON_H
#define COMMON_H

#include "device.h"

/***********************************************************************
 * ------------------------ Timer settings -----------------------------
 **********************************************************************/
#define _10MHZ 63320
#define _16MHZ 61768
#define _20MHZ 60768
#define _32MHZ 57768
#define _40MHZ 55768

/***********************************************************************
 * ----------------------------- Events --------------------------------
 **********************************************************************/
#define ALARM_EVENT 0x80
#define ACTIVE_EVENT 0x40
#define LCD_EVENT 0x10
#define LCD_INITEVENT 0x20
#define DATAREADY_EVENT 0x02
#define ALERT_EVENT 0x01

/***********************************************************************
 * ----------------------------- Task ID -------------------------------
 **********************************************************************/
/* Info about the tasks:
 * TASK0: USART
 * TASK1: USART
 */
#define TASK0_ID 1
#define TASK1_ID 2
#define LCD_ID 4
//#define DRIVERTASK_ID        5

/* Priorities of the tasks */
#define TASK0_PRIO 5
#define TASK1_PRIO 9
#define LCD_PRIO 2
//#define DRIVERTASK_PRIO            1

#define ALARM_TSK0 0
#define ALARM_TSK1 1
#define ALARM_TSK2 2
#define LCD_ALARM_ID 3
//#define DRIVERTASK_ALARM        4

/**********************************************************************
 * ----------------------- GLOBAL DEFINITIONS -------------------------
 **********************************************************************/

/* System States */
#define _WAITING 0 // waiting state
#define _STATE0 1  // operating state for task 0
#define _STATE1 2  // operating state for task 1
#define _ACTIVE 3  // operating state for task 1
#define _FINISHED 4

/**********************************************************************
 * ----------------------- FUNCTION PROTOTYPES ------------------------
 **********************************************************************/
/* transmits data using serial communication */
void sendData();
/* Invoked when receive interrupt occurs; meaning that data is received */
void getData();

#endif

/* End of File : common.h */
