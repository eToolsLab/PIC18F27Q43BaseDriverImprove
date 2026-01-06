/* 
 * File:   Q_interrupt.h
 * Author: yutak
 *
 * Created on 2022/12/30, 2:04
 */

#ifndef Q_INTERRUPT_H
#define	Q_INTERRUPT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "Q_peripheral27Q43.h"
  
extern void __interrupt(irq(IRQ_U1RX)) USART1RX_ISR(void);    
extern void __interrupt(irq(IRQ_TMR0)) Timer0_ISR(void); 
extern extern void __interrupt(irq(IRQ_TMR2)) Timer2_ISR(void); 
extern void __interrupt(irq(IRQ_IOC)) IOC_ISR(void);

#ifdef	__cplusplus
}
#endif

#endif	/* Q_INTERRUPT_H */

