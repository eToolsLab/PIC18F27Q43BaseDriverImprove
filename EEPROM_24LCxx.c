#include "EEPROM_24LCxxx.h"
#include "Q_I2C1Tup.h"

uint8_t eeprom_b1Write(uint8_t _deviceAdd, uint16_t _address, uint8_t _data)
{
    uint8_t _addH, _addL;
    _addH=(uint8_t)(_address>>8);
    _addL=(uint8_t)(_address);
    PIE11bits.TMR4IE=1;
    T4CONbits.TMR4ON=1;
    I2C1CON0bits.EN=1;
    I2C1ADB1 = _deviceAdd;
    I2C1CNT=3;
    I2C1TXB=_addH;
    
    while(!I2C1STAT0bits.BFRE)
    {
         if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    I2C1CON0bits.S=1;
    
    
    while(I2C1STAT1bits.TXBE!=1)
    {
     if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    I2C1TXB=_addL;
    while(I2C1STAT1bits.TXBE!=1)
    {
         if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    I2C1TXB=_data;
    while(I2C1STAT1bits.TXBE!=1)
    {
         if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    while(I2C1PIRbits.PC1IF==0)
    {
         if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    I2C1PIRbits.PC1IF=0;
    I2C1CON0bits.S=0;
    while(I2C1STAT0bits.MMA)
    {
         if(I2C1ERRbits.BTOIF)
        {
           I2C_TimeOUT_OP();
            return false;
        }
    };
    //Set Clear Buffer Flag
    I2C1STAT1bits.CLRBF = 1;
    PIE11bits.TMR4IE=0;
    T4CONbits.TMR4ON=0;
    __delay_ms(10);
    return true;
}

uint8_t eeprom_b1Read(uint8_t _deviceAdd,uint16_t _address)
{
    uint8_t ret;
    uint8_t _addH, _addL;
    _addH=(uint8_t)(_address>>8);
    _addL=(uint8_t)(_address);
    PIE11bits.TMR4IE=1;
    T4CONbits.TMR4ON=1;
    I2C1CON0bits.EN=1;

    I2C1ADB1 = _deviceAdd;
    I2C1CNT=2;
    I2C1TXB=_addH;
    
    while(!I2C1STAT0bits.BFRE)
    {
        if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return 0xE0;
        }
    };
    //リスタート
    I2C1CON0bits.RSEN=1;
    I2C1CON0bits.S=1;
    
    while(I2C1STAT1bits.TXBE!=1)
    {
        if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return 0xE1;
        }
    };
    I2C1TXB=_addL;
    while(I2C1STAT1bits.TXBE!=1)
    {
         if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return 0xE2;
        }
    };
    I2C1PIRbits.PC1IF=0;
    
    while(!I2C1CON0bits.MDR)
    {
        if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return 0xE3;
        }
    };
    I2C1ADB1 = _deviceAdd|0x01;
    I2C1CNT=1;
    I2C1CON0bits.S=1;
    
    while(!I2C1STAT1bits.RXBF)
    {
        if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return 0xE4;
        }
    };
    ret=I2C1RXB;
    I2C1CON0bits.RSEN=0;
    I2C1CON0bits.S=0;
    I2C1PIR=0x00;
    while(I2C1STAT0bits.MMA)
    {
        if(I2C1ERRbits.BTOIF)
        {
           I2C_TimeOUT_OP();
            return 0xE5;
        }
    };
    //Set Clear Buffer Flag
    I2C1STAT1bits.CLRBF = 1;
    PIE11bits.TMR4IE=0;
    T4CONbits.TMR4ON=0;
    return ret;
}

