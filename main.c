
/*
 * File:   main.c
 * Author: yutak
 *
 * Created on 2022/12/29, 18:47
 */



// PIC18F27Q43 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator Selection (Oscillator not enabled)
#pragma config RSTOSC = HFINTOSC_64MHZ// Reset Oscillator Selection (HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1)

// CONFIG2
#pragma config CLKOUTEN = OFF   // Clock out Enable bit (CLKOUT function is disabled)
#pragma config PR1WAY = OFF     // PRLOCKED One-Way Set Enable bit (PRLOCKED bit can be set and cleared repeatedly)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)

// CONFIG3
#pragma config MCLRE = EXTMCLR  // MCLR Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTS = PWRT_OFF // Power-up timer selection bits (PWRT is disabled)
#pragma config MVECEN = ON      // Multi-vector enable bit (Multi-vector enabled, Vector table used for interrupts)
#pragma config IVT1WAY = ON     // IVTLOCK bit One-way set enable bit (IVTLOCKED bit can be cleared and set only once)
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit (Low-Power BOR disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)

// CONFIG4
#pragma config BORV = VBOR_1P9  // Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR) set to 1.9V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD module is disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = OFF     // PPSLOCK bit One-Way Set Enable bit (PPSLOCKED bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG5
#pragma config WDTCPS = WDTCPS_31// WDT Period selection bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled; SWDTEN is ignored)

// CONFIG6
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG7
#pragma config BBSIZE = BBSIZE_512// Boot Block Size selection bits (Boot Block size is 512 words)
#pragma config BBEN = OFF       // Boot Block enable bit (Boot block disabled)
#pragma config SAFEN = OFF      // Storage Area Flash enable bit (SAF disabled)
#pragma config DEBUG = OFF      // Background Debugger (Background Debugger disabled)

// CONFIG8
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block not Write protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers not Write protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not Write protected)
#pragma config WRTSAF = OFF     // SAF Write protection bit (SAF not Write Protected)
#pragma config WRTAPP = OFF     // Application Block write protection bit (Application Block not write protected)

// CONFIG10
#pragma config CP = OFF         // PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code protection disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "Q_peripheral27Q43.h"
#include "Q_interrupt27Q43.h"
#include "Q_I2C1Tup.h"
#include "I2C_LCD.h"
#include "stringFormat.h"
#include "Q_SPI2.h"
#include "MCP23Sxx.h"
#include "MCP23017.h"
#include "EEPROM_24LCxxx.h"
#include "RTC_DS3231.h"
#include "stringFormat.h"

void portInit(void);
void oscillatorInit(void);
void vicInit(void);

uint8_t txt[20];

