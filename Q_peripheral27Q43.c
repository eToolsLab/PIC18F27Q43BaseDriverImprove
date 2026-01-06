
#include <pic18f27q43.h>
#include "Q_peripheral27Q43.h"

//*****************************************************//
//USART1 初期化
//*****************************************************//
_Usart USART1;
void usartInit(void)
{
    //1.ボーレート設定115200
    U1BRG=SET_SPBRG_DATA1;
    //2.モード　非同期
    U1CON0bits.MODE=0b0000; //Asynchronous 8-bit USART mode
    //4.Serial port enabled
    U1CON1bits.ON=1;
    //5.Transmit is enabled.
    U1CON0bits.TXEN=1;
    U1CON0bits.RXEN=1;
    //6.PPS
    PPSLOCK = 0x55; 
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0;//Unlock.
    U1RXPPS=0b010111;//RC7:rx
    RC6PPS=0x20;//RC6:tx
    TRISC|=0x80;
    PPSLOCK = 0x55; 
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 1;//lock.
    //7.interrupt
    PIR4bits.U1RXIF=0;
    PIE4bits.U1RXIE=1; 
}

void putch(char _txData)
{
    if(U1ERRIRbits.TXMTIF==High)//ｼﾌﾄﾚｼﾞｽﾀ空?
    {
        do{
            U1TXB=_txData;
            while(U1ERRIRbits.TXMTIF==Low);
        }while(U1FIFObits.TXBF);//TXﾊﾞｯﾌｧ空になるまで送信。
    }else{
        //error operation
    }
}

int getch(void)
{
    while(PIR4bits.U1RXIF==Low);//受信完了
    return U1RXB;
}

//*****************************************************//
//Timer0初期化
//*****************************************************//
_tm tm0;
void timer0Init(void)
{
//Timer0初期化--------------------------------------------------
    T0CON1bits.CS=0b010;//クロックソースFosc/4
    T0CON1bits.CKPS=0b0011;//プリスケ1:8
    T0CON0bits.MD16=1;//16bitsタイマー
    T0CON0bits.OUTPS=0b0000;//ﾎﾟｽﾄｽｹｰﾗ1:1
    TMR0H=0xD8;//10ms Fosc=32Mhz
    TMR0L=0xF0;//10ms 
    T0CON0bits.EN=1;
    PIE3bits.TMR0IE=1;
    PIR3bits.TMR0IF=0;
}

/*---------------------------------------------------
 Timer2
 ---------------------------------------------------*/
_tm tm2;
void timer2Init(void)
{
    TMR2=0x00;
    T2CONbits.OUTPS=0b1111;//postScaler 1:16
    T2CONbits.T2CKPS=0b100;//prescle1:16
    T2HLTbits.MODE=0b00000;//softwareGateMode(従来のTimer)
    T2HLTbits.PSYNC=1;//synchoronized to Fosc/4
    T2CLKCONbits.CS=0b00001;//Fosc/4
    PR2=0x1F;//Fosc=32Mhz 1ms
    //PR2=0x9C;//Fosc=32Mhz 5ms
    tm2.cnt=0;
    tm2.fg=false;        
    PIR3bits.TMR2IF=0;
    PIE3bits.TMR2IE=1;
    T2CONbits.TMR2ON=1;
}

/*---------------------------------------------------
 Timer4
 ---------------------------------------------------*/
_tm tm4;
void timer4Init(void)
{
    TMR4=0x00;
    T4CONbits.OUTPS=0b1111;//postScaler 1:16
    T4CONbits.T4CKPS=0b111;//prescle1:128
    T4HLTbits.PSYNC=1;//synchoronized to Fosc/4
    T4CLKCONbits.CS=0b00001;//Fosc/4
    //PR4=0x1F;//Fosc=32Mhz 1ms
    PR4=0x27;//Fosc=32Mhz 10ms
    tm4.cnt=0;
    tm4.fg=false;        
    PIR11bits.TMR4IF=0;
    PIE11bits.TMR4IE=0;
    T4CONbits.TMR4ON=0;
}


//*****************************************************//
//ioc初期化
//*****************************************************//
_ioc ioc;
void iocInit(void)
{
    IOCCNbits.IOCCN0=1;//RC0
    TRISC|=0x01;//RC0
    PIE0bits.IOCIE=1;
    PIR0bits.IOCIF=0;
}

//******************************************************//
// * ADC初期化 Basic Mode
// *****************************************************//
_adc adcInfo;
void adcInit(void)
{
    //port設定
    TRISA|=0x04;            //RA2:input 
    ANSELA|=0x04;           //RA2:ANA4;   
    //参照電圧設定
    ADREFbits.NREF=0;       //Negative Voltage: Vss
    ADREFbits.PREF=0b00;    //Positive Voltage: Vdd
    //ADC変換クロック設定 1us    
    ADCON0bits.CS=0;        //ConversionClockSource:Fosc=32Mhz
    ADCLK=15;               //Fosc/(2*(n+1)):n=15 Tad=1us                           
    //チャンネル指定
    ADPCH=2;                //Channel Select RA2 ANA2
    adcInfo.ch=2;   
    //フォーマット
    ADCON0bits.FM=1;        //Right justified
    //mode
    ADCON2bits.MD = 0b000;  //Basic mode
    //Auto Conversion Trigger Source
    ADACTbits.ACT=0b00100;  //TMR2 postscaled.
    //Precharge Time 0;
    ADPRE = 0;
    //acquisition time Tacq=7us(50度、10kΩ、5v)
    ADACQ = 224;            //Tosc=1/32Mhz = 1/32 us =0.03125us
                            //7us/0.03125us = 224
    //割込み許可
    PIR1bits.ADIF=0;
    PIE1bits.ADIE=1;
    ADCON0bits.ON=1;//注：以降、OFFにしないこと。
    //Trigger Timer begins
    timer2Init();
}

//******************************************************//
// * Clock Reference Output 初期化
// *****************************************************//
void CLKREF_Init(void)
{
    CLKRCONbits.DC = 0b10; //duty 50%
    //CLKRCONbits.DC = 0b01; //duty 25%
    CLKRCONbits.DIV = 0b110; //divided by 64 I2CCLK:0.5Mhz/5=100kHz
    //CLKRCONbits.DIV = 0b101; //divided by 32 I2CCLK:1Mhz/5=200kHz
    //CLKRCONbits.DIV = 0b100;   //divided by 16 I2CCLK:2Mhz/5=400kHz
    //CLKRCONbits.DIV = 0b011; //divided by  8 I2CCLK:4Mhz/5=800kHz
    //CLKRCONbits.DIV = 0b010; //divided by  4 I2CCLK:8Mhz/5=1600kHz
    CLKRCLK = 0b00000; //Sorce is Fosc
    //CLKRCLK = 0b00001; //Sorce is HFINTOSC
    CLKRCONbits.EN=1;
    __delay_ms(10);
}