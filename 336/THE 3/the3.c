// Furkan DOGAN  - 2098937
// S.Talha KOSAR - 2099190

/*
 
 * We first write Elector Vote to the LCD screen and then we busy waited for RE1 button to pressed and released.
 * After that we started to interrupts and conversion
 * And then we take total voter number from RB6 and RB7
 * After then when RB7 pressed we started voting
 * Every 250 ms we blinked the cursor
 * Every 100 ms we started A/D conversion
 * Every 1s we counted down and write it to upper part of LCD
 * After A/D conversion finished we changed the current candidate on LCD screen
 * We check for time to be 0 or total vote number exceed and stopped the voting
 * And if reset button pressed we started voting again
 
 */
#include <p18cxxx.h>
#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

#define _XTAL_FREQ   40000000

#include "Includes.h"
#include "lcd.h"

unsigned int result; // result of A/D conversion
int firstCount;
int a;
int b;
int count;
int timer0Count;
int timer1Count;
int flag;
int startVotingFlag;
char c1;
char c2;
int timeLeftH;
int timeLeftL;
int candidateL;
int candidateH;
int candidate1, candidate2, candidate3, candidate4, candidate5, candidate6, candidate7, candidate8; //candidated vote numbers
char * currentCandidate; // current candidate name
int blinkFlag;
int totalVote;
int voteFlag;
int max; //maxVote number
int maxCandidate;
int firstVoteFlag;
int sevenSegment;
int timeFlag; //time is over flag
int totalVoteFlag; //total vote exceeded flag

void init();
void show7Segment();
void loop();
void __interrupt(high_priority) high_isr(void);
void __interrupt(low_priority) low_isr(void);
void clear();

// Main Function

void main(void) {
    clear();
    __delay_ms(15);
    __delay_ms(15);
    __delay_ms(15);
    __delay_ms(15);

    InitLCD(); // Initialize LCD in 4bit mode
    TRISE1 = 1; // RE1 as input


    ClearLCDScreen();
    WriteCommandToLCD(0x80);
    WriteStringToLCD(" #Electro Vote# ");
    WriteCommandToLCD(0xC0);
    WriteStringToLCD(" ############## ");
    while (1) { // RE1 button pressed and released
        if ((PORTE & 0x02) == 2) {
            while (1) {
                if ((PORTE & 0x02) == 0) {
                    break;
                }

            }
            break;
        }
    }
    init();
    loop(); // loop for 3 seconds
    WriteCommandToLCD(0x80);
    WriteStringToLCD(" #Enter Voters# ");
    WriteCommandToLCD(0xC0);
    WriteStringToLCD(" 00             ");
    while (1) {
        if (startVotingFlag) { // voting started
            break;
        }
    }


    T1CONbits.TMR1ON = 1; // start timer 0 interrupt
    T0CONbits.TMR0ON = 1; // start timer 0 interrupt


    LATH = 0x0F; // - - - - on 7 segment display
    LATJ6 = 1; //  - - - - on 7 segment display

    while (1) {
        if (0 == a * 10 + b) {
            candidateL = 0;
            candidateH = 0;
            timeFlag = 1;
            totalVoteFlag = 1;
        }
        show7Segment();
        if (voteFlag && !timeFlag) {
            if (!RB7) {
                firstVoteFlag = 1;
                voteFlag = 0;
                totalVote++;
                if (totalVote == a * 10 + b) {
                    timeFlag = 1;
                    totalVoteFlag = 1;
                }

                if (strncmp(currentCandidate, "18 Poe    :", 11) == 0) {
                    candidate1++;
                    if (candidate1 > max) {
                        max = candidate1;
                        maxCandidate = 18;
                    }
                    candidateL = candidate1 % 10;
                    candidateH = (candidate1 - candidate1 % 10) / 10;
                } else if (strncmp(currentCandidate, "64 Galib  :", 11) == 0) {
                    candidate2++;
                    if (candidate2 > max) {
                        max = candidate2;
                        maxCandidate = 64;
                    }
                    candidateL = candidate2 % 10;
                    candidateH = (candidate2 - candidate2 % 10) / 10;
                } else if (strncmp(currentCandidate, "34 Selimi :", 11) == 0) {
                    candidate3++;
                    if (candidate3 > max) {
                        max = candidate3;
                        maxCandidate = 34;
                    }
                    candidateL = candidate3 % 10;
                    candidateH = (candidate3 - candidate3 % 10) / 10;
                } else if (strncmp(currentCandidate, "23 Nesimi :", 11) == 0) {
                    candidate4++;
                    if (candidate4 > max) {
                        max = candidate4;
                        maxCandidate = 23;
                    }
                    candidateL = candidate4 % 10;
                    candidateH = (candidate4 - candidate4 % 10) / 10;

                } else if (strncmp(currentCandidate, "33 Hatayi :", 11) == 0) {
                    candidate5++;
                    if (candidate5 > max) {
                        max = candidate5;
                        maxCandidate = 33;
                    }
                    candidateL = candidate5 % 10;
                    candidateH = (candidate5 - candidate5 % 10) / 10;

                } else if (strncmp(currentCandidate, "67 Zweig  :", 11) == 0) {
                    candidate6++;
                    if (candidate6 > max) {
                        max = candidate6;
                        maxCandidate = 67;
                    }
                    candidateL = candidate6 % 10;
                    candidateH = (candidate6 - candidate6 % 10) / 10;

                } else if (strncmp(currentCandidate, "63 Nabi   :", 11) == 0) {
                    candidate7++;
                    if (candidate7 > max) {
                        max = candidate7;
                        maxCandidate = 63;
                    }
                    candidateL = candidate7 % 10;
                    candidateH = (candidate7 - candidate7 % 10) / 10;

                } else if (strncmp(currentCandidate, "99 Austen :", 11) == 0) {
                    candidate8++;
                    if (candidate8 > max) {
                        max = candidate8;
                        maxCandidate = 99;
                    }
                    candidateL = candidate8 % 10;
                    candidateH = (candidate8 - candidate8 % 10) / 10;
                }
            }
        }
    }
}

