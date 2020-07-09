// Group 47
// 2098937 Furkan DOGAN
// 2099190 Sinan Talha KOSAR

// 	TASK0: Firstly, the data will receive. After ":", received will be set. Examine the data, check GO command and response
// 	GO command -> Set an event to Task1 to activate.
// 	Sensor Response -> Is next move? Consider priorities
// 	Backtracking is also important to consider. We hold the number
// 	for cells. Shift priorities list.
// 	All in all, movement i.e picking the key and going to the gate worked fine for all of the
// 	possible maps.

// 	TASK1: Every 50ms +- 20ms, send the nextMove as a command. Next move was decided on task0.
// 	Default command is 'S'.

// Priorities:
// 	Ideally, long task -> lower priority vice versa
// 	So that the shorter tasks can preempt the longer tasks.

#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

#include "common.h"

/**********************************************************************
 * Definition dedicated to the local functions.
 **********************************************************************/
#define DEFAULT_MODE 0

/**********************************************************************
 * Function prototypes.
 **********************************************************************/
void main(void);
void Init(void);
void StartupHook(void);
void ShutdownHook(StatusType error);
void ErrorHook(StatusType error);
void PreTaskHook(void);
void PostTaskHook(void);

extern union Timers Tmr0;

AppModeType SelectedMode;

/**********************************************************************
 * -------------------------- main function ---------------------------
 *
 * Setup the different alarms and start the kernel.
 *
 **********************************************************************/
void main(void)
{
	STKPTR = 0;
	SelectedMode = DEFAULT_MODE;
	Init();

	while (1)
	{
		StartOS(SelectedMode);
	}
}

/**********************************************************************
 * Clear all RAM memory and set PORTB to output mode.
 *
 * @return void
 **********************************************************************/
void Init(void)
{
	FSR0H = 0;
	FSR0L = 0;

	/* User setting : actual PIC frequency */
	Tmr0.lt = _40MHZ;

	/* configure I/O ports */
	TRISD = 0; // set all ports of PortD as output
	TRISE = 0; // set all ports of PortE as output
	PORTD = 0; // clear PortD
	PORTE = 0; // clear PortE

	/* Timer OFF - Enabled by Kernel */
	T0CON = 0x08;
	TMR0H = Tmr0.bt[1];
	TMR0L = Tmr0.bt[0];

	/* configure USART transmitter/receiver */
	SPBRG1 = 21;   // for 40 MHz, to have 115200 baud rate, it should be 21
	TXSTA1 = 0x04; // (= 00000100) 8-bit transmit, transmitter NOT enabled,
				   // asynchronous, high speed mode
	RCSTA1 = 0x90; // (= 10010000) 8-bit receiver, receiver enabled,
				   // continuous receive, serial port enabled

	/* configure the interrupts */
	INTCON = 0;			// clear interrupt register completely
	PIE1bits.TX1IE = 1; // enable USART transmit interrupt
	PIE1bits.RC1IE = 1; // enable USART receive interrupt
	PIR1 = 0;			// clear all peripheral flags

	INTCONbits.PEIE = 1; // enable peripheral interrupts
	INTCONbits.GIE = 1;  // globally enable interrupts
}

/**********************************************************************
 * Hook routine called just before entering in kernel.
 *
 * @param error      IN The new error stored in buffer
 * @return error     Error level
 **********************************************************************/
void StartupHook(void)
{
}

/**********************************************************************
 * Hook routine called just after leaving the kernel.
 *
 * @param error      IN The last error detected by OS
 * @return void
 **********************************************************************/
void ShutdownHook(StatusType error)
{
}

/**********************************************************************
 * Store a new error in a global buffer keeping a track of the 
 * application history.
 *
 * @param error      IN The new error stored in buffer
 * @return void
 **********************************************************************/
void ErrorHook(StatusType error)
{
}

/**********************************************************************
 * Hook routine called just before entering in a task.
 *
 * @return void
 **********************************************************************/
void PreTaskHook(void)
{
}

/**********************************************************************
 * Hook routine called just after leaving a task.
 *
 * @return void
 **********************************************************************/
void PostTaskHook(void)
{
}

/* End of File : main.c */
