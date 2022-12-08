/********************************************************************************
* button.hpp: Innehåller funktionalitet för enkel implementering av tryckknappar
*             via klassen button samt associerade funktioner. Denna klass
*             fungerar också utmärkt för andra digitala inportar där insignalen
*             ska kunna läsas av samt avbrott ska kunna genereras vid ett
*             godtyckligt event.
********************************************************************************/
#ifndef BUTTON_HPP_
#define BUTTON_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"

/********************************************************************************
* button: Klass för implementering av tryckknappar och andra digitala inportar.
*         PCI-avbrott kan aktiveras på aktuell pin. Därmed får eventdetektering
*         implementeras av användaren, då PCI-avbrott inte möjliggör kontroll
*         av vilken flank som avbrott ska ske på.
********************************************************************************/
class button
{
private:
   uint8_t pin_ = 0x00;                   /* Tryckknappens pin-nummer på aktuell I/O-port. */
   enum io_port io_port_ = io_port::none; /* I/O-port som lysdioden är ansluten till. */
   bool interrupt_enabled_ = false;       /* Indikerar ifall PCI-avbrott är aktiverat. */

public:
   /********************************************************************************
   * button: Initierar ny tryckknapp på angiven pin.
   *
   *         - pin : Tryckknappens pin-nummer på Arduino Uno, exempelvis 13.
   *                 Alternativt kan motsvarande port-nummer på ATmega328P anges,
   *                 exempelvis B5 för pin 13 eller D3 för pin 3.
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
   * ~button: Nollställer tryckknapp samt motsvarande pin.
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
   * interrupt_enabled: Indikerar ifall PCI-avbrott är aktiverat på knappens pin.
   ********************************************************************************/
   bool interrupt_enabled(void) const
   {
      return this->interrupt_enabled_;
   }

   /********************************************************************************
   * is_pressed: Läser av tryckknappens pin och indikerar ifall denna är nedtryckt. 
   *             I så fall returneras true, annars false.
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
   * enable_interrupt: Aktiverar PCI-avbrott på angiven tryckknapp så att event på
   *                   tryckknappens pin medför avbrott, både på stigande och 
   *                   fallande flank. Kontroll av flank måste skötas av användaren, 
   *                   förslagsvis via avläsning i motsvarande avbrottsrutin.
   *
   *                   Nedan visas sambandet mellan använd I/O-port samt
   *                   avbrottsvektorn för motsvarande avbrottsrutin:
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
   * disable_interrupt: Inaktiverar PCI-avbrott på angiven tryckknapp.
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
   * toggle_interrupt: Togglar aktivering av PCI-avbrott på angiven tryckknapp.
   *                   Om avbrott är aktiverat vid anrop så sker inaktivering.
   *                   På samma sätt gäller att om avbrott är inaktiverat vid anrop
   *                   så sker aktivering.
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