/* 
 * File:   hd44780.c
 * Author: http://pic18fxx.blogspot.com
 *
 * Dot matrix liquid crystal display.
 * Driver HD44780.
 * Interface: 4-bit mode.
*/

#include <xc.h>
#include "lcd_hd44780.h"
#define _XTAL_FREQ   20000000 // This one is just for __delay_ms

//==============================================================================
// Initializes the Lcd module.
//==============================================================================
void Lcd_Init(void){
unsigned char data;
TRISRD7 = 0; TRISRD6 = 0; TRISRD5 = 0; TRISRD4 = 0; TRISEN = 0; TRISRS = 0;
LCD_RD7 = 0; LCD_RD6 = 0; LCD_RD5 = 0; LCD_RD4 = 0; LCD_EN = 0; LCD_RS = 0;
__delay_us(5500);__delay_us(5500);__delay_us(5500);
__delay_us(5500);__delay_us(5500);__delay_us(5500);

for(data = 1; data < 4; data ++)
   {
    LCD_RD7 = 0; LCD_RD6 = 0; LCD_RD5 = 1; LCD_RD4 = 1; LCD_EN = 0; LCD_RS = 0;
    LCD_RD7 = 0; LCD_RD6 = 0; LCD_RD5 = 1; LCD_RD4 = 1; LCD_EN = 1; LCD_RS = 0;
    __delay_us(5);
    LCD_RD7 = 0; LCD_RD6 = 0; LCD_RD5 = 1; LCD_RD4 = 1; LCD_EN = 0; LCD_RS = 0;
    __delay_us(5500);
   }
LCD_RD7 = 0; LCD_RD6 = 0; LCD_RD5 = 1; LCD_RD4 = 0; LCD_EN = 0; LCD_RS = 0;
LCD_RD7 = 0; LCD_RD6 = 0; LCD_RD5 = 1; LCD_RD4 = 0; LCD_EN = 1; LCD_RS = 0;
__delay_us(5);
LCD_RD7 = 0; LCD_RD6 = 0; LCD_RD5 = 1; LCD_RD4 = 0; LCD_EN = 0; LCD_RS = 0;
__delay_us(5500);
data = 40; Lcd_Command(data);
data = 16; Lcd_Command(data);
data = 1;  Lcd_Command(data);
data = 15; Lcd_Command(data);
}

//==============================================================================
// Prints text constant on the Lcd.
// y: Starting position row number. Valid values: 1, 2, 3, 4
// x: Starting position column number. Valid values: 1..20
// buffer: Byte of array.
//==============================================================================
void Lcd_ConstText(unsigned char y, unsigned char x, const char *buffer){
unsigned char data;
    switch(y)
    {
    case 1: data = (unsigned char)(127 + x); break;
    case 2: data = (unsigned char)(191 + x); break;
    case 3: data = (unsigned char)(147 + x); break;
    case 4: data = (unsigned char)(211 + x); break;
    default: break;
    }
    Lcd_Command(data);
    while(*buffer)                // Write data to LCD up to null
         {
          Lcd_CharCP(*buffer);
          buffer++;               // Increment buffer
         }
return;
}


//==============================================================================
// Prints character on the Lcd.
// y: Starting position row number. Valid values: 1, 2, 3, 4
// x: Starting position column number. Valid values: 1..20
// c: character to be written.
//==============================================================================
void Lcd_WriteChar(unsigned char y, unsigned char x, char c){
unsigned char data;
switch(y){
case 1: data = (unsigned char)(127 + x); break;
case 2: data = (unsigned char)(191 + x); break;
case 3: data = (unsigned char)(147 + x); break;
case 4: data = (unsigned char)(211 + x); break;
default: break;}
Lcd_Command(data);
Lcd_CharCP(c);
}

//==============================================================================
// Prints text on Lcd at current cursor position.
// c: character to be written.
//==============================================================================
void Lcd_CharCP(char c){
LCD_EN = 0; LCD_RS = 1;
LCD_RD7 = (unsigned char)((c & 0b10000000)>>7); LCD_RD6 = (unsigned char)((c & 0b01000000)>>6);
LCD_RD5 = (unsigned char)((c & 0b00100000)>>5); LCD_RD4 = (unsigned char)((c & 0b00010000)>>4);
//_delay(10);
LCD_EN = 1; __delay_us(5); LCD_EN = 0;
LCD_RD7 = (unsigned char)((c & 0b00001000)>>3); LCD_RD6 = (unsigned char)((c & 0b00000100)>>2);
LCD_RD5 = (unsigned char)((c & 0b00000010)>>1); LCD_RD4 = (unsigned char)((c & 0b00000001));
//_delay(10);
LCD_EN = 1; __delay_us(5); LCD_EN = 0;
__delay_us(5); __delay_us(5500);
}

//==============================================================================
// Sends command to Lcd.
// command: command to be sent.
//==============================================================================
void Lcd_Command(unsigned char command){
LCD_EN = 0; LCD_RS = 0;
LCD_RD7 = (unsigned char)((command & 0b10000000)>>7); LCD_RD6 = (unsigned char)((command & 0b01000000)>>6);
LCD_RD5 = (unsigned char)((command & 0b00100000)>>5); LCD_RD4 = (unsigned char)((command & 0b00010000)>>4);
//_delay(10);
LCD_EN = 1; __delay_us(5); LCD_EN = 0;
LCD_RD7 = (unsigned char)((command & 0b00001000)>>3); LCD_RD6 = (unsigned char)((command & 0b00000100)>>2);
LCD_RD5 = (unsigned char)((command & 0b00000010)>>1); LCD_RD4 = (unsigned char)((command & 0b00000001));
//_delay(10);
LCD_EN = 1; __delay_us(5); LCD_EN = 0;
__delay_us(5500);
}
