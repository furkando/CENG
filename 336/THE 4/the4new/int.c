#include "common.h"

/**********************************************************************
 * Function you want to call when an IT occurs.
 **********************************************************************/
extern void AddOneTick(void);
/*extern void MyOwnISR(void); */
void InterruptVectorL(void);
void InterruptVectorH(void);

/**********************************************************************
 * General interrupt vector. Do not modify.
 **********************************************************************/
#pragma code IT_vector_low = 0x18
void Interrupt_low_vec(void)
{
  _asm goto InterruptVectorL _endasm
}
#pragma code

#pragma code IT_vector_high = 0x08
void Interrupt_high_vec(void)
{
  _asm goto InterruptVectorH _endasm
}
#pragma code

/**********************************************************************
 * General ISR router. Complete the function core with the if or switch
 * case you need to jump to the function dedicated to the occuring IT.
 * .tmpdata and MATH_DATA are saved automaticaly with C18 v3.
 **********************************************************************/
#pragma code _INTERRUPT_VECTORL = 0x003000
#pragma interruptlow InterruptVectorL
void InterruptVectorL(void)
{
  EnterISR();

  if (RCSTA1bits.OERR)
  {
    RCSTA1bits.CREN = 0;
    RCSTA1bits.CREN = 1;
  }

  if (INTCONbits.TMR0IF == 1)
    AddOneTick();
  /* Here are the other interrupts you would desire to manage */
  if (PIR1bits.TX1IF == 1)
  {
    sendData();
  }
  if (PIR1bits.RC1IF == 1)
  {
    getData();
    PIR1bits.RC1IF = 0; // clear RC1IF flag
  }
  LeaveISR();
}
#pragma code

/* BE CARREFULL : ONLY BSR, WREG AND STATUS REGISTERS ARE SAVED  */
/* DO NOT CALL ANY FUNCTION AND USE PLEASE VERY SIMPLE CODE LIKE */
/* VARIABLE OR FLAG SETTINGS. CHECK THE ASM CODE PRODUCED BY C18 */
/* IN THE LST FILE.                                              */
#pragma code _INTERRUPT_VECTORH = 0x003300
#pragma interrupt InterruptVectorH nosave = FSR0, TBLPTRL, TBLPTRH, TBLPTRU, TABLAT, PCLATH, PCLATU, PROD, section(".tmpdata"), section("MATH_DATA")
void InterruptVectorH(void)
{
  if (INTCONbits.INT0IF == 1)
    INTCONbits.INT0IF = 0;
}
#pragma code

extern void _startup(void);
#pragma code _RESET_INTERRUPT_VECTOR = 0x003400
void _reset(void)
{
  _asm goto _startup _endasm
}
#pragma code

/* End of file : int.c */
