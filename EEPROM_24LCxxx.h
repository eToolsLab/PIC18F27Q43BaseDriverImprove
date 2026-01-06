/* 
 * File:   24LCxxx.h
 * Author: yutak
 *
 * Created on 2023/01/04, 18:05
 */

#ifndef EEPROM24LCxxx_H
#define	EEPROM24LCxxx_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "Q_peripheral27Q43.h"
#include "Q_I2C1Tup.h"
    
#define EEPROMdeviceAdd 0xAC
#define EEPROM24LC256 0xAC
#define EEPROM24LC512 0xAC
    
//#define EEPROM_PAGE_SIZE 32 
//0x0000-0x007FÇ‹Ç≈Ç™1ÉyÅ[ÉW
#define EEPROM_PAGE_SIZE 128
extern volatile uint8_t eepromPageBuffer[EEPROM_PAGE_SIZE];
extern volatile uint8_t eepromRxBuffer[EEPROM_PAGE_SIZE];
extern uint8_t eeprom_b1Write(uint8_t _deviceAdd, uint16_t _address, uint8_t _data);
extern uint8_t eeprom_b1Read(uint8_t _deviceAdd,uint16_t _address);
extern uint8_t eeprom_bnWrite(uint8_t _deviceAdd,uint16_t _address, uint8_t *_dataArray ,uint8_t length);
extern uint8_t eeprom_bnRead(uint8_t _deviceAdd, uint16_t _address, uint8_t *_dataArray ,uint8_t length);
#ifdef	__cplusplus
}
#endif

#endif	/* 24LCXXX_H */

