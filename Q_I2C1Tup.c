#include "Q_I2C1Tup.h"


//*************************************************//
//** I2C1初期化
//*************************************************//
void I2C1_Init(void)
{
    //Set Clear Buffer Flag
    I2C1STAT1bits.CLRBF = 1;//I2Cバッファ、RXBF、TXBEビットクリア  
    I2C1CON0=0x00;
    I2C1CON0bits.MODE=0b100;//Host mode, 7bits address
    I2C1CON1=0x00;
    I2C1CON1bits.ACKCNT=1;  //最終ACK確認で返す値0:ACK 1:NACK
    I2C1CON2=0x00;
    I2C1CON2bits.ABD=0;     //I2C1ADB1をアドレスバッファとして使用
    I2C1CON2bits.BFRET=0b00;//8clocksスタートになる準備ﾊﾟﾙｽ数e
    I2C1CON2bits.FME = 0;   //FI2Cclk/5
    //I2C1CON2bits.FME = 1;     //FI2Cclk/4
    //I2C1CLK=0b0011;       //MFINTOSC500kHz//I2C用クロックソース
    I2C1CLK = 0b0100;       //Clock Reference Output//400KHz,500KHz
    I2C1PIR=0x00;
    //Bus Time out
    I2C1BTO=0b0010;//Timer4
    I2C1ERRbits.BTOIE=1;
    I2C1ERRbits.BTOIF=0;
    
    //PPS-------------------------
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0;
    //I2C1-------------------------
    //RC4 for SDA
    RC4PPS=0x38;
    I2C1SDAPPS=0x14;
    //RC3 for SCL
    RC3PPS = 0x37;
    I2C1SCLPPS=0x13;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 1;
    
    //IO I2Cpin setting---------------
    RC3I2Cbits.TH=0b00;
    RC4I2Cbits.TH=0b00;
    RC3I2Cbits.SLEW=0b11;
    RC4I2Cbits.SLEW=0b11;
    
    I2C1CON0bits.EN=1;      //I2C1モジュール有効
}

/**
 * I2C1_b1Write
 * 1バイト書込み
 * @param _deviceAdd　Cilentアドレス＋RWbit
 * @param _data1    送信データ
 * @return 送信：成功と失敗
 */
