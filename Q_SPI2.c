#include "Q_SPI2.h"


//****************************************//
//* SPI2_Init       　                    *//
//SPI2モジュール初期化
//****************************************//
void SPI2_Init(void)
{
    //SPI2 pin setting-------------------
    TRISB|=0x04;    //RB2:SDI　input
    //PPS
    PPSLOCK = 0x55; 
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0;//Unlock.
    SPI2SDIPPS=0x0A;    //RB2;
    RB3PPS=0x34;        //SPI2 CLK
    RB4PPS=0x35;        //SPI2 SDO
    //RB5PPS=0x36;      //SPI2 SS
    PPSLOCK = 0x55; 
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 1;//lock.
    
    //SPI2 Configration----------------
    SPI2CON1=0x44;  //CKE:1, SSP:1 active low
    SPI2CON2=0x03;  //TXR=1 RXR=1; Full Duplex mode
    SPI2CLK=0b00000;//Fosc=32Mhz
    SPI2BAUD=0x1;   //8Mhz n=1 32Mhz/(2*(1+1))
    SPI2CON0=0x03;  //MST:1 BMODE:1
    SPI2TWIDTH=8;   //１データビット数
    SPI2CON0bits.EN=1;
    SPI2_CS=1;
}

//****************************************//
//* SPI2_START                           *//
//****************************************//
void SPI2_START(void)
{
    uint8_t timeUp=0;
    while(SPI2CON2bits.BUSY)
    {
        timeUp++;
        if(timeUp>200)
        {
            return;
        }
    };
    SPI2_CS=0;
}
//****************************************//
//* SPI2_STOP                            *//
//****************************************//
void SPI2_STOP(void)
{
    SPI2_CS=1;
}
//****************************************//
//* SPI2_SetTotalCount                   *//
//_val:ビット数またはバイト数(BMODEによる)
//****************************************//
#define BMODE 1
void SPI2_SetTotalCount(uint16_t _val)
{
#if BMODE
    SPI2TCNT=_val;//バイト数
#else
    SPI2TWIDTH = (uint8_t)(_val&0x0007);
    SPI2TCNT=_val>>3;
#endif    
}
//****************************************//
//* SPI2_SetTotalCount                   *//
//return: SPI2CNT値　または、SPI2CNT&SPI2WIDHT値
//****************************************//
uint16_t SPI2_GetTotalCount(void)
{
    uint16_t ret;
#if BMODE
    ret=SPI2TCNT;
    //printf("byteNum=%d\n",ret);
#else    
    ret = SPI2TCNT;
    ret = ret<<3 | SPI2TWIDTH;
    printf("bitNum=%d\n",ret);
#endif    
    return ret;
}
//****************************************//
//* SPI2_Write                           *//
//_val:送信データ
//****************************************//
SPI_RD SPI2_Write(uint8_t _val)
{
    SPI_RD spiRD;
    uint8_t val,timeUp;
    
    spiRD.errorCode=false;
    SPI2TXB=_val;
    timeUp=0;
    while(PIR5bits.SPI2RXIF == 0)
    {
        timeUp++;
        if(timeUp>200)
        {
            return spiRD;
        }
    };
    spiRD.val=SPI2RXB;
    spiRD.errorCode=true;
    return spiRD;
}
//****************************************//
//* SPI2_bnWrite                           *//
//nバイト送信関数
//_num:送信数（バイト数）
//_data:送信バッファ先頭ｱﾄﾞﾚｽ
//****************************************//
SPI_RD SPI2_bnWrite(uint8_t _num, uint8_t* _data)
{
    uint8_t val,timeUp;
    SPI_RD spiRD;
    
    SPI2_START();
    SPI2_SetTotalCount(_num);//bit Counter set
    do{
        SPI2TXB=*_data++;
        timeUp=0;
        while(PIR5bits.SPI2RXIF == 0)
        {
            timeUp++;
            if(timeUp>200)
            {
                return spiRD;
            }
        };
        val=SPI2RXB;
    }while(--_num);
    
    SPI2_STOP();
    spiRD.val=val;
    spiRD.errorCode=true;
    return spiRD;
}
//****************************************//
//* SPI2_b1Write                          *//
//1バイト送信関数
//_val:送信データ
//return:自作Spiデータ構造体
//****************************************//
SPI_RD SPI2_b1Write(uint8_t _val)
{
    SPI_RD spiRD;
    uint8_t val,timeUp;
    
    spiRD.errorCode=false;
    
    SPI2_SetTotalCount(1);//送信バイト数セット
    SPI2_START();       //CS High=>Low
    SPI2TXB=_val;       //送信データセット
    
    timeUp=0;
    while(PIR5bits.SPI2RXIF == 0)//送信完了待ちを受信フラグで確認
    {
        timeUp++;
        if(timeUp>200)
        {
            return spiRD;
        }
    };
    val=SPI2RXB;    //バイト読み出し
    SPI2_STOP();    //CS Low=>High
    spiRD.val=SPI2RXB;//結果を構造体に格納。
    spiRD.errorCode=true;
    return spiRD;
}

