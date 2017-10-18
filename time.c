/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher
 * Copyright: GPL V2
 * See http://www.gnu.org/licenses/gpl.html
 *
 * functions to convert ntp timestamps into time and date
 *********************************************/
#include <string.h>
#include <stdio.h>

#include "tm_stm32_delay.h"
#include "time.h"

// EPOCH = Jan 1 1970 00:00:00
#define EPOCH_YR    1970
//(24L * 60L * 60L)
#define SECS_DAY    86400UL
#define LEAPYEAR(year)  (!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define YEARSIZE(year)  (LEAPYEAR(year) ? 366 : 365)

static const char day_abbrev[] = "SunMonTueWedThuFriSat";

// isleapyear = 0-1
// month=0-11
// return: how many days a month has
//
// We could do this if ram was no issue:
//uint8_t monthlen(uint8_t isleapyear,uint8_t month){
//const uint8_t mlen[2][12] = {
//      { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
//      { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
//  };
//  return(mlen[isleapyear][month]);
//}
//
static uint8_t monthlen(uint8_t isleapyear, uint8_t month){
    if(month==1)
        return (28+isleapyear);
    if(month>6)
        month--;
    if(month%2==1)
        return (30);
    return (31);
}

//
//
//
const char *asctime(const struct tm *tm){
	static char asctime[ 30 ];
	char dstr[4] = { 0, 0, 0, 0 };
	uint8_t i = 0;

	while( i < 3 ){
		dstr[i] = (day_abbrev[tm->tm_wday * 3 + i]);
		i++;
	}
	memset( asctime, 0, sizeof(asctime) );
	sprintf( asctime, "%s %d.%02d.%02d %02d:%02d:%02dUTC", dstr, tm->tm_year, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec );
	return asctime;
}

static uint32_t offset = 0;

// Sets the system's idea of the time and date. The time, pointed to by t,
// is measured in seconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
void stime(time_t t){
	offset = t - HAL_GetTick()/1000;
}

// returns the time as the number of seconds since the Epoch,
// 1970-01-01 00:00:00 +0000 (UTC).
__attribute__((used)) time_t __time(void){
	return HAL_GetTick()/1000 + offset;
}

// The gmtime() function converts the calendar time timep to broken-down
// time representation, expressed in Coordinated Universal Time (UTC).
const struct tm* gmtime(time_t time){
	static struct tm tm;
	uint32_t dayclock = time % SECS_DAY;

	tm.tm_mday = time / SECS_DAY;
	tm.tm_year = EPOCH_YR;
	tm.tm_sec = time % 60UL;
	tm.tm_min = (time % 3600UL) / 60;
	tm.tm_hour = dayclock / 3600UL;
	tm.tm_wday = (tm.tm_mday + 4) % 7;  /* day 0 was a thursday */

	while (tm.tm_mday >= YEARSIZE(tm.tm_year)) {
		tm.tm_mday -= YEARSIZE(tm.tm_year);
		tm.tm_year++;
	}
	tm.tm_mon = 0;
	while (tm.tm_mday >= monthlen(LEAPYEAR(tm.tm_year),tm.tm_mon)) {
		tm.tm_mday -= monthlen(LEAPYEAR(tm.tm_year),tm.tm_mon);
		tm.tm_mon++;
	}
	tm.tm_mday++;
	return &tm;
}
