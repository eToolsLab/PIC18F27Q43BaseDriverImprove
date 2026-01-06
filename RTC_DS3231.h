/* 
 * File:   RTC_DS3231_26k22.h
 * Author: h
 *
 * Created on 2019/12/12, 21:50
 */

#ifndef RTC_DS3231_26K22_H
#define	RTC_DS3231_26K22_H

#ifdef	__cplusplus
extern "C" {
#endif
/* 
 * File:   RTC_DS3231_26k22.h
 * Author: h
 *
 * Created on 2019/12/12
 */
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "Q_I2C1Tup.h"
#include "Q_peripheral27Q43.h"
    
#define stop 1
#define go	0
#define mode12 1
#define mode24 0
#define DateMode 0//date alarm
#define WeekMode 1//week alarm
#define RTCDeviceAdd 0xD0

typedef union {
	uint8_t byt;
	struct {
		uint8_t sec:7;
		uint8_t CH:1;
	};
}_DSecData;
extern _DSecData DSecData;
#define DSec	0x00
#define DMin	0x01
typedef union{
	uint8_t byt;
	struct {
		uint8_t  hour:6;
		uint8_t  tmode:1;
		uint8_t  non:1;
	};
}_DhourData;
_DhourData DhourData;
#define DHour 	0x02
#define DDay	0x03
#define DDate	0x04
#define DMonth  0x05
#define DYear	0x06
//Alerm1 section------------------------------
typedef union{
    uint8_t byt;
    struct{
        uint8_t sec:7;
        uint8_t A1M1:1;
    };
}_A1Sec_data;
extern _A1Sec_data A1Sec_data; 
#define A1Sec    0x07

typedef union{
    uint8_t byt;
    struct{
        uint8_t min:7;
        uint8_t A1M2:1;
    };
}_A1Min_data;
extern _A1Min_data A1Min_data; 
#define A1Min    0x08

typedef union {
    uint8_t byt;
    struct {
        uint8_t hour:6;
        uint8_t tmode:1;
        uint8_t A1M3:1;
    };
}_A1Hour_data;
extern _A1Hour_data A1Hour_data;
#define A1Hour   0x09

typedef union {
    uint8_t byt;
    struct{
        uint8_t val:6;
        uint8_t DYDT:1;
        uint8_t A1M4:1;
    };
}_A1DAY_DATE_data;
extern _A1DAY_DATE_data A1DAY_DATE_data;
#define A1DAY_DATE 0x0A
//Control Register
typedef union {
    uint8_t byt;
    struct{
        unsigned A1IE:1;
        unsigned A2IE:1;
        unsigned INTCN:1;
        unsigned RS:2;
        unsigned CONV:1;
        unsigned BBSQW:1;
        unsigned EOSC:1;
    };
}_Control_data;
extern _Control_data Control_data;
#define Control 0x0E
typedef union {
    uint8_t byt;
    struct {
        unsigned A1F:1;
        unsigned A2F:1;
        unsigned BSY:1;
        unsigned EN32:1;
        unsigned non:2;
        unsigned OSF:1;
    };
}_Status_data;
extern _Status_data Status_data;
#define Status  0x0F

///////////////////////

//-------------------
extern void initDS3231(void);
extern void setTimeSystem(uint8_t mode);
extern void setDS3231Time(uint8_t y, uint8_t m, uint8_t date, uint8_t h, uint8_t min, uint8_t sec);
extern void setDS3231Alarm1(uint8_t dydtmode, uint8_t tmod,
        uint8_t dydt, uint8_t h, uint8_t min, uint8_t sec, uint8_t mask);
extern void setDS3231Control(void);
extern void setDS3231_A1FClear(void);
extern void stopDS3231(uint8_t st);

extern void rtcDateRead(uint8_t _devAdd, uint8_t* buf);
extern void rtcTimeRead(uint8_t _devAdd, uint8_t *buf);

extern unsigned char RtcTime[3];

typedef struct
{
    uint8_t TM[6];
    uint8_t Date[3];
    uint8_t Time[3];
    uint8_t AlarmTime;
}_Rtc;
extern _Rtc Rtc;

typedef struct
{
    uint8_t min;
    uint8_t cnt;
    uint8_t fg;
}_rtcAlarmData;
extern  _rtcAlarmData RtcAlarmData;

#ifdef	__cplusplus
}
#endif

#endif	/* RTC_DS3231_26K22_H */