/*
 In this function we set the values of registers we enabled TMR0,TMR1 and we set the ADCON registers for A/D convertion. We also gave the inital values to the timers.
 */
void init() {
    TRISB = 0xC0; //  RB6 and RB7 as input
    TRISH = 0;
    TRISH4 = 1; // RH4 as input to use potentiometer
    TRISJ = 0; //J as output
    RCONbits.IPEN = 1;
    INTCON = 0x08;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.TMR0IE = 1;
    PIE1bits.TMR1IE = 1;
    T0CON = 01000101;
    T1CONbits.RD16 = 0;
    T1CONbits.T1CKPS1 = 1;
    T1CONbits.T1CKPS0 = 1;
    T1CONbits.TMR1CS = 0;
    IPR1bits.TMR1IP = 0;
    IPR1bits.ADIP = 0;
    PIR1bits.ADIF = 0;
    TMR1H = 0x0B;
    TMR1L = 0xDC;
    TMR0L = 0x64;
    ADCON0 = 0b00110001;
    ADCON1 = 0b00000000;
    ADCON2 = 0b10001111;

}

/*
 This function shows the winner candidate it shows on the 7 segment display.
 */
void show7Segment() {
    if (firstVoteFlag) {
        LATH = LATH & 0xF0;

        if ((maxCandidate - maxCandidate % 10) / 10 == 0) {
            LATJ = 0b00111111;
        } else if ((maxCandidate - maxCandidate % 10) / 10 == 1) {
            LATJ = 0b00000110;
        } else if ((maxCandidate - maxCandidate % 10) / 10 == 2) {
            LATJ = 0b01011011;
        } else if ((maxCandidate - maxCandidate % 10) / 10 == 3) {
            LATJ = 0b01001111;
        } else if ((maxCandidate - maxCandidate % 10) / 10 == 4) {
            LATJ = 0b01100110;
        } else if ((maxCandidate - maxCandidate % 10) / 10 == 5) {
            LATJ = 0b01101101;
        } else if ((maxCandidate - maxCandidate % 10) / 10 == 6) {
            LATJ = 0b01111101;
        } else if ((maxCandidate - maxCandidate % 10) / 10 == 7) {
            LATJ = 0b00000111;
        } else if ((maxCandidate - maxCandidate % 10) / 10 == 8) {
            LATJ = 0b01111111;
        } else if ((maxCandidate - maxCandidate % 10) / 10 == 9) {
            LATJ = 0b01100111;
        }
        LATH = 0x01;

        __delay_us(100);

        LATH = LATH & 0xF0;

        if (maxCandidate % 10 == 0) {
            LATJ = 0b00111111;
        } else if (maxCandidate % 10 == 1) {
            LATJ = 0b00000110;
        } else if (maxCandidate % 10 == 2) {
            LATJ = 0b01011011;
        } else if (maxCandidate % 10 == 3) {
            LATJ = 0b01001111;
        } else if (maxCandidate % 10 == 4) {
            LATJ = 0b01100110;
        } else if (maxCandidate % 10 == 5) {
            LATJ = 0b01101101;
        } else if (maxCandidate % 10 == 6) {
            LATJ = 0b01111101;
        } else if (maxCandidate % 10 == 7) {
            LATJ = 0b00000111;
        } else if (maxCandidate % 10 == 8) {
            LATJ = 0b01111111;
        } else if (maxCandidate % 10 == 9) {
            LATJ = 0b01100111;
        }
        LATH = 0x02;

        __delay_us(100);

        LATH = LATH & 0xF0;

        if ((max - max % 10) / 10 == 0) {
            LATJ = 0b00111111;
        } else if ((max - max % 10) / 10 == 1) {
            LATJ = 0b00000110;
        } else if ((max - max % 10) / 10 == 2) {
            LATJ = 0b01011011;
        } else if ((max - max % 10) / 10 == 3) {
            LATJ = 0b01001111;
        } else if ((max - max % 10) / 10 == 4) {
            LATJ = 0b01100110;
        } else if ((max - max % 10) / 10 == 5) {
            LATJ = 0b01101101;
        } else if ((max - max % 10) / 10 == 6) {
            LATJ = 0b01111101;
        } else if ((max - max % 10) / 10 == 7) {
            LATJ = 0b00000111;
        } else if ((max - max % 10) / 10 == 8) {
            LATJ = 0b01111111;
        } else if ((max - max % 10) / 10 == 9) {
            LATJ = 0b01100111;
        }
        LATH = 0x04;

        __delay_us(100);

        LATH = LATH & 0xF0;

        if (max % 10 == 0) {
            LATJ = 0b00111111;
        } else if (max % 10 == 1) {
            LATJ = 0b00000110;
        } else if (max % 10 == 2) {
            LATJ = 0b01011011;
        } else if (max % 10 == 3) {
            LATJ = 0b01001111;
        } else if (max % 10 == 4) {
            LATJ = 0b01100110;
        } else if (max % 10 == 5) {
            LATJ = 0b01101101;
        } else if (max % 10 == 6) {
            LATJ = 0b01111101;
        } else if (max % 10 == 7) {
            LATJ = 0b00000111;
        } else if (max % 10 == 8) {
            LATJ = 0b01111111;
        } else if (max % 10 == 9) {
            LATJ = 0b01100111;
        }
        LATH = 0x08;

        __delay_us(100);

        LATH = LATH & 0xF0;
    }
}

