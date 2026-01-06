/* 
 * File:   stringFormat.h
 * Author: yutak
 *
 * Created on January 2, 2023, 8:25 AM
 */

#ifndef STRINGFORMAT_H
#define	STRINGFORMAT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "Q_peripheral27Q43.h"

    
extern char strFmt[10];
extern void uint8ToAscii(uint8_t val,uint8_t *txt);
extern void int8ToAscii(int8_t val,uint8_t *txt);
extern void uint16ToAscii(uint16_t val,uint8_t *txt);
extern void int16ToAscii(int16_t val,uint8_t *txt);
extern void asciiToIntArray(uint8_t* buf, uint8_t* ret);

#ifdef	__cplusplus
}
#endif

#endif	/* STRINGFORMAT_H */

