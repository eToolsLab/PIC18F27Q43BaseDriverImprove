/* 
 * File:   LCDdriver.h
 * Author: h
 *
 * Created on 2019/12/09, 21:05
 */

#ifndef LCDDRIVER_H
#define	LCDDRIVER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "Q_peripheral27Q43.h"

    
#define CM	0	//Command Mode
#define DM	1	//Data Mode
#define WM  0	//èoóÕWriteMode
#define RM  1   //ì¸óÕReadMode
#define High 1
#define Low  0
#define LED_ON 1
#define LED_OFF 0
#define LcdDeviceAdd 0x4E//FC-113(RCF8574 address)
#define LcdDeviceAdd_AT 0x7E//RCF8574AT address    

#define LcdDDRAMAdd_firstline 0x80
#define LcdDDRAMAdd_secondline 0xC0
#define LcdDDRAMAdd_thirdline 0x94
#define LcdDDRAMAdd_forthline 0xD4
#define LcdCmd_Clear_all 0x01
    
    typedef struct{
        uint8_t length;
        uint8_t txt[18];
        
    }_LCD_DATA;
    extern _LCD_DATA Lcd;
    
extern uint8_t LCD_BackLight_EN[2];
union {
	uint8_t byt;
	struct{
		uint8_t RS:1;//LSB
		uint8_t RW:1;
		uint8_t EN:1;
		uint8_t LED:1;
		uint8_t D4:1;
		uint8_t D5:1;
		uint8_t D6:1;
		uint8_t D7:1;//MSB
	};
}OD;

extern bool LCD_send(uint8_t _device_add,uint8_t _data, uint8_t _mode);
extern bool LCD_WriteData(uint8_t _device_add,uint8_t data);
extern bool LCD_Command(uint8_t _device_add,uint8_t cmd);
extern void LCD_Init(uint8_t _device_add);
extern bool LCD_Printf(uint8_t _device_add, uint8_t *string, uint8_t _length, uint8_t _DDRAM_Add);
extern void LCD_LED_Pow(uint8_t _device_add);
  

#ifdef	__cplusplus
}
#endif

#endif	/* LCDDRIVER_H */

