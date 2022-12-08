/********************************************************************************
* setup.cpp: Innehåller funktionalitet för initiering av det inbyggda systemet.
*            Lysdioder initieras på pin 8 - 9, tryckknappar initieras med 
*            aktiverade PCI-avbrott på pin 12 - 13 och samtliga timerkretsar 
*            initieras, där timer 0 sätts till att löpa ut efter 300 ms (för att 
*            temporärt inaktivera PCI-avbrott på I/O-port B efter nedtryckning 
*            för att undvika multipla avbrott orsakat av kontaktstudsar, medan 
*            timer 1 - 2 sätts till att löpa ut efter 100 ms (för blinkning via 
*            toggling av lysdioder).
********************************************************************************/
#include "header.hpp"

/* Definition av globala objekt: */
led l1(8);
led l2(9);    

button b1(12);
button b2(13);   

timer t0(timer::sel::timer0, 300); 
timer t1(timer::sel::timer1, 100);
timer t2(timer::sel::timer2, 100);

/********************************************************************************
* setup: Initierar det inbyggda systemet. 
********************************************************************************/
void setup(void)
{
   b1.enable_interrupt();
   b2.enable_interrupt();
   return;
}