/*
 3s generator loop
 */
void loop() {
    for (int i = 0; i < 138; i++) {
        for (int i = 0; i < 138; i++) {
            for (int i = 0; i < 138; i++) {

            }
        }
    }
}

void __interrupt(high_priority) high_isr(void) {
    // RB button pressed interrupt
    if (INTCONbits.RBIF) {
        show7Segment();
        // RB6 pressed
        if ((PORTB & 0x40) == 0x40 && !voteFlag) {
            if (!flag) {
                a = a + 1;
                if (a == 10) {
                    a = 0;
                }
                WriteCommandToLCD(0xC0);
                c1 = " ";
                WriteDataToLCD(c1);
                c1 = (char) (((int) '0') + a);
                WriteDataToLCD(c1);
            } else {
                b = b + 1;
                if (b == 10) {
                    b = 0;
                }
                WriteCommandToLCD(0xC0);
                c1 = " ";
                WriteDataToLCD(c1);
                c1 = (char) (((int) '0') + a);
                WriteDataToLCD(c1);
                c1 = (char) (((int) '0') + b);
                WriteDataToLCD(c1);
            }
        }
        //RB7 pressed
        if ((PORTB & 0x80) == 0x80) {
            count++;
            if (!count) {
                flag = 1;
                PIE1bits.ADIE = 1; //A/D convertion started
            } else if (count == 1) {
                GO_DONE = 1;
                startVotingFlag = 1; //voting started
            } else {
                voteFlag = 1; //voted
            }
        }
        INTCONbits.RBIF = 0; // clear flag bit

    }
    // TMR0 interrupt for 1ms
    if (INTCONbits.TMR0IF) {
        show7Segment();
        TMR0L = 0x64;
        timer0Count++;
        // blink
        if (timer0Count == 250) {
            if (!blinkFlag && !timeFlag) {
                blinkFlag = 1;
                WriteCommandToLCD(0xC0);
                WriteStringToLCD("  ");
                WriteStringToLCD(currentCandidate);
                c2 = (char) (((int) '0') + candidateH);
                WriteDataToLCD(c2);
                c2 = (char) (((int) '0') + candidateL);
                WriteDataToLCD(c2);
                c2 = " ";
                WriteDataToLCD(c2);
            } else {
                blinkFlag = 0;
                WriteCommandToLCD(0xC0);
                WriteStringToLCD(" >");
                WriteStringToLCD(currentCandidate);
                c2 = (char) (((int) '0') + candidateH);
                WriteDataToLCD(c2);
                c2 = (char) (((int) '0') + candidateL);
                WriteDataToLCD(c2);
                c2 = " ";
                WriteDataToLCD(c2);
            }

            timer0Count = 0;
        }
        // A/D convertion started
        if (timer0Count == 100) {
            GO_DONE = 1;
        }


        INTCONbits.TMR0IF = 0; //clear flag bit
    }

}

