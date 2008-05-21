#include "timer.h"
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

struct time_m timer_start()
{
 struct time_m wyn;
 int who = RUSAGE_SELF;
 struct rusage usage;
 getrusage(who,&usage);
 wyn.sec=usage.ru_utime.tv_sec + usage.ru_stime.tv_sec;
 wyn.usec=usage.ru_utime.tv_usec + usage.ru_stime.tv_usec;
 if (wyn.usec > 1000000) {
	wyn.usec-=1000000;
	wyn.sec++;
 };
 return wyn;
};

double timer_stop(struct time_m old)
{
 struct time_m wyn;
 int who = RUSAGE_SELF;
 struct rusage usage;
 getrusage(who,&usage);
 wyn.sec=usage.ru_utime.tv_sec + usage.ru_stime.tv_sec;
 wyn.usec=usage.ru_utime.tv_usec + usage.ru_stime.tv_usec;
 if (wyn.usec > 1000000) {
	wyn.usec-=1000000;
	wyn.sec++;
 };
 wyn.sec-=old.sec;
 if (old.usec<=wyn.usec)
	{  wyn.usec-=old.usec; }
 else {
	if(wyn.sec>0) wyn.sec--;   //getrusage wywo³. w krótkim odstêpie czasu najwyraŸniej czasem "g³upieje"
	                           // bo na Solarisie (i tylko tu) bez tego sprawdzenia dostawa³em czasem czasy ujemne
	wyn.usec=1000000+wyn.usec-old.usec;
 };
 return wyn.sec + wyn.usec/1000000.0;
};


