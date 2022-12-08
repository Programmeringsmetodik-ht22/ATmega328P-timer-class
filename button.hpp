/********************************************************************************
* button.hpp: Inneh�ller funktionalitet f�r enkel implementering av tryckknappar
*             via klassen button samt associerade funktioner. Denna klass
*             fungerar ocks� utm�rkt f�r andra digitala inportar d�r insignalen
*             ska kunna l�sas av samt avbrott ska kunna genereras vid ett
*             godtyckligt event.
********************************************************************************/
#ifndef BUTTON_HPP_
#define BUTTON_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"

/********************************************************************************
* button: Klass f�r implementering av tryckknappar och andra digitala inportar.
*         PCI-avbrott kan aktiveras p� aktuell pin. D�rmed f�r eventdetektering
*         implementeras av anv�ndaren, d� PCI-avbrott inte m�jligg�r kontroll
*         av vilken flank som avbrott ska ske p�.
********************************************************************************/
class button
{
private:
   uint8_t pin_ = 0x00;                   /* Tryckknappens pin-nummer p� aktuell I/O-port. */
   enum io_port io_port_ = io_port::none; /* I/O-port som lysdioden �r ansluten till. */
   bool interrupt_enabled_ = false;       /* Indikerar ifall PCI-avbrott �r aktiverat. */

public:
   /********************************************************************************
   * button: Initierar ny tryckknapp p� angiven pin.
   *
   *         - pin : Tryckknappens pin-nummer p� Arduino Uno, exempelvis 13.
   *                 Alternativt kan motsvarande port-nummer p� ATmega328P anges,
   *                 exempelvis B5 f�r pin 13 eller D3 f�r pin 3.
   ********************************************************************************/
   button(const uint8_t pin)
   {
      if (pin >= 0 && pin <= 7)
      {
         this->io_port_ = io_port::d;
         this->pin_ = pin;
         PORTD |= (1 << this->pin_);
      }
      else if (pin >= 8 && pin <= 13)
      {
         this->io_port_ = io_port::b;
         this->pin_ = pin - 8;
         PORTB |= (1 << this->pin_);
      }
      else if (pin >= 14 && pin <= 19)
      {
         this->io_port_ = io_port::c;
         this->pin_ = pin - 14;
         PORTC |= (1 << this->pin_);
      }
      else
      {
         this->io_port_ = io_port::none;
         this->pin_ = 0;
      }

      this->interrupt_enabled_ = false;
      return;
   }

   /********************************************************************************
   * ~button: Nollst�ller tryckknapp samt motsvarande pin.
   ********************************************************************************/
   ~button(void)
   {
      this->disable_interrupt();

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

      this->io_port_ = io_port::none;
      this->pin_ = 0;
      return;
   }

   /********************************************************************************
   * pin: Returnerar tryckknappens pin-nummer.
   ********************************************************************************/
   uint8_t pin(void) const
   {
      return this->pin_;
   }

   /********************************************************************************
   * get_port: Returnerar tryckknappens I/O-port.
   ********************************************************************************/
   io_port get_port(void) const
   {
      return this->io_port_;
   }

   /********************************************************************************
   * interrupt_enabled: Indikerar ifall PCI-avbrott �r aktiverat p� knappens pin.
   ********************************************************************************/
   bool interrupt_enabled(void) const
   {
      return this->interrupt_enabled_;
   }

   /********************************************************************************
   * is_pressed: L�ser av tryckknappens pin och indikerar ifall denna �r nedtryckt. 
   *             I s� fall returneras true, annars false.
   ********************************************************************************/
   bool is_pressed(void) const
   {
      if (this->io_port_ == io_port::b)
      {
         return (PINB & (1 << this->pin_));
      }
      else if (this->io_port_ == io_port::c)
      {
         return (PINC & (1 << this->pin_));
      }
      else if (this->io_port_ == io_port::d)
      {
         return (PIND & (1 << this->pin_));
      }
      else
      {
         return false;
      }
   }
   
   /********************************************************************************
   * enable_interrupt: Aktiverar PCI-avbrott p� angiven tryckknapp s� att event p�
   *                   tryckknappens pin medf�r avbrott, b�de p� stigande och 
   *                   fallande flank. Kontroll av flank m�ste sk�tas av anv�ndaren, 
   *                   f�rslagsvis via avl�sning i motsvarande avbrottsrutin.
   *
   *                   Nedan visas sambandet mellan anv�nd I/O-port samt
   *                   avbrottsvektorn f�r motsvarande avbrottsrutin:
   *
   *                   I/O-port     pin (Arduino Uno)     Avbrottsvektor
   *                      B              8 - 13             PCINT0_vect
   *                      C             A0 - A5             PCINT1_vect
   *                      D              0 - 7              PCINT2_vect
   ********************************************************************************/
   void enable_interrupt(void)
   {
      asm("SEI");

      if (this->io_port_ == io_port::b)
      {
         PCICR |= (1 << PCIE0);
         PCMSK0 |= (1 << this->pin_);
      }
      else if (this->io_port_ == io_port::c)
      {
         PCICR |= (1 << PCIE1);
         PCMSK1 |= (1 << this->pin_);
      }
      else if (this->io_port_ == io_port::d)
      {
         PCICR |= (1 << PCIE2);
         PCMSK2 |= (1 << this->pin_);
      }

      this->interrupt_enabled_ = true;
      return;
   }

   /********************************************************************************
   * disable_interrupt: Inaktiverar PCI-avbrott p� angiven tryckknapp.
   ********************************************************************************/
   void disable_interrupt(void)
   {
      if (this->io_port_ == io_port::b)
      {
         PCMSK0 &= ~(1 << this->pin_);
      }
      else if (this->io_port_ == io_port::c)
      {
         PCMSK1 &= ~(1 << this->pin_);
      }
      else if (this->io_port_ == io_port::d)
      {
         PCMSK2 &= ~(1 << this->pin_);
      }

      this->interrupt_enabled_ = false;
      return;
   }

   /********************************************************************************
   * toggle_interrupt: Togglar aktivering av PCI-avbrott p� angiven tryckknapp.
   *                   Om avbrott �r aktiverat vid anrop s� sker inaktivering.
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

};

#endif /* BUTTON_HPP_ */