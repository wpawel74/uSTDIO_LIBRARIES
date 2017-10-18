/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher
 * Copyright: GPL V2
 *
 *********************************************/
//@{
#ifndef GMTIME_H
#define GMTIME_H
#include <sys/types.h>

struct tm {
	int tm_sec;				/* Seconds.	[0-60] (1 leap second) */
	int tm_min;				/* Minutes.	[0-59] */
	int tm_hour;			/* Hours.	[0-23] */
	int tm_mday;			/* Day.		[1-31] */
	int tm_mon;				/* Month.	[0-11] */
	int tm_year;			/* Year	- 1900.  */
	int tm_wday;			/* Day of week.	[0-6] */
	int tm_yday;			/* Days in year.[0-365]	*/
	int tm_isdst;			/* DST.		[-1/0/1]*/
};

// Number of seconds between 1-Jan-1900 and 1-Jan-1970, unix time starts 1970
// and ntp time starts 1900.
#define GETTIMEOFDAY_TO_NTP_OFFSET 2208988800UL

const struct tm* gmtime(time_t time);

const char *asctime(const struct tm *tm);

void stime(time_t t);

time_t time(void);

#endif /* GMTIME_H */
//@}
