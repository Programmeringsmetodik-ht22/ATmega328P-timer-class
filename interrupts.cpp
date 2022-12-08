/********************************************************************************
* interrupts.cpp: Innehåller avbrottsrutiner.
********************************************************************************/
#include "header.hpp"

/********************************************************************************
* ISR (PCINT0_vect): Avbrottsrutin som äger rum vid nedtryckning/uppsläppning
*                    av någon av tryckknapparna. Beroende på vilken tryckknapp
*                    som trycks ned så togglas antingen timer 1 eller timer 2.
*                    Vid uppsläppning av en tryckknapp görs ingenting.
*
*                    Oavsett vad som orsakade avbrottet inaktiveras PCI-avbrott
*                    på I/O-port B i 300 millisekunder via timer 0 för att
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
* ISR (TIMER0_OVF_vect): Avbrottsrutin som äger rum vid overflow av timer 0,
*                        dvs. uppräkning till 256, vilket sker var 0.128:e
*                        millisekund när timern är aktiverad.
*
*                        Timern räknas upp via uppräkning av varje passerat
*                        avbrott. När timern löper ut (när beräknat antal
*                        avbrott för specificerad tid har räknats upp) så
*                        återaktiveras PCI-avbrott på I/O-port B (som har)
*                        stängts av i 300 millisekunder för att undvika
*                        multipla avbrott orsakat av kontaktstudsar), följt
*                        av att timern stängs av.
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
* ISR (TIMER1_COMPA_vect): Avbrottsrutin som äger rum vid uppräkning till 256 av
*                          timer 1 i CTC Mode, vilket sker var 0.128:e
*                          millisekund när timern är aktiverad.
*
*                          Timern räknas upp via uppräkning av varje passerat
*                          avbrott. När timern löper ut (när beräknat antal
*                          avbrott för specificerad tid har räknats upp) så
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
* ISR (TIMER2_OVF_vect): Avbrottsrutin som äger rum vid overflow av timer 2,
*                        dvs. uppräkning till 256, vilket sker var 0.128:e
*                        millisekund när timern är aktiverad.
*
*                        Timern räknas upp via uppräkning av varje passerat
*                        avbrott. När timern löper ut (när beräknat antal
*                        avbrott för specificerad tid har räknats upp) så
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
