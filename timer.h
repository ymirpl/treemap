#ifndef _TIMER_H
#define _TIMER_H

struct time_m { unsigned int sec,usec; };

struct time_m timer_start();
/* zwraca czas w milisekundach */
double timer_stop(struct time_m);

#endif

