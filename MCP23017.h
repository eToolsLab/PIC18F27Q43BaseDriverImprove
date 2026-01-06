/* 
 * File:   MCP23017.h
 * Author: yutak
 *
 * Created on 2023/01/03, 11:48
 */

#ifndef MCP23017_H
#define	MCP23017_H

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
    
#define MCP23017Add 0x4C    
    
enum{
    IODIRA=0x00,
    IPOLA,
    GPINTENA,
    DEFVALA,
    INTCONA,
    IOCON1,
    GPPUA,
    INTFA,
    INTCAPA,
    GPIOA,
    OLATA,
    IODIRB=0x10,
    IPOLB,
    GPINTENB,
    DEFVALB,
    INTCONB,
    IOCON2,
    GPPUB,
    INTFB,
    INTCAPB,
    GPIOB,
    OLATB
};

extern uint8_t MCP23017InitData[11];
    
extern bool MCP23017_Init(uint8_t _clientAddress);

#define MCP23017_Write I2C1_b2Write  
#define MCP23017_Read I2C1_b1Read

#ifdef	__cplusplus
}
#endif

#endif	/* MCP23017_H */

