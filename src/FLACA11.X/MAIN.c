// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcd_hd44780.h"
#define _XTAL_FREQ 20000000

int ADC;
float VOLTAJE;
int SENSOR1;
int SENSOR2;
int ESTADO = 0;
int TIEMPO = 0;
int Boton_Aumento;
int Boton_Decremento;
int Boton_Aceptar;

char SEN1[6];
char SEN2[6];
char TIEM[6];
void INI()
{
    Lcd_Init();
    Lcd_Command(LCD_CLEAR);
    Lcd_Command(LCD_CURSOR_OFF);
    Lcd_ConstText (1, 5, "DIGITALES 2");
    Lcd_ConstText (2, 7, "HARITOL");
    __delay_ms(2000);
    ADCON1 = 0b10000000; //0b10000000
    nRBPU = 0;
    TRISB = 0xFF;
    TRISD = 0x00;
    PORTDbits.RD0 = 0;
    PORTBbits.RB0 = 1;
    PORTBbits.RB1 = 1;
}

void search_channel(int channel)
{
    ADCON0 = channel;
    ADCON0bits.ADON = 1;
    __delay_us(30);
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO == 1);
    ADCON0bits.ADON = 0;
                    
    ADC = ADRESH;
    ADC = ADC << 8;
    ADC = ADC + ADRESL;
    
    VOLTAJE = (ADC*5.0)/1024.0;
    
    if (channel == 0b01000000)
    {
        SENSOR1 = (VOLTAJE*100);
        sprintf(SEN1, "%i", SENSOR1);

    } 
    
    if (channel == 0b01001000)
    {
        SENSOR2 = (VOLTAJE*100);
        sprintf(SEN2, "%i", SENSOR2);

    }   
}

void main()
{
   INI();
   while(1)
   {
       Boton_Aumento = PORTBbits.RB0;
       Boton_Decremento = PORTBbits.RB1;
       Boton_Aceptar = PORTBbits.RB2;
       
       switch(ESTADO)
       {
           
           case 0:
               
               Lcd_Command(LCD_CLEAR);
               Lcd_ConstText (1, 1, "TIMER");
               Lcd_ConstText (2, 1, "TIEMPO:");
               Lcd_ConstText (2, 11, "SEG");
               ESTADO = 1;
           break;
           
           case 1:
               
               if (Boton_Aumento == 0)
               {
                   __delay_ms(250);
                   TIEMPO = TIEMPO + 1;
                   if (TIEMPO > 60)
                   {
                       TIEMPO = 0;
                   }
                   sprintf(TIEM, "%i", TIEMPO);
                   Lcd_ConstText (2, 9, TIEM);
               }
               
               if (Boton_Decremento == 0)
               {
                   __delay_ms(250);
                   TIEMPO = TIEMPO - 1;
                   if (TIEMPO < 0)
                   {
                       TIEMPO = 60;
                   }
                   sprintf(TIEM, "%i", TIEMPO);
                   Lcd_ConstText (2, 9, TIEM);
               }
               
               if (Boton_Aceptar == 0)
               {
                   __delay_ms(250);
                   ESTADO = 2;
               }
               
           break;
           
           case 2:
           
               Lcd_Command(LCD_CLEAR);
               Lcd_ConstText (1, 5, "PROCESO OK");
               ESTADO = 3;
           break;
           
           case 3:
               
               search_channel(0b01000000);
               search_channel(0b01001000);
               Lcd_ConstText (2, 1, "TEMPERATURA: ");
               Lcd_ConstText (2, 15, SEN1);
               Lcd_ConstText (3, 1, "HUMEDAD: ");
               Lcd_ConstText (3, 11, SEN2);
               
               
               if (SENSOR1 >= 30)
               {
                   PORTDbits.RD0 = 1;
                   for (int i = 0; i < TIEMPO; i++)
                   {
                       __delay_ms(1000);
                   }
                   PORTDbits.RD0 = 0;
               }
               
               if (SENSOR2 <= 30)
               {
                   PORTDbits.RD0 = 1;
                   for (int i = 0; i < TIEMPO; i++)
                   {
                       __delay_ms(1000);
                   }
                   PORTDbits.RD0 = 0;
               }
           break;
       }
       
       
       
   }
}
