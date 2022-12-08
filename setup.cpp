/********************************************************************************
* setup.cpp: Inneh�ller funktionalitet f�r initiering av det inbyggda systemet.
*            Lysdioder initieras p� pin 8 - 9, tryckknappar initieras med 
*            aktiverade PCI-avbrott p� pin 12 - 13 och samtliga timerkretsar 
*            initieras, d�r timer 0 s�tts till att l�pa ut efter 300 ms (f�r att 
*            tempor�rt inaktivera PCI-avbrott p� I/O-port B efter nedtryckning 
*            f�r att undvika multipla avbrott orsakat av kontaktstudsar, medan 
*            timer 1 - 2 s�tts till att l�pa ut efter 100 ms (f�r blinkning via 
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