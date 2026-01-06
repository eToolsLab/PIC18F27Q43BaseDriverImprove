
#include "MCP23017.h"

uint8_t MCP23017InitData[11]
            ={0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00};

/**
 * MCP23017‰Šú‰»ŠÖ”
 * @return 
 */
bool MCP23017_Init(uint8_t _clientAddress)
{
    uint8_t i,val;
    bool ret;
    
    ret=MCP23017_Write(_clientAddress,0x0A,0x80);//BankØ‚è‘Ö‚¦    
    if(!ret)return false;
    printf("MCP23017 init\r");
    for(i=0; i<=0x0A; i++)
    {        
        ret=MCP23017_Write(_clientAddress,i,MCP23017InitData[i]);
        if(!ret)return false;
        val=MCP23017_Read(_clientAddress,i);
        printf("[%x]=%x\r",i,val);
    }
    for(i=0x10; i<=0x1A; i++)
    {        
        ret=MCP23017_Write(_clientAddress,i,MCP23017InitData[i-0x10]);
        if(!ret)return false;
        val=MCP23017_Read(_clientAddress,i);
        printf("[%x]=%x\r",i,val);
    }
    return ret;
}

