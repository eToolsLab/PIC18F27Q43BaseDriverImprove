#include "Q_I2C1Tup.h"
#include "I2C_LCD.h"

uint8_t LCD_BackLight_EN[2];
_LCD_DATA Lcd;

#define LCDWAITMS 300
#define LCDWAITMS1 100

/**
 * LCD_send コマンド、データ送信関数
 * @param _device_add Clientアドレス
 * @param _data 上位4bit or 下位4bit
 * @param _mode CM 0:Command Mode 1:Data Mode
 */
bool LCD_send(uint8_t _device_add,uint8_t _data, uint8_t _mode)
{   
    bool ret=true;;
    
    OD.byt=0x00;
    
    if(_device_add==LcdDeviceAdd_AT)
          OD.LED=LCD_BackLight_EN[0];
    else
          OD.LED=LCD_BackLight_EN[1];
    
    OD.byt = OD.byt|(_data& 0xF0);
    ret = I2C1_b1Write(_device_add,OD.byt);
    if(!ret)return false;
    __delay_us(300);
    OD.RS=_mode;
    ret = I2C1_b1Write(_device_add,OD.byt);
    if(!ret)return false;
    __delay_us(250);
    OD.EN=High;
    ret = I2C1_b1Write(_device_add,OD.byt);
    if(!ret)return false;
   __delay_us(200);
    OD.EN=Low;
    ret = I2C1_b1Write(_device_add,OD.byt);
    if(!ret)return false;
    __delay_us(LCDWAITMS1);
    
    return ret;
}

/**
 * LCD_WriteData データ送信
 * @param _device_add
 * @param _data
 */
bool LCD_WriteData(uint8_t _device_add,uint8_t _data)
{
    bool ret=true;
    ret=LCD_send(_device_add, _data,DM);
    if(!ret)return false;
    ret=LCD_send(_device_add, _data<<4,DM);
    //__delay_us(40);
    return ret;
}
/**
 * LCD_Command コマンド送信
 * @param _device_add
 * @param cmd
 */
bool LCD_Command(uint8_t _device_add,uint8_t cmd)
{
    bool ret=true;
    ret=LCD_send(_device_add,cmd,CM);
    if(!ret)return false;
    ret=LCD_send(_device_add,cmd<<4,CM);
    __delay_us(40);
    return ret;
}
/**
 * LCD_Init　LCD初期化
 * @param _device_add Clientアドレス
 */
void LCD_Init(uint8_t _device_add)
{
    uint8_t ret;
    __delay_ms(15);
    ret=LCD_send(_device_add,0x30,CM);//8bits mode
    if(!ret)return;
    __delay_ms(5);
    ret=LCD_send(_device_add,0x30,CM);//8bits mode
    if(!ret)return;
	__delay_us(100);
	ret=LCD_send(_device_add,0x30,CM);//8bits mode three times setting
	if(!ret)return;
    ret=LCD_send(_device_add,0x20,CM);//4bits mode 
	if(!ret)return;
    ret=LCD_Command(_device_add,0x28);//Function setting.
	if(!ret)return;
    ret=LCD_Command(_device_add,0x08);//display off
	if(!ret)return;
    ret=LCD_Command(_device_add,0x01);//display clear
	if(!ret)return;
    ret=LCD_Command(_device_add,0x06);//entry
	if(!ret)return;
    ret=LCD_Command(_device_add,0x02);//cursor home
	if(!ret)return;
    ret=LCD_Command(_device_add,0x0C);//display on
    if(!ret)return;
    LCD_BackLight_EN[0]=LED_ON;
    LCD_BackLight_EN[1]=LED_ON;
    
}

/**
 * LCD_Printf 文字列表示関数
 * @param _device_add Clientアドレス
 * @param string 文字列
 * @param _length 文字列長
 * @param _DDRAM_Add DDRAMアドレス
 */
bool LCD_Printf(uint8_t _device_add, uint8_t *string, uint8_t _length, uint8_t _DDRAM_Add)
{
    bool ret=true;
    ret=LCD_Command(_device_add,_DDRAM_Add);
    if(!ret)return false;
    do{
        ret=LCD_WriteData(_device_add, *string);
        if(!ret)
            return false;
        string++;
    }while(--_length);
    return true;
}


/**
 * LCD_LED_Pow　バックライト点灯
 * @param _device_add
 */
void LCD_LED_Pow(uint8_t _device_add)
{
    uint8_t cmd=0x80;
    cmd|=0x08;
    LCD_send(_device_add,cmd,CM);
    LCD_send(_device_add,cmd<<4,CM);
    __delay_us(40);
}


