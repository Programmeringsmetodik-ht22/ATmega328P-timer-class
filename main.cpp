/********************************************************************************
* main.cpp: Demonstration av datastruktur för implementering av timerkretsar
*           för inbyggda system. Två lysdioder anslutna till pin 8 - 9 (PORTB0
*           - PORTB1) togglas mellan att vara avstängda samt blinka via toggling
*           med timerkretsar timer1 samt timer2, där timer1 togglar lysdioden
*           ansluten till pin 8 (PORTB0) och timer2 togglar lysdioden ansluten
*           till pin 9 (PORTB1). När timergenererat avbrott är aktiverat på en
*           given timer blinkar motsvarande lysdiod, annars hålls denna släckt.
*
*           För att toggla aktivering av timergenererade avbrott på timer-
*           kretsarna används två tryckknappar anslutna till pin 12 - 13 
*           (PORTB4 - PORTB4), som PCI-avbrott har aktiverats på. 
*           Vid nedtryckning av tryckknappen ansluten till pin 12 (PORTB4) 
*           togglas timergenererat avbrott på timer1, medan nedtryckning av 
*           tryckknappen ansluten till pin 13 (PORTB5) medför toggling av 
*           timergenererat avbrott på timer2.
*
*           För att undvika multipla avbrott på grund av kontaktstudsar vid
*           nedtryckning av tryckknapparna inaktiveras PCI-avbrott på 
*           I/O-port B i 300 ms efter att avbrott sker. Räkningen genomförs 
*           av timerkrets timer 0, som timergenererat avbrott aktiveras då 
*           aktiveras på. Efter 300 ms har passerat återställs PCI-avbrott på 
*           I/O-port B och timergenererat avbrott på timer 0 inaktiveras inför
*           nästa nedtryckning av tryckknapparna.
********************************************************************************/
#include "header.hpp"

/********************************************************************************
* main: Initierar systemet vid start, vilket innefattar initiering av lysdioder,
*       tryckknappar och timerkretsar. Programmet är i övrigt avbrottsgenererat.
********************************************************************************/
int main(void)
{
   setup();

   while (1)
   {

   }

   return 0;
}

