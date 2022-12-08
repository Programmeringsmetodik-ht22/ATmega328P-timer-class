/********************************************************************************
* led.hpp: Inneh�ller funktionalitet f�r enkel implementering av lysdioder via
*          klassen led. Denna klass fungerar �ven f�r andra digitala utportar.
********************************************************************************/
#ifndef LED_HPP_
#define LED_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"

/********************************************************************************
* led: Klass f�r implementering av lysdioder och andra digitala utportar.
********************************************************************************/
class led
{
private:
   uint8_t pin_ = 0;                 /* Lysdiodens pin-nummer p� aktuell I/O-port. */
   io_port io_port_ = io_port::none; /* I/O-port som lysdioden �r ansluten till. */
   bool enabled_ = false;            /* Indikerar ifall lysdioden �r t�nd. */

public:

   /********************************************************************************
   * led: Initierar ny lysdiod p� angiven pin.
   *
   *      - pin      : Lysdiodens pin-nummer p� Arduino Uno, exempelvis 8.
   *                   Alternativt kan motsvarande port-nummer p� ATmega328P
   *                   anges, exempelvis B0 f�r pin 8 eller D2 f�r pin 2.
   *      - start_val: Lysdiodens startv�rde (default = 0, dvs. sl�ckt).
   ********************************************************************************/
   led(const uint8_t pin,
       const uint8_t start_val = 0)
   {
      if (pin >= 0 && pin <= 7)
      {
         this->io_port_ = io_port::d;
         this->pin_ = pin;
         DDRD |= (1 << this->pin_);
      }
      else if (pin >= 8 && pin <= 13)
      {
         this->io_port_ = io_port::b;
         this->pin_ = pin - 8;
         DDRB |= (1 << this->pin_);
      }
      else if (pin >= 14 && pin <= 19)
      {
         this->io_port_ = io_port::c;
         this->pin_ = pin - 14;
         DDRC |= (1 << this->pin_);
      }
      else
      {
         this->io_port_ = io_port::none;
         this->pin_ = 0;
      }

      this->enabled_ = false;
      if (start_val) this->on();
      return;
   }

   /********************************************************************************
   * ~led: Nollst�ller lysdiod samt motsvarande pin.
   ********************************************************************************/
   ~led(void)
   {
      if (this->io_port_ == io_port::b)
      {
         PORTB &= ~(1 << this->pin_);
         DDRB &= ~(1 << this->pin_);
      }
      else if (this->io_port_ == io_port::c)
      {
         PORTC &= ~(1 << this->pin_);
         DDRC &= ~(1 << this->pin_);
      }
      else if (this->io_port_ == io_port::d)
      {
         PORTD &= ~(1 << this->pin_);
         DDRD &= ~(1 << this->pin_);
      }

      this->pin_ = 0;
      this->io_port_ = io_port::none;
      this->enabled_ = false;
      return;
   }

   /********************************************************************************
   * pin: Returnerar lysdiodens pin-nummer.
   ********************************************************************************/
   uint8_t pin(void) const
   {
      return this->pin_;
   }

   /********************************************************************************
   * get_port: Returnerar lysdiodens I/O-port.
   ********************************************************************************/
   io_port get_port(void) const
   {
      return this->io_port_;
   }

   /********************************************************************************
   * enabled: Indikerar ifall lysdioden �r t�nd.
   ********************************************************************************/
   bool enabled(void) const
   {
      return this->enabled_;
   }

   /********************************************************************************
   * on: T�nder angiven lysdiod.
   ********************************************************************************/
   void on(void)
   {
      if (this->io_port_ == io_port::b)
      {
         PORTB |= (1 << this->pin_);
      }
      else if (this->io_port_ == io_port::c)
      {
         PORTC |= (1 << this->pin_);
      }
      else if (this->io_port_ == io_port::d)
      {
         PORTD |= (1 << this->pin_);
      }

      this->enabled_ = true;
      return;
   }

   /********************************************************************************
   * off: Sl�cker angiven lysdiod.
   ********************************************************************************/
   void off(void)
   {
      if (this->io_port_ == io_port::b)
      {
         PORTB &= ~(1 << this->pin_);
      }
      else if (this->io_port_ == io_port::c)
      {
         PORTC &= ~(1 << this->pin_);
      }
      else if (this->io_port_ == io_port::d)
      {
         PORTD &= ~(1 << this->pin_);
      }

      this->enabled_ = false;
      return;
   }

   /********************************************************************************
   * toggle: Togglar utsignalen p� angiven lysdiod. Om lysdioden �r sl�ckt vid
   *         anropet s� t�nds den. P� samma s�tt g�ller att om lysdioden �r t�nd
   *         vid anropet s� sl�cks den.
   ********************************************************************************/
   void toggle(void)
   {
      if (this->enabled_)
      {
         this->off();
      }
      else
      {
         this->on();
      }

      return;
   }

   /********************************************************************************
   * blink: Blinkar lysdiod en g�ng med angiven blinkhastighet.
   *
   *        - blink_speed_ms: Referens till blinkhastigheten m�tt i millisekunder.
   ********************************************************************************/
   void blink(const uint16_t& blink_speed_ms)
   {
      auto delay_ms = [](const uint16_t& delay_time_ms)
      {
         for (uint16_t i = 0; i < delay_time_ms; ++i)
         {
            _delay_ms(1);
         }
      };

      this->toggle();
      delay_ms(blink_speed_ms);
      return;
   }
};

#endif /* LED_HPP_ */