void main(void)
{
    uint8_t counter=0,val,i;
    uint8_t ledVal;
    bool ret;
    bool lcdError;
    uint8_t eCount;
    
    //CPUハード初期化-----------------------
    portInit();
    oscillatorInit();
    vicInit();
    __delay_ms(50);
    //周辺機能初期化--------------------------------
    timer0Init();
    timer4Init();
    //iocInit();
    usartInit();
    CLKREF_Init();
    SPI2_Init();
    MCP23S17_Init(0x4E);
    I2C1_Init();
    __delay_ms(10);
    MCP23017_Init(MCP23017Add);
    printf("START\n");    
    LCD_Init(LcdDeviceAdd);
    CLKRCONbits.DIV = 0b100;   //divided by 16 I2CCLK:2Mhz/5=400kHz
    //CLKRCONbits.DIV = 0b011; //divided by  8 I2CCLK:4Mhz/5=800kHz
    LCD_Printf(LcdDeviceAdd,"18F27Q43",8,0x80);
    //EEPROM test
     for(i=0; i<0x3F; i++)
    {
        eepromPageBuffer[i]=0xFF;
    }
    eeprom_bnWrite(EEPROM24LC256,0x0000,&eepromPageBuffer,0x3F);
    __delay_ms(30);
    eeprom_bnRead(EEPROM24LC256,0x0000,&eepromRxBuffer,0x03F);
    for(i=0; i<=0x002F; i++)
    {
        if(i%16==0)
        {
            putch('\r');
            printf("%04X:",i);
        }
        printf("%02X ",eepromRxBuffer[i]);
    }
    putchar('\r');
    
    for(i=0; i<10; i++)
    {
        eepromPageBuffer[i]=0xA0+i;
    }
    
    eeprom_b1Write(EEPROM24LC256,0x0000,0xA1);
    eeprom_b1Write(EEPROM24LC256,0x0001,0xA3);
    val = eeprom_b1Read(EEPROM24LC256,0x0000);
    printf("EEPROM 0000:%02X\r",val);
    val = eeprom_b1Read(EEPROM24LC256,0x0001);
    printf("EEPROM 0001:%02X\r",val);
    
    eeprom_bnWrite(EEPROM24LC256,0x0020,&eepromPageBuffer,10);
    __delay_ms(30);
    eeprom_bnRead(EEPROM24LC256,0x0020,&eepromRxBuffer,10);
   
    eeprom_bnRead(EEPROM24LC256,0x0000,&eepromRxBuffer,0x02F);
    for(i=0; i<=0x002F; i++)
    {
        if(i%16==0)
        {
            putch('\r');
            printf("%04X:",i);
        }
        printf("%02X ",eepromRxBuffer[i]);
    }
    putch('\r');
    //RTC
    initDS3231();
    rtcDateRead(RTCDeviceAdd,txt);
    printf("%s\r",txt);
    LCD_Printf(LcdDeviceAdd,&txt,8,0x80);
    //RTC時刻読込-----------------------------------------
    rtcTimeRead(RTCDeviceAdd,txt);
    //LCD表示　日時
    LCD_Printf(LcdDeviceAdd,txt,8,0xC0);
    
    //CLKRCONbits.DIV = 0b100;   //divided by 16 I2CCLK:2Mhz/5=400kHz
    
    INTCON0bits.GIE=1;//Enable all masked interrupts
    ledVal=0xAA;
    lcdError=false;
    eCount=0;
    while(1)
    {
        //Timer0 interrupt process-------
        if(tm0.fg)
        {
            tm0.fg=false;
            
            //LED点灯
            SPI2_b3Write(MCP23S17_Device1Add,0x0A,ledVal);
            SPI2_b3Write(MCP23S17_Device1Add,0x1A,(uint8_t)ledVal);
                
            ret=MCP23017_Write(MCP23017Add,0x0A,ledVal);
            ret=MCP23017_Write(MCP23017Add,0x1A,ledVal);
            
            

            //Lcd.length = sprintf(Lcd.txt,"%04d",counter);
            //ret=LCD_Printf(LcdDeviceAdd,Lcd.txt,Lcd.length,0xC0);
           
            //RTC時刻読込-----------------------------------------
            rtcTimeRead(RTCDeviceAdd,txt);
            //LCD表示　日時
            LCD_Printf(LcdDeviceAdd,txt,8,0xC0);
            
            printf("%d\r",counter); 
            counter++;
            if(counter>1000)
            {
                counter=0;
            }
            if(ledVal==0xAA)
                ledVal=0x55;
            else
                ledVal=0xAA;
            T0CON0bits.EN=1;
            PIE3bits.TMR0IE=1;
        } 
        
        //Usart interrupt process--------
        if(USART1.rx.completed)
        {
            USART1.rx.completed=false;
            printf("echo:%s\r",USART1.rx.buf);
             //USART受信　RTC時間設定
            if(USART1.rx.buf[0]=='t' && USART1.rx.length==15)
            {
                //initDS1307();
                asciiToIntArray(&USART1.rx.buf[1],Rtc.TM);
                for(i=0; i<6; i++)
                     printf("TM[%d]=%x",i,Rtc.TM[i]);
                printf("\n");
                setDS3231Time(Rtc.TM[0],Rtc.TM[1],Rtc.TM[2],Rtc.TM[3]
                                 ,Rtc.TM[4],Rtc.TM[5]);
                rtcDateRead(RTCDeviceAdd,txt);
                LCD_Printf(LcdDeviceAdd,&txt,8,0x80);
            }
            USART1.rx.length=0;
            PIE4bits.U1RXIE=1;
        }
        

    }
    return;
}

void oscillatorInit(void)
{
    //オシレータ設定----------------
    OSCCON3bits.CSWHOLD=1;//Hold
    OSCCON1bits.NDIV=1;//64Mhz/2=32Mhz;
    while(!OSCCON3bits.NOSCR);
    while(!PIR0bits.CSWIF);//ready state
    PIR0bits.CSWIF=0;
    OSCCON3bits.CSWHOLD=0;
    while(!OSCCON3bits.ORDY);
}

/**
 * ポート初期化
 * ペリフェラルIOの初期化は、
 * 各初期化関数ないで設定
 */
void portInit(void)
{
    //ポート設定----------------------    
    PORTA=0x00;
    LATA=0x00;
    ANSELA=0x00;
    TRISA=0x00;
    
    PORTB=0x00;
    LATB=0x00;
    ANSELB=0x00;
    TRISB=0x00;
    
    PORTC=0x00;
    LATC=0x00;
    ANSELC=0x00;
    TRISC=0x00;
  
}

/**
 * 割り込みベクタ開始アドレス設定
 */
void vicInit(void)
{
 //割り込みテーブルaddress設定----------------------------------
    INTCON0bits.GIE=0;//Enable all masked interrupts
    IVTLOCK=0x55;
    IVTLOCK=0xAA;
    IVTLOCKbits.IVTLOCKED=0;
    IVTBASE = 0x000008;
    IVTLOCK=0x55;
    IVTLOCK=0xAA;
    IVTLOCKbits.IVTLOCKED=1;
}