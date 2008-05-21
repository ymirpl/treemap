/** 
@file main.cc

Plik z funkcja main() do cwiczenia 4 na laboratoriach z AISDI.

@author
Pawel Cichocki, Michal Nowacki

@date  
last revision
- 2005.12.01 Michal Nowacki: lab #4
- 2005.10.27 Pawel Cichocki: added some comments
- 2005.10.26 Michal Nowacki: creation - separated from another file

COPYRIGHT:
Copyright (c) 2005 Instytut Informatyki, Politechnika Warszawska
ALL RIGHTS RESERVED
*******************************************************************************/

#include <iostream>

#ifdef _SUNOS
#include "/home/common/dyd/aisdi/tree/TreeMap.h"
#include "timer.h"
#else
#include "TreeMap.h"
#endif

int CCount::count=0;

int Test2();
void test();

int main()
{
   struct time_m czasstart;
   double czas;
   std::cout << "AISDI tree: wchodze do funkcji main." << std::endl;
   test();
   // Biblioteka z bardziej rygorystyczna wersja tych testow bedzie udostepniona na nastepnych zajeciach.
   czasstart = timer_start();
   Test2();
   czas = timer_stop( czasstart );
   std::cout << std::endl << "Czas wykonania Test2() : " << czas << " s." << std::endl;
   //system("PAUSE");
   return EXIT_SUCCESS;
}
