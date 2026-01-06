/* 
 * File:   Q_I2C1.h
 * Author: yutak
 *
 * Created on 2023/01/01, 18:25
 */

#ifndef Q_I2C1_H
#define	Q_I2C1_H
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "Q_peripheral27Q43.h"


extern void I2C1_Init(void);
extern bool I2C1_b1Write(uint8_t _deviceAdd, uint8_t _data1);
extern uint8_t I2C1_b2Write(uint8_t _deviceAdd, uint8_t _data1, uint8_t _data2);
extern uint8_t I2C1_b1Read(uint8_t _deviceAdd,uint8_t _address);
extern void I2C_TimeOUT_OP(void);
#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* Q_I2C1_H */