uint8_t volatile eepromPageBuffer[EEPROM_PAGE_SIZE]={0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9};
uint8_t eeprom_bnWrite(uint8_t _deviceAdd, uint16_t _address, uint8_t *_dataArray, uint8_t length)
{
    uint8_t cnt=0,val;
    uint8_t _addH, _addL;
    _addH=(uint8_t)(_address>>8);
    _addL=(uint8_t)(_address);
    PIE11bits.TMR4IE=1;
    T4CONbits.TMR4ON=1;
    I2C1CON0bits.EN=1;
    I2C1ADB1 = _deviceAdd;
    //I2C1CNT=2+EEPROM_PAGE_SIZE;
    I2C1CNT=2+length;
    I2C1TXB=_addH;
    
    while(!I2C1STAT0bits.BFRE)
    {
         if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    
    I2C1CON0bits.S=1;
    
    while(I2C1STAT1bits.TXBE!=1)
    {
     if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    I2C1TXB=_addL;
    while(I2C1STAT1bits.TXBE!=1)
    {
         if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    do{
        I2C1TXB=_dataArray[cnt++];
        while(I2C1STAT1bits.TXBE!=1)
        {
             if(I2C1ERRbits.BTOIF)
            {
                I2C_TimeOUT_OP();
                return false;
            }
        };
    }while(I2C1CNT!=0);
        
    while(I2C1PIRbits.PC1IF==0)
    {
         if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    I2C1PIRbits.PC1IF=0;
    I2C1CON0bits.S=0;
    while(I2C1STAT0bits.MMA)
    {
         if(I2C1ERRbits.BTOIF)
        {
           I2C_TimeOUT_OP();
            return false;
        }
    };
    //Set Clear Buffer Flag
    I2C1STAT1bits.CLRBF = 1;
    PIE11bits.TMR4IE=0;
    T4CONbits.TMR4ON=0;
    return true;
}

uint8_t volatile eepromRxBuffer[EEPROM_PAGE_SIZE]; 
uint8_t eeprom_bnRead(uint8_t _deviceAdd, uint16_t _address, uint8_t *_dataArray, uint8_t length)
{
    uint8_t cnt,ret;
    uint8_t _addH, _addL;
    _addH=(uint8_t)(_address>>8);
    _addL=(uint8_t)(_address);
    PIE11bits.TMR4IE=1;
    T4CONbits.TMR4ON=1;
    I2C1CON0bits.EN=1;

    I2C1ADB1 = _deviceAdd;
    I2C1CNT=2;
    I2C1TXB=_addH;
    
    while(!I2C1STAT0bits.BFRE)
    {
        if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return 0xE0;
        }
    };
    
    //スタートコンディション発令
    I2C1CON0bits.RSEN=1;
    I2C1CON0bits.S=1;
    
    while(I2C1STAT1bits.TXBE!=1)
    {
        if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return 0xE1;
        }
    };
    I2C1TXB=_addL;
    while(I2C1STAT1bits.TXBE!=1)
    {
         if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return 0xE2;
        }
    };
    I2C1PIRbits.PC1IF=0;
   
    while(!I2C1CON0bits.MDR)
    {
        if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return 0xE3;
        }
    };
    
    //リスタート開始
    I2C1ADB1 = _deviceAdd|0x01;
    I2C1CNT=length;
    I2C1CON0bits.S=1;
    
    cnt=0;
    do{
        while(!I2C1STAT1bits.RXBF)
        {
            if(I2C1ERRbits.BTOIF)
            {
                I2C_TimeOUT_OP();
                return 0xE4;
            }
        };
        _dataArray[cnt++]=I2C1RXB;
    }while(I2C1CNT!=0);
    I2C1CON0bits.RSEN=0;
    I2C1CON0bits.S=0;
    I2C1PIR=0x00;
    while(I2C1STAT0bits.MMA)
    {
        if(I2C1ERRbits.BTOIF)
        {
           I2C_TimeOUT_OP();
            return 0xE5;
        }
    };
    //Set Clear Buffer Flag
    I2C1STAT1bits.CLRBF = 1;
    PIE11bits.TMR4IE=0;
    T4CONbits.TMR4ON=0;
    return 1;
}
