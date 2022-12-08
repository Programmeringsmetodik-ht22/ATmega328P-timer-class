/********************************************************************************
* timer.hpp: Inneh�ller funktionalitet f�r implementering av interruptbaserade
*            timerkretsar via klassen timer. Dessa timerkretsar fungerar ocks� 
*            utm�rkt att anv�nda som r�knare.
********************************************************************************/
#ifndef TIMER_HPP_
#define TIMER_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"

/********************************************************************************
* timer: Klass f�r implementering av interruptbaserade timerkretsar, som vid
*        behov kan anv�ndas som r�knare.
********************************************************************************/
class timer
{
public:
   enum class sel; /* F�rdeklaration av enumerationsklass f�r val av timerkrets. */
private:
   volatile uint32_t counter_ = 0;                            /* 32-bitars r�knare. */
   uint32_t max_count_ = 0;                                   /* Maxv�rde som uppr�kning ska ske till. */
   sel timer_sel_ = sel::none;                                /* Val av timerkrets. */
   bool interrupt_enabled_ = false;                           /* Indikerar ifall timergenererat avbrott �r aktiverat. */
   static constexpr auto TIME_BETWEEN_INTERRUPTS_MS_ = 0.128; /* 0.128 ms mellan varje timergenererat avbrott. */

   /********************************************************************************
   * get_max_count: Returnerar antalet timergenererade avbrott som kr�vs f�r
   *                angiven tid, avrundad till n�rmaste heltal.
   *
   *                - time_ms: �nskad tid m�tt i millisekunder.
   ********************************************************************************/
   static inline uint32_t get_max_count(const double time_ms)
   {
      return static_cast<uint32_t>(time_ms / timer::TIME_BETWEEN_INTERRUPTS_MS_ + 0.5);
   }

   /********************************************************************************
   * init_circuit: Initierar angiven timerkrets. Timer 0 samt Timer 2 initieras 
   *               i Normal Mode, medan Timer 1 initieras i CTC Mode med uppr�kning 
   *               till 256. Vid aktiverat avbrott p� godtycklig initierad timer 
   *               sker timergenererat avbrott var 0.128:e millisekund.
   *
   *               - timer_sel: Timerkretsen som ska initieras.
   ********************************************************************************/
   static void init_circuit(const sel timer_sel)
   {
      if (timer_sel == sel::timer0)
      {
         TCCR0B = (1 << CS01);
      }
      else if (timer_sel == sel::timer1)
      {
         TCCR1B = (1 << CS11) | (1 << WGM12);
         OCR1A = 256;
      }
      else if (timer_sel == sel::timer2)
      {
         TCCR2B = (1 << CS21);
      }

      asm("SEI");
      return;
   }

public:

   /********************************************************************************
   * timer: Initierar ny timerkrets med angiven tid m�tt i millisekunder.
   *
   *        - timer_sel: Val av timerkrets.
   *        - time_ms  : Tiden timern ska s�ttas p� m�tt i millisekunder.
   ********************************************************************************/
   timer(const sel timer_sel, 
         const double time_ms)
   {
      this->timer_sel_ = timer_sel;
      this->max_count_ = this->get_max_count(time_ms);
      this->init_circuit(this->timer_sel_);
      return;
   }

   /********************************************************************************
   * ~timer: St�nger av angiven timerkrets innan den raderas. 
   ********************************************************************************/
   ~timer(void)
   {
      this->reset();
      return;
   }

   /********************************************************************************
   * counter: Returnerar lagrat v�rde fr�n angiven timers r�knare.
   ********************************************************************************/
   uint32_t counter(void) const
   {
      return this->counter_;
   }

   /********************************************************************************
   * max_cout: Returnerar det v�rde som angiven timer ska r�kna upp till.
   ********************************************************************************/
   uint32_t max_count(void) const
   {
      return this->max_count_;
   }

   /********************************************************************************
   * timer_sel: Returnerar anv�nd timerkrets via en enumerator av
   *            enumerationsklassen timer::sel.
   ********************************************************************************/
   enum sel timer_sel(void) const
   {
      return this->timer_sel_;
   }

   /********************************************************************************
   * enabled: Indikerar ifall timergenererat avbrott �r aktiverat p� angiven timer.
   ********************************************************************************/
   bool interrupt_enabled(void) const
   {
      return this->interrupt_enabled_;
   }

