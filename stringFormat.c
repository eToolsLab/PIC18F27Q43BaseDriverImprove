#include "stringFormat.h"

char strFmt[10];

//*****************************************//
//*uint8ToAscii
//uint8_t val:元値
//uint8_t *txt:格納文字配列ポインタ
//*****************************************//
void uint8ToAscii(uint8_t val,uint8_t *txt)
{
    txt[0]=(val/100)|0x30;
    txt[1]=(val%100/10)|0x30;
    txt[2]=(val%10)|0x30;
}

//*****************************************//
//*int8ToAscii
//uint8_t val:元値
//uint8_t *txt:格納文字配列ポインタ
//*****************************************//
void int8ToAscii(int8_t val,uint8_t *txt)
{
    if( val>=0)
    {
        txt[0]='+';
        txt[1]=(val/100)|0x30;
        txt[2]=(val%100/10)|0x30;
        txt[3]=(val%10)|0x30;
    }else
    {
        txt[0]='-';
        val*=-1;
        txt[1]=(val/100)|0x30;
        txt[2]=(val%100/10)|0x30;
        txt[3]=(val%10)|0x30;
    }
}

//*****************************************//
//*intToAscii
//uint16_t val:元値
//uint8_t *txt:格納文字配列ポインタ
//*****************************************//
void uint16ToAscii(uint16_t val,uint8_t *txt)
{
    txt[0]=(val/1000)|0x30;
    txt[1]=((val%1000/100))|0x30;
    txt[2]=((val%100))/10|0x30;
    txt[3]=((val%10))|0x30;
}
//*****************************************//
//*intToAscii
//uint16_t val:元値
//uint8_t *txt:格納文字配列ポインタ
//*****************************************//
void int16ToAscii(int16_t val,uint8_t *txt)
{
    txt[0]=(val/1000)|0x30;
    txt[1]=((val%1000/100))|0x30;
    txt[2]=((val%100))/10|0x30;
    txt[3]=((val%10))|0x30;
}


//*****************************************//
//*asciiToIntArray
//　"369"(0x33,0x36,0x39)→ret[0]=3, ret[1]=9, ret[2]=9
//uint8_t* buf:文字列
//uint8_t* ret:
//*****************************************//
void asciiToIntArray(uint8_t* buf, uint8_t* ret)
{
    char i;
    uint8_t a,b;
    i=0;
    do{
        a=buf[2*i]<<4;
        b=a|(buf[2*i+1]&0x0F);
        ret[i]=b;
        i++;
    }while(i<6);
}