void __interrupt(low_priority) low_isr(void) {

    INTCONbits.TMR0IE = 0;
    show7Segment();
    // TMR1 overflow interrupt
    if (PIR1bits.TMR1IF) {
        timer1Count++;
        // 1s countdown
        if (timer1Count == 20 && !totalVoteFlag && !timeFlag) {
            TMR1H = 0x0B;
            TMR1L = 0xDC;
            timeLeftL--;
            if (timeLeftL == -1) {
                timeLeftH--;
                if (timeLeftH == -1) {
                    timeLeftH = 0;
                    timeLeftL = 0;
                    timeFlag = 1;
                } else {
                    timeLeftL = 9;
                }
            }
            WriteCommandToLCD(0x80);
            WriteStringToLCD("  Time left :");
            c1 = (char) (((int) '0') + timeLeftH);
            WriteDataToLCD(c1);
            c1 = (char) (((int) '0') + timeLeftL);
            WriteDataToLCD(c1);
            timer1Count = 0;
        }
        PIR1bits.TMR1IF = 0; // clear flag bit

    } else if (PIR1bits.ADIF) { //A/D interrupt

        if (!firstCount) {
            firstCount++;
        } else {
            // change current candidate by A/D conversion result
            result = ADRES;
            if (result >= 0 && result < 128) {
                currentCandidate = "18 Poe    :";
                candidateL = candidate1 % 10;
                candidateH = (candidate1 - candidate1 % 10) / 10;
            } else if (result >= 128 && result < 256) {
                currentCandidate = "64 Galib  :";
                candidateL = candidate2 % 10;
                candidateH = (candidate2 - candidate2 % 10) / 10;
            } else if (result >= 256 && result < 384) {
                currentCandidate = "34 Selimi :";
                candidateL = candidate3 % 10;
                candidateH = (candidate3 - candidate3 % 10) / 10;
            } else if (result >= 384 && result < 512) {
                currentCandidate = "23 Nesimi :";
                candidateL = candidate4 % 10;
                candidateH = (candidate4 - candidate4 % 10) / 10;
            } else if (result >= 512 && result < 640) {
                currentCandidate = "33 Hatayi :";
                candidateL = candidate5 % 10;
                candidateH = (candidate5 - candidate5 % 10) / 10;
            } else if (result >= 640 && result < 768) {
                currentCandidate = "67 Zweig  :";
                candidateL = candidate6 % 10;
                candidateH = (candidate6 - candidate6 % 10) / 10;
            } else if (result >= 768 && result < 896) {
                currentCandidate = "63 Nabi   :";
                candidateL = candidate7 % 10;
                candidateH = (candidate7 - candidate7 % 10) / 10;
            } else if (result > 895) {
                currentCandidate = "99 Austen :";
                candidateL = candidate8 % 10;
                candidateH = (candidate8 - candidate8 % 10) / 10;
            }
        }

        PIR1bits.ADIF = 0;
        PIE1bits.ADIE = 1;
        INTCONbits.GIE = 1;
        INTCONbits.PEIE = 1;

    }
    INTCONbits.TMR0IE = 1;
}

/*
 clear all global variables to start from 0 when the reset button pressed
 */
void clear() {
    result = 0;
    firstCount = 0;
    a = 0;
    b = 0;
    count = -1;
    timer0Count = 0;
    timer1Count = 0;
    flag = 0;
    startVotingFlag = 0;
    timeLeftH = 9;
    timeLeftL = 0;
    candidateL = 0;
    candidateH = 0;
    candidate1 = 0;
    candidate2 = 0;
    candidate3 = 0;
    candidate4 = 0;
    candidate5 = 0;
    candidate6 = 0;
    candidate7 = 0;
    candidate8 = 0;
    currentCandidate = "18 Poe    :";
    blinkFlag = 0;
    totalVote = 0;
    voteFlag = 0;
    max = 0;
    maxCandidate = 0;
    firstVoteFlag = 0;
    sevenSegment = 0;
    timeFlag = 0;
    totalVoteFlag = 0;
    LATJ = 0;
    LATH = 0;
    TRISB = 0;
    TRISH = 0;
    TRISJ = 0;
    TRISE = 0;
    RCON = 0;
    INTCON = 0;
    PIR1 = 0;
    PIE1 = 0;
    T0CON = 0;
    T1CON = 0;
    IPR1 = 0;
    TMR1H = 0;
    TMR1L = 0;
    TMR0L = 0;
    ADCON0 = 0;
    ADCON1 = 0;
    ADCON2 = 0;
    T0CON = 0;
    T1CON = 0;
}
