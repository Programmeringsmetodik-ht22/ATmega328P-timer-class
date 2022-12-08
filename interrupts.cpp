/********************************************************************************
* interrupts.cpp: Inneh�ller avbrottsrutiner.
********************************************************************************/
#include "header.hpp"

/********************************************************************************
* ISR (PCINT0_vect): Avbrottsrutin som �ger rum vid nedtryckning/uppsl�ppning
*                    av n�gon av tryckknapparna. Beroende p� vilken tryckknapp
*                    som trycks ned s� togglas antingen timer 1 eller timer 2.
*                    Vid uppsl�ppning av en tryckknapp g�rs ingenting.
*
*                    Oavsett vad som orsakade avbrottet inaktiveras PCI-avbrott
*                    p� I/O-port B i 300 millisekunder via timer 0 f�r att
*                    undvika multipla avbrott orsakade av kontaktstudsar.
********************************************************************************/
ISR (PCINT0_vect)
{
   misc::disable_pin_change_interrupt(io_port::b);
   t0.enable_interrupt();

   if (b1.is_pressed())
   {
      t1.toggle_interrupt();
      if (!t1.interrupt_enabled())
      {
         l1.off();
      }
   }
   else if (b2.is_pressed())
   {
      t2.toggle_interrupt();
      if (!t2.interrupt_enabled())
      {
         l2.off();
      }
   }

   return;
}

/********************************************************************************
* ISR (TIMER0_OVF_vect): Avbrottsrutin som �ger rum vid overflow av timer 0,
*                        dvs. uppr�kning till 256, vilket sker var 0.128:e
*                        millisekund n�r timern �r aktiverad.
*
*                        Timern r�knas upp via uppr�kning av varje passerat
*                        avbrott. N�r timern l�per ut (n�r ber�knat antal
*                        avbrott f�r specificerad tid har r�knats upp) s�
*                        �teraktiveras PCI-avbrott p� I/O-port B (som har)
*                        st�ngts av i 300 millisekunder f�r att undvika
*                        multipla avbrott orsakat av kontaktstudsar), f�ljt
*                        av att timern st�ngs av.
********************************************************************************/
ISR (TIMER0_OVF_vect)
{
   t0.count();

   if (t0.elapsed())
   {
      misc::enable_pin_change_interrupt(io_port::b);
      t0.disable_interrupt();
   }

   return;
}

/********************************************************************************
* ISR (TIMER1_COMPA_vect): Avbrottsrutin som �ger rum vid uppr�kning till 256 av
*                          timer 1 i CTC Mode, vilket sker var 0.128:e
*                          millisekund n�r timern �r aktiverad.
*
*                          Timern r�knas upp via uppr�kning av varje passerat
*                          avbrott. N�r timern l�per ut (n�r ber�knat antal
*                          avbrott f�r specificerad tid har r�knats upp) s�
*                          togglas lysdiod 1.
********************************************************************************/
ISR (TIMER1_COMPA_vect)
{
   t1.count();

   if (t1.elapsed())
   {
      l1.toggle();
   }

   return;
}

/********************************************************************************
* ISR (TIMER2_OVF_vect): Avbrottsrutin som �ger rum vid overflow av timer 2,
*                        dvs. uppr�kning till 256, vilket sker var 0.128:e
*                        millisekund n�r timern �r aktiverad.
*
*                        Timern r�knas upp via uppr�kning av varje passerat
*                        avbrott. N�r timern l�per ut (n�r ber�knat antal
*                        avbrott f�r specificerad tid har r�knats upp) s�
*                        togglas lysdiod 2.
********************************************************************************/
ISR (TIMER2_OVF_vect)
{
   t2.count();

   if (t2.elapsed())
   {
      l2.toggle();
   }

   return;
}
