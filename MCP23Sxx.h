/* 
 * File:   MCP23S17.h
 * Author: yutak
 *
 * Created on 2023/01/05, 8:00
 */

#ifndef MCP23S17_H
#define	MCP23S17_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "Q_peripheral27Q43.h"
#include "Q_SPI2.h"

#define MCP23S17_deviceAdd1 0x4E
#define MCP23S17_deviceAdd2 0x46

/*-----------------------------------*/
//MCP23S08 I/O expander
/*-----------------------------------*/
#define MCP23S08_Device1Add 0x40
#define MCP23S08_Device2Add 0x42    
#define MCP23S_R 1
#define MCP23S_W 0

typedef enum 
{
    IODIR=0x00,
    IPOL,
    GPINTEN,
    DEFVAL,
    INTCN,
    IOCON,
    GPPU,
    mINTF,
    INTCAP,
    GPIO,
    OLAT=0x0A
}_reg;

extern unsigned char  reg_value[10];
extern uint8_t MCP23S_buf[30];
extern SPI_RD MCP23S08_Init(uint8_t _device_add);

/*-----------------------------------*/
//MCP23S17 I/O expander
/*-----------------------------------*/
#define MCP23S17_Device1Add 0x4E
extern SPI_RD MCP23S17_Init(uint8_t _device_add);
#define MCP23S08_Write SPI2_b3Write
#define MCP23S17_Write SPI2_b3Write
#define MCP23S08_Read SPI2_b1Read
#define MCP23S17_Read SPI2_b1Read

#ifdef	__cplusplus
}
#endif

#endif	/* MCP23S17_H */