//****************************************//
//* SPI2_b3Write                           *//
//３バイト送信関数
//_val:送信データ
//_data1:送信データ
//_data2:送信データ
//****************************************//
SPI_RD SPI2_b3Write(uint8_t _val, int8_t _data1, int8_t _data2)
{
    SPI_RD spiRD;
    uint8_t val,timeUp;
    uint8_t bitNum;
    
    spiRD.errorCode=false;
    
    SPI2_SetTotalCount(3);
    SPI2_START();
    SPI2_GetTotalCount();
    SPI2TXB=_val;
    timeUp=0;
    while(PIR5bits.SPI2RXIF == 0)
    {
        timeUp++;
        if(timeUp>200)
        {
            return spiRD;
        }
    };
    SPI2_GetTotalCount();
    val=SPI2RXB;
    SPI2TXB=_data1;
    timeUp=0;
    while(PIR5bits.SPI2RXIF == 0){
        timeUp++;
        if(timeUp>200)
        {
            return spiRD;
        }
    };
    SPI2_GetTotalCount();
    val=SPI2RXB;
    SPI2TXB=_data2;
    timeUp=0;
    while(PIR5bits.SPI2RXIF == 0){
        timeUp++;
        if(timeUp>200)
        {
            return spiRD;
        }
    };
    val=SPI2RXB;
    SPI2_STOP();
    spiRD.val=SPI2RXB;
    spiRD.errorCode=true;
    SPI2_GetTotalCount();
    return spiRD;
}



//****************************************//
//* SPI2_b1Read                          *//
//１バイト受信関数（２バイト送信後）
//_val:送信データ
//_data:送信データ
//return:SPI_RD構造体(受信データ)
//****************************************//
SPI_RD SPI2_b1Read(uint8_t _val, uint8_t _data1)
{
    SPI_RD spiRD;
    uint8_t val,timeUp;
    
    SPI2_SetTotalCount(3);
    SPI2_START();
    //1.SPIｱﾄﾞﾚｽ送信
    SPI2TXB=_val;
    timeUp=0;
    while(PIR5bits.SPI2RXIF == 0)
    {
        timeUp++;
        if(timeUp>200)
        {
            return spiRD;
        }
    };
    val=SPI2RXB;
    //2.レジスタｱﾄﾞﾚｽ送信
    SPI2TXB=_data1;
     timeUp=0;
    while(PIR5bits.SPI2RXIF == 0)
    {
        timeUp++;
        if(timeUp>200)
        {
            return spiRD;
        }
    };
    val=SPI2RXB;
    //3.レジスタ値受信
    SPI2TXB=0xFF;//dummy tx data
     timeUp=0;
    while(PIR5bits.SPI2RXIF == 0)
    {
        timeUp++;
        if(timeUp>200)
        {
            return spiRD;
        }
    };
    val=SPI2RXB;
    SPI2_STOP();
    spiRD.val=val;
    spiRD.errorCode=true;
    return spiRD;
}


