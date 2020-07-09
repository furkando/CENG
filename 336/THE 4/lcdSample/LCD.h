#ifndef __LCD
#define __LCD

// LCD module connections
#define LCD_RS              LATBbits.LATB2;      // RS pin for LCD
#define LCD_E               LATBbits.LATB5;      // Enable pin for LCD
#define LCD_Data_Bus_D4     LATDbits.LATD4;
#define LCD_Data_Bus_D5     LATDbits.LATD5;
#define LCD_Data_Bus_D6     LATDbits.LATD6;
#define LCD_Data_Bus_D7     LATDbits.LATD7;

#define LCD_RS_Dir          TRISB2_bit;
#define LCD_E_Dir           TRISB5_bit;
#define LCD_Data_Bus_Dir_D4 TRISDbits.TRISD4;     // Data bus bit 4
#define LCD_Data_Bus_Dir_D5 TRISDbits.TRISD5;     // Data bus bit 5
#define LCD_Data_Bus_Dir_D6 TRISDbits.TRISD6;     // Data bus bit 6
#define LCD_Data_Bus_Dir_D7 TRISDbits.TRISD7;     // Data bus bit 7
// End LCD module connections

// Constants
#define E_Delay       1


// Function Declarations
void WriteCommandToLCD(unsigned char);
void WriteDataToLCD(char);
void InitLCD(void);
void WriteStringToLCD(const char*);
void ClearLCDScreen(void);
void LcdPrintString(const char *,  unsigned char, unsigned char);


#endif

/* End of file : lcd_drv.h */