   /********************************************************************************
   * enable_interrupt: Aktiverar timergenererat avbrott, som �ger rum n�r timern
   *                   r�knar upp till overflow eller specificerat max.
   *
   *                   Timer 0 samt Timer 2 aktiveras i Normal Mode, medan Timer 1
   *                   aktiveras i CTC Mode med uppr�kning till 256, vilket g�r
   *                   att tiden mellan varje timergenererat avbrott �r samma 
   *                   oavsett anv�nd timerkrets.
   *
   *                   Avbrottsvektorer f�r timerkretsarna deklareras nedan:
   *
   *                   Timerkrets     Avbrottsvektor
   *                     Timer 0     TIMER0_OVF_vect
   *                     Timer 1     TIMER1_COMPA_vect
   *                     Timer 2     TIMER2_OVF_vect
   ********************************************************************************/
   void enable_interrupt(void)
   {
      if (this->timer_sel_ == sel::timer0)
      {
          TIMSK0 = (1 << TOIE0);
      }
      else if (this->timer_sel_ == sel::timer1)
      {
         TIMSK1 = (1 << OCIE1A);
      }
      else if (this->timer_sel_ == sel::timer2)
      {
         TIMSK2 = (1 << TOIE2);
      }

      this->interrupt_enabled_ = true;
      return;
   }

   /********************************************************************************
   * disable_interrupt: Inaktiverar timergenererat avbrott p� angiven timer.
   ********************************************************************************/
   void disable_interrupt(void)
   {
      if (this->timer_sel_ == sel::timer0)
      {
         TIMSK0 = 0;
      }
      else if (this->timer_sel_ == sel::timer1)
      {
         TIMSK1 = 0;
      }
      else if (this->timer_sel_ == sel::timer2)
      {
         TIMSK2 = 0;
      }

      this->interrupt_enabled_ = false;
      return;
   }

   /********************************************************************************
   * toggle_interrupt: Togglar aktivering av timergenererat avbrott p� angiven 
   *                   timer. Om avbrott �r aktiverat vid anrop sker inaktivering.
   *                   P� samma s�tt g�ller att om avbrott �r inaktiverat vid anrop 
   *                   s� sker aktivering.
   ********************************************************************************/
   void toggle_interrupt(void)
   {
      if (this->interrupt_enabled_)
      {
         this->disable_interrupt();
      }
      else
      {
         this->enable_interrupt();
      }
      return;
   }

   /********************************************************************************
   * count: R�knar upp angiven timer.
   ********************************************************************************/
   void count(void)
   {
      this->counter_++;
      return;
   }

   /********************************************************************************
   * elapsed: Indikerar ifall angiven timer har l�pt ut genom att returnera true
   *          eller false. Ifall timern har l�pt ut nollst�lls r�knaren inf�r
   *          n�sta uppr�kning.
   ********************************************************************************/
   bool elapsed(void)
   {
      if (this->counter_ >= this->max_count_)
      {
         this->counter_ = 0;
         return true;
      }
      else
      {
         return false;
      }
   }

   /********************************************************************************
   * reset: �terst�ller angiven timer till startl�get.
   ********************************************************************************/
    void reset(void)
    {
       this->disable_interrupt();
       this->counter_ = 0;
       return;
    }

   /********************************************************************************
   * set_time_ms: S�tter ny tid p� angiven timerkrets m�tt i millisekunder.
   * 
   *               - new_time_ms: Tiden timern ska s�ttas p� i millisekunder.
   ********************************************************************************/
   void set_time_ms(const double new_time_ms)
   {
      this->max_count_ = this->get_max_count(new_time_ms);
      return;
   }

   /********************************************************************************
   * set_max_count: S�tter nytt maxv�rde f�r uppr�kning av timern n�r denna ska
   *                anv�ndas som en r�knare.
   *
   *                - new_max_count: Maxv�rde f�r uppr�kningen.
   ********************************************************************************/
   void set_max_count(const uint32_t new_max_count)
   {
      this->max_count_ = new_max_count;
      return;
   }

   /********************************************************************************
   * sel: Enumeration f�r val av timerkrets.
   ********************************************************************************/
   enum class sel
   {
      timer0, /* Timer 0. */
      timer1, /* Timer 1. */
      timer2, /* Timer 2. */
      none    /* Timer ospecificerad. */
   };
};

#endif /* TIMER_HPP_ */