bool I2C1_b1Write(uint8_t _deviceAdd, uint8_t _data1)
{
   
    //タイムアウト設定-------------
    PIR11bits.TMR4IF=0;
    PIE11bits.TMR4IE=1;
    T4CONbits.TMR4ON=1;
    I2C1CON0bits.EN=1;//タイムアウトからの復帰
    
    //送信前設定-----------------
    I2C1ADB1 = _deviceAdd;  //ｱﾄﾞﾚｽﾊﾞｯﾌｧにClientアドレスをセット
    I2C1CNT=1;              //送信データ数をセット
    I2C1TXB=_data1;         //送信データを1バイトだけ、前もってセット
    
    //I2Cバスが解放されているか？
    while(!I2C1STAT0bits.BFRE)
    {
       if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    
    //スタートコンディションの発令-----
    I2C1CON0bits.S=1;       //Startコンディション
   
    
    //送信バッファが空か？（セットしたデータの送信完了）
    while(!I2C1STAT1bits.TXBE)
    {
       if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    //Stopコンディションになったか？
    while(I2C1PIRbits.PC1IF==0)
    {
       if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    I2C1PIRbits.PC1IF=0;//Stopフラグクリア
    //I2C1CON0bits.S=0;
    //Host mode active を見て終了確認 0:not active 1:active
    while(I2C1STAT0bits.MMA)
    {
        if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    I2C1STAT1bits.CLRBF = 1;//I2Cバッファ、RXBF,TXBEのクリア
    PIE11bits.TMR4IE=0;
    T4CONbits.TMR4ON=0;
    TMR4=0;
    
    return true;
}

/**
 * I2C1_b2Write
 * I2C 2バイト送信
 * @param _deviceAdd Clientアドレス
 * @param _data1　送信データ１
 * @param _data2　送信データ２
 * @return 
 */
uint8_t I2C1_b2Write(uint8_t _deviceAdd, uint8_t _data1, uint8_t _data2)
{
    //タイムアウト設定-------------
    PIR11bits.TMR4IF=0;
    PIE11bits.TMR4IE=1;
    T4CONbits.TMR4ON=1;
    I2C1CON0bits.EN=1;
    
    //送信前設定-----------------
    I2C1ADB1 = _deviceAdd;  //Clientデバイスアドレスセット
    I2C1CNT=2;              //送信データ数
    I2C1CON1bits.ACKCNT=1;  //最終ACK確認で返す値0:ACK 1:NACK
    I2C1TXB=_data1;         //送信バッファに最初のデータをセット
    
    //I2Cバスが解放されているか？
    while(!I2C1STAT0bits.BFRE)
    {
         if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    
    //スタートコンディション
    I2C1CON0bits.S=1;
    
    //最初のデータ送信完了？
    while(I2C1STAT1bits.TXBE!=1)
    {
        if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    //while(!I2C1CON1bits.ACKSTAT);
    I2C1TXB=_data2;         //次の送信データをセット
    //送信完了？------------
    while(I2C1STAT1bits.TXBE!=1)
    {
        if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    //ストップコンディション確定？
    while(I2C1PIRbits.PC1IF==0)
    {
        if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    I2C1PIRbits.PC1IF=0;//ストップフラグクリア
    I2C1CON0bits.S=0;
    //Host mode Active確認 0:not Active 1:active
    while(I2C1STAT0bits.MMA)
    {
         if(I2C1ERRbits.BTOIF)
        {
           I2C_TimeOUT_OP();
            return false;
        }
    };

    I2C1STAT1bits.CLRBF = 1;//I2C1バッファ、TXBE,RXBFクリア
    PIE11bits.TMR4IE=0;
    T4CONbits.TMR4ON=0;
    
    
    return true;
}

/**
 * I2C1_b1Read
 * I2C　1バイト読み込み
 * @param _deviceAdd Clientアドレス
 * @param _address 読み出しアドレス
 * @return 
 */
uint8_t I2C1_b1Read(uint8_t _deviceAdd,uint8_t _address)
{
    uint8_t ret;
   
    //タイムアウト始動-------------
    PIE11bits.TMR4IE=1;
    T4CONbits.TMR4ON=1;
    I2C1CON0bits.EN=1;
    //送信前設定-------------------
    I2C1ADB1 = _deviceAdd;  //Clientアドレスセット
    I2C1CNT=1;              //送信バイト数(この場合は、引数の_address)
    I2C1TXB=_address;       //送信バイトセット(読み出し先のアドレスセット）
    
    //バスフリーチェック------------
    while(!I2C1STAT0bits.BFRE)//I2Cバス解放？確認
    {
        if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    
     //リスタート--------------------
    I2C1CON0bits.RSEN=1;    //リスタート開始
    
    //スタートコンディション--------
    I2C1CON0bits.S=1;       //Startコンディションセット
    //送信完了待ち------------------
    while(I2C1STAT1bits.TXBE!=1)//送信バッファ 0:full 1:empty 送信完了確認
    {
        if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    //-----------------------------
    I2C1PIRbits.PC1IF=0;
    
    
    while(!I2C1CON0bits.MDR)//RSENセットで１になる。(読み出しでクリア）
    {
        if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    //スタートコンディション--------
    I2C1ADB1 = _deviceAdd|0x01; //Clientｱﾄﾞﾚｽ＋R/Wビット
    I2C1CNT=1;                  //読み出し数
    I2C1CON0bits.S=1;           //Startコンディションセット
   
    while(!I2C1STAT1bits.RXBF)  //読み込み完了待ち？
    {
        if(I2C1ERRbits.BTOIF)
        {
            I2C_TimeOUT_OP();
            return false;
        }
    };
    ret=I2C1RXB;                //バッファから読み出し
    I2C1CON0bits.RSEN=0;        //リスタート解除       
    I2C1CON0bits.S=0;
    I2C1PIR=0x00;               //PIRクリア
    while(I2C1STAT0bits.MMA)    //HostmodeActime 0:not Active 1:active
    {
        if(I2C1ERRbits.BTOIF)
        {
           I2C_TimeOUT_OP();
            return false;
        }
    };
    //Set Clear Buffer Flag
    I2C1STAT1bits.CLRBF = 1;    //I2C1バッファクリア
    PIE11bits.TMR4IE=0;
    T4CONbits.TMR4ON=0;
    
    return ret;
}

/**
 * I2C_TimeOUT_OP
 * I2C通信タイムアウト後の処理関数
 */
void I2C_TimeOUT_OP(void)
{
     LATBbits.LATB0=0;
    I2C1ERRbits.BTOIF=0;
    PIE11bits.TMR4IE=0;
    T4CONbits.TMR4ON=0;
    TMR4=0x00;
    I2C1CON0bits.S=0;
    I2C1STAT1bits.CLRBF = 1;
    I2C1CON0bits.EN=0;
}