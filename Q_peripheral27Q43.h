/* 
 * File:   Q_peripheral27Q43.h
 * Author: yutak
 *
 * Created on 2022/12/30, 1:55
 */

#ifndef Q_PERIPHERAL27Q43_H
#define	Q_PERIPHERAL27Q43_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
    
#define Low 0
#define High 1
   
#define _XTAL_FREQ 32000000
    
//USART---------------------------------------    
#define BAUDRATE 115200
//#define BAUDRATE 1000000
#define SET_SPBRG_DATA  (((unsigned int)((_XTAL_FREQ/4)/BAUDRATE))-1)
#define SET_SPBRG_DATA1 (((unsigned int)((_XTAL_FREQ/16)/BAUDRATE))-1)   

#define RXUSART_BUFFER_SIZE 20
#define TXUSART_BUFFER_SIZE 30
typedef struct
{
    uint8_t buf[RXUSART_BUFFER_SIZE];
    uint8_t length;
    uint8_t completed;
    uint8_t overflow;
}_rxUsart;

typedef struct
{
    uint8_t buf[TXUSART_BUFFER_SIZE];
}_txUsart;

typedef struct{
    _rxUsart rx;
    _txUsart tx;
}_Usart;

extern _Usart USART1;

extern void usartInit(void);
//extern void putch(uint8_t);
//extern uint8_t getch(void);
//Timer--------------------------------------
typedef struct
{
    uint16_t cnt;
    bool fg;
}_tm;
extern _tm tm0,tm2;
extern void timer0Init(void);    
extern void timer2Init(void);
extern void timer4Init(void);
//IOC------------------------------------------
extern void iocInit(void);
typedef struct
{
    bool fg;
}_ioc;
extern _ioc ioc;  
//--------------------------------------//
//ADC 12bits
//--------------------------------------//
#define ADC_SamplingNum 8
typedef struct{
    uint16_t val[6];
    uint32_t sum[6];
    uint16_t avr[6];
    uint8_t ch;
    uint8_t cnt;
    bool completed;
}_adc;
extern _adc adcInfo;
extern void adcInit(void);    


//--------------------------------
//クロックリファレンス
//--------------------------------
extern void CLKREF_Init(void);
#ifdef	__cplusplus
}
#endif

#endif	/* Q_PERIPHERAL27Q43_H */

