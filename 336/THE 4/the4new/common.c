#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
char state_system = _WAITING; // state of the system; _WAITING or _OPERATING
char transmitCount;           // index to the transmit_buffer array
char N[2][64];
char receive_buffer[64];
char transmit_buffer[64]; // holds the bytes to be transmitted
char receive_count;
char sendNumber;
char received = 0;
char alertNumber = 0;
char solved = 0;
unsigned char length_zero;

/**********************************************************************
 * ----------------------- GLOBAL FUNCTIONS ---------------------------
 **********************************************************************/
/* transmits data using serial communication */
void sendData()
{
    if (transmitCount < sendNumber)
    {
        TXREG1 = transmit_buffer[transmitCount];
        transmitCount++;
    }
    else
    {                        // all the bytes have been sent
        TXSTA1bits.TXEN = 0; // disable transmitter
    }
}

/* Invoked when receive interrupt occurs; meaning that data is received */
void getData()
{
    unsigned char rxbyte = RCREG1;

    if (rxbyte == '$')
    {
        receive_count = 0;
    }

    receive_buffer[receive_count] = rxbyte;
    receive_count++;

    if (rxbyte == ':')
    {
        received = 1;
    }
}

/* End of File : common.c */
