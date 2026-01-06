/* 
 * File:   Q_SPI2.h
 * Author: yutak
 *
 * Created on 2023/01/05, 7:14
 */

#ifndef Q_SPI2_H
#define	Q_SPI2_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "Q_peripheral27Q43.h"

#define SPI2_CS PORTBbits.RB5    
    
typedef struct{
    uint8_t val;
    bool errorCode;
}SPI_RD;


extern void SPI2_Init(void);
extern void SPI2_START(void);
extern void SPI2_STOP(void);
extern SPI_RD SPI2_b3Write(uint8_t _val, int8_t _data1, int8_t _data2);
extern SPI_RD SPI2_bnWrite(uint8_t _num, uint8_t* _data);
extern SPI_RD SPI2_b1Read(uint8_t _val, uint8_t _data1);
extern SPI_RD SPI2_b1Write(uint8_t _val);
extern void  SPI2_SetTotalCount(uint16_t _val);
extern uint16_t SPI2_GetTotalCount(void);
extern SPI_RD SPI2_Write(uint8_t _val);
    
#ifdef	__cplusplus
}
#endif

#endif	/* Q_SPI2_H */

