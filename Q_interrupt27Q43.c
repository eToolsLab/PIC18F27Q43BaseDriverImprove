#include "Q_interrupt27Q43.h"

//****************割り込み関数***********************//
// USART1割込み
//**************************************************//
void __interrupt(irq(IRQ_U1RX)) USART1RX_ISR(void)
{
    uint8_t ch;
    PIR4bits.U1RXIF=0;
    
    ch=getch();
    USART1.rx.buf[USART1.rx.length++]=ch;
    if(USART1.rx.length>RXUSART_BUFFER_SIZE)
    {
        USART1.rx.length=0;
        USART1.rx.overflow=1;
    }
    if(USART1.rx.buf[USART1.rx.length-1]==0x0a 
            && USART1.rx.buf[USART1.rx.length-2]==0x0D && USART1.rx.length>=3 
            && USART1.rx.overflow!=1)
    {
        USART1.rx.buf[USART1.rx.length-2]=0x00;
        PIE4bits.U1RXIE=0;
        USART1.rx.completed=1;
    }
    if(USART1.rx.overflow && ch==0x0A)
    {
        USART1.rx.length=0;
        USART1.rx.overflow=0;
    }
}

//****************割り込み関数***********************//
// Timer0割込み
//**************************************************//
void __interrupt(irq(IRQ_TMR0)) Timer0_ISR(void)
{
    PIR3bits.TMR0IF=0;
    tm0.cnt++;
    if(tm0.cnt==50)
    {
        tm0.cnt=0;
        tm0.fg=true;
        T0CON0bits.EN=0;
        PIE3bits.TMR0IE=0;
    }
    
    TMR0L=0xF0;//10ms
}
//****************割り込み関数***********************//
// Timer2割込み
//**************************************************//
void __interrupt(irq(IRQ_TMR2)) Timer2_ISR(void)
{
    PIR3bits.TMR2IF=0;
    //ADCON0bits.GO = 1; //Start conversion
    //while (ADCON0bits.GO); //Wait for conversion done
}

//****************割り込み関数***********************//
// Timer4割込み
//**************************************************//
void __interrupt(irq(IRQ_TMR4)) Timer4_ISR(void)
{
    PIR11bits.TMR4IF=0;
    //PORTBbits.RB2=~PORTBbits.RB2;
   // PIE11bits.TMR4IE=1;
   // T4CONbits.TMR4ON=1;
}

//****************割り込み関数***********************//
// IOC割込み
// ※印は、割り込み関数実行中の変化を取りこぼさないためのコード。
//**************************************************//
void __interrupt(irq(IRQ_IOC)) IOC_ISR(void)
{
    uint8_t val,mask;
    
    PIR0bits.IOCIF=0;
    val=IOCCF;  //※IOCフラグレジスタ読み出し
    mask=0xFF;  //※ゼロクリアマスク
    mask=mask^val;//現状を記録
    printf("IOCCF:%02X mask:%02X\n", IOCCF, mask);
    if(IOCCNbits.IOCCN0==1 && val==0x01)
    {
        ioc.fg=true; //main関数で実処理開始
        PIE0bits.IOCIE=0;//一時割り込み停止
    }
    //zero clear--------------    
    IOCCF&=mask;   //※状態変化の取りこぼしをしない。
    printf("IOCCF:%02X\n",IOCCF);
}

//****************割り込み関数***********************//
// ADC割込み
//**************************************************//
void __interrupt(irq(IRQ_AD)) ADC_ISR(void)
{
    PIR1bits.ADIF=0;
    adcInfo.val[0]=ADRES;
    adcInfo.completed=true;//mainで処理
    //main関数で復帰
    PIE3bits.TMR2IE=0;
    T2CONbits.TMR2ON=0;
    PIE1bits.ADIE=0;
    //ADCON0bits.ON=0;これを有効にすると
    //acquisitonTime, pretimeを使用できない。                  
}