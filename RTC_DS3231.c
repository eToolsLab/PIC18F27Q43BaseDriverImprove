#include "RTC_DS3231.h"

_DSecData DSecData;
_DhourData DhourData;
_A1Sec_data A1Sec_data; 
_A1Min_data A1Min_data; 
_A1Hour_data A1Hour_data;
_A1DAY_DATE_data A1DAY_DATE_data;
_Control_data Control_data;
_Status_data Status_data;
_Rtc Rtc;
_rtcAlarmData RtcAlarmData;

void initDS3231()
{
    setDS3231Control(); 
	//setDS3231Time(0x19,0x10,0x21,0x09,0x09,0x00);
	setTimeSystem(mode24);	
}

void decimalToHex(uint8_t val)
{
    
}

//mode:12 or 24 hour indicator
void setTimeSystem(uint8_t mode)
{
	DhourData.byt=I2C1_b1Read(RTCDeviceAdd,DHour);
	//printf("DhourData.byt=%x\n\r",DhourData.byt);
	if(mode==mode12)
        DhourData.tmode=mode12;
	else
		DhourData.tmode=mode24;
	//printf("DhourData.byt=%x\n\r",DhourData.byt);
	I2C1_b2Write(RTCDeviceAdd,DHour,DhourData.byt);
}


void setDS3231Time(uint8_t y, uint8_t m, uint8_t date, uint8_t h, uint8_t min, uint8_t sec)
{
	if(!I2C1_b2Write(RTCDeviceAdd,DYear,y))
        printf("Error1 ");
	if(!I2C1_b2Write(RTCDeviceAdd,DMonth,m))
        printf("Error2 ");
    if(!I2C1_b2Write(RTCDeviceAdd,DDate,date))
        printf("Error3 ");
    if(!I2C1_b2Write(RTCDeviceAdd,DHour,h))
        printf("Error4 ");
    if(!I2C1_b2Write(RTCDeviceAdd,DMin,min))
        printf("Error5 ");
    if(!I2C1_b2Write(RTCDeviceAdd,DSec,sec))
        printf("Error6 ");
    setTimeSystem(mode24);	
}


//dydt: bit7:dy/dt 0:a match of date of mounth:DateMode
//                 1:a match of day of week:WeekMode
//tmode: bit6:12/24 0:mode24 1:mode12
//mask: bit0:A1M1  bit1: A1M2 bit3:A1M3
//      1110:seconds matching
//      1100:minutes & seconds matching
void setDS3231Alarm1(uint8_t dydtmode, uint8_t tmode,
        uint8_t dydt, uint8_t h, uint8_t min, uint8_t sec, uint8_t mask)
{
    A1DAY_DATE_data.A1M4=(mask>>3)&0x01;;
	A1DAY_DATE_data.DYDT=dydtmode;
    A1DAY_DATE_data.val=dydt;
	if(!I2C1_b2Write(RTCDeviceAdd,A1DAY_DATE,A1DAY_DATE_data.byt))
        printf("Error01 ");
    A1Hour_data.A1M3=(mask>>2)&0x01;
    A1Hour_data.tmode=tmode;
    A1Hour_data.hour=h;
	if(!I2C1_b2Write(RTCDeviceAdd,A1Hour,A1Hour_data.byt))
        printf("Error11 ");
    A1Min_data.A1M2=(mask>>1)&0x01;
    A1Min_data.min=min;
    if(!I2C1_b2Write(RTCDeviceAdd,A1Min,A1Min_data.byt))
        printf("Error21 ");
    A1Sec_data.A1M1=mask&0x01;
    A1Sec_data.sec=sec;
    if(!I2C1_b2Write(RTCDeviceAdd,A1Sec,A1Sec_data.byt))
        printf("Error31 ");
    setTimeSystem(mode24);	
}

void setDS3231Control(void)
{
    Control_data.EOSC=0;
    Control_data.BBSQW=0;
    Control_data.CONV = 0;
    Control_data.RS=0;
    Control_data.INTCN=1;
    Control_data.A2IE=0;
    Control_data.A1IE=1;
    
    if(!I2C1_b2Write(RTCDeviceAdd,Control,Control_data.byt))
        printf("Error41 ");
    Status_data.byt=0x00;
    if(!I2C1_b2Write(RTCDeviceAdd,Status,Status_data.byt))
        printf("Error42 ");
    
}


//****************************************
//
//****************************************
void setDS3231_A1FClear()
{
    Status_data.byt=I2C1_b1Read(RTCDeviceAdd,Status);
    Status_data.byt&=0xFE;
    if(!I2C1_b2Write(RTCDeviceAdd,Status,Status_data.byt))
        printf("error52");
}

void stopDS3231(uint8_t st)
{
	uint8_t buf;
	DSecData.byt=I2C1_b1Read(RTCDeviceAdd,DSec);
	if(st)
	{
		DSecData.CH=1;//stop
	}
	else
	{
		DSecData.CH=0;
	}
	
	I2C1_b2Write(RTCDeviceAdd,DSec,DSecData.byt);
    __delay_ms(10);
}

void rtcDateRead(uint8_t _devAdd, uint8_t* buf)
{
    Rtc.Date[0]=I2C1_b1Read(RTCDeviceAdd,DYear);
    Rtc.Date[1]=I2C1_b1Read(RTCDeviceAdd,DMonth);
    Rtc.Date[2]=I2C1_b1Read(RTCDeviceAdd,DDate);
    buf[0]=(Rtc.Date[0]>>4)|0x30;
    buf[1]=(Rtc.Date[0]&0x0F)|0x30;
    buf[2]='/';
    buf[3]=(Rtc.Date[1]>>4)|0x30;
    buf[4]=(Rtc.Date[1]&0x0F)|0x30;
    buf[5]='/';
    buf[6]=(Rtc.Date[2]>>4)|0x30;
    buf[7]=(Rtc.Date[2]&0x0F)|0x30;
}

void rtcTimeRead(uint8_t _devAdd, uint8_t* buf)
{
    Rtc.Time[0]=I2C1_b1Read(RTCDeviceAdd,DHour);
    Rtc.Time[1]=I2C1_b1Read(RTCDeviceAdd,DMin);
    Rtc.Time[2]=I2C1_b1Read(RTCDeviceAdd,DSec);
    buf[0]=(Rtc.Time[0]>>4)|0x30;
    buf[1]=(Rtc.Time[0]&0x0F)|0x30;
    buf[2]=':';
    buf[3]=(Rtc.Time[1]>>4)|0x30;
    buf[4]=(Rtc.Time[1]&0x0F)|0x30;
    buf[5]=':';
    buf[6]=(Rtc.Time[2]>>4)|0x30;
    buf[7]=(Rtc.Time[2]&0x0F)|0x30;
}