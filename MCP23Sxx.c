#include "MCP23Sxx.h"

/*-----------------------------------*/
//MCP23S08 I/O expander
//_device_add:デバイスｱﾄﾞﾚｽ
//return:SPI_RD構造体
/*-----------------------------------*/
SPI_RD MCP23S08_Init(uint8_t _device_add)
{
    SPI_RD spiRD;
    uint8_t i;
    
    for(i=0x00; i<0x0A; i++)
    {
        MCP23S08_Write(MCP23S08_Device1Add,i,0x00);
    }
    
    MCP23S08_Write(MCP23S08_Device1Add,IODIR,0x00);
    MCP23S08_Write(MCP23S08_Device1Add,IOCON,0x38);
    MCP23S08_Write(MCP23S08_Device2Add,IODIR,0x00);
    MCP23S08_Write(MCP23S08_Device2Add,IOCON,0x38);
    spiRD.errorCode=true;
    
    for(i=0x00; i<=0x0A; i++)
    {
        spiRD=MCP23S08_Read(MCP23S08_Device1Add|0x01,i);
        printf("[%02x]=%02x\n",i,spiRD.val);
    }
    putch('\n');
     for(i=0x00; i<=0x0A; i++)
    {
        spiRD=MCP23S08_Read(MCP23S08_Device2Add|0x01,i);
        printf("[%02x]=%02x\n",i,spiRD.val);
    }
    
    return spiRD;
}

/*-----------------------------------*/
//MCP23S17 I/O expander
/*-----------------------------------*/
SPI_RD MCP23S17_Init(uint8_t _device_add)
{
    SPI_RD spiRD;
    uint8_t i;
    
    //Bank切り替え
    MCP23S17_Write(MCP23S17_Device1Add,0x0A,0xA8);
    __delay_ms(10);
    //Bank=1
    MCP23S17_Write(MCP23S17_Device1Add,0x00,0x00);    //PORTA IO direction
    MCP23S17_Write(MCP23S17_Device1Add,0x10,0x00);    //PORTB IO direction
    MCP23S17_Write(MCP23S17_Device1Add,0x09,0x00);    //GPIOA 
    MCP23S17_Write(MCP23S17_Device1Add,0x19,0x00);    //GPIOB
    spiRD.errorCode=true;
    //設定確認のレジスタ読み出し
    printf("MCP23S17 init\n");
    for(i=0x00; i<=0x0A; i++)
    {
        spiRD=MCP23S17_Read(MCP23S17_Device1Add|0x01,i);
        printf("[%02x]=%02x\n",i,spiRD.val);
    }
    for(i=0x10; i<=0x1A; i++)
    {
        spiRD=MCP23S17_Read(MCP23S17_Device1Add|0x01,i);
        printf("[%02x]=%02x\n",i,spiRD.val);
    }
    putch('\n');
    
    return spiRD;
}
