/********************************************************************************
* adc.hpp: Inneh�ller funktionalitet f�r enkel implementering av AD-omvandling
*          f�r analoga signaler via klassen adc. AD-omvandling sker via 
*          inl�sning av signaler fr�n analoga pinnar A0 - A5 p�  Arduino Uno,
*          vilket motsvarar PORTC0 - PORTC5 p� ATmega328P.
*
*          Analoga insignaler mellan 0 - 5 V AD-omvandlas till digitala
*          motsvarigheter mellan 0 - 1023. Duty cycle kan anv�ndas f�r
*          PWM-generering och ber�knas enligt nedan:
*
*                         Duty cycle = ADC_result / ADC_MAX,
*
*          d�r ADC_result �r resultat avl�st fr�n AD-omvandlaren OCH ADC_MAX
*          utg�r h�gsta m�jliga avl�sta v�rde, vilket �r 1023.0.
********************************************************************************/
#ifndef ADC_HPP_
#define ADC_HPP_

/********************************************************************************
* adc: Klass f�r implementering av AD-omvandlare, som m�jligg�r avl�sning
*      av insignaler fr�n analoga pinnar, ber�kning av on- och off-tid f�r
*      PWM-generering (tiden som PWM-styrd utsignal ska vara h�g respektive l�g).
********************************************************************************/
class adc
{
private:
   uint8_t pin_ = 0;                        /* Analog pin som skall anv�ndas f�r avl�sning. */
   uint16_t pwm_on_us_ = 0;                 /* On-tid f�r PWM-generering i mikrosekunder. */
   uint16_t pwm_off_us_ = 0;                /* Off-tid f�r PWM-generering i mikrosekunder. */
   static constexpr auto ADC_MAX_ = 1023.0; /* H�gsta digitala v�rde vid AD-omvandling. */
public:

   /********************************************************************************
   * adc: Initierar analog pin f�r avl�sning och AD-omvandling av insignaler.
   *
   *      - pin : Analog pin som skall l�sas f�r AD-omvandling, som antingen kan
   *              anges som ett tal mellan 0 - 5 eller via konstanter A0 - A5
                  (som motsvarar heltal 14 - 19).
   ********************************************************************************/
   adc(const uint8_t pin)
   {
      if (pin >= 0 && pin <= 5)
      {
         this->pin_ = pin;
      }
      else if (pin >= 14 && pin <= 19)
      {
         this->pin_ = pin - 14;
      }
      
      static_cast<void>(this->read());
      return;
   }

   /********************************************************************************
   * pin: Returnerar anv�nd analog pin.
   ********************************************************************************/
   uint8_t pin(void) const
   {
      return this->pin_;
   }

   /********************************************************************************
   * pwm_on_us: Returnerar aktuell on-tid f�r PWM-generering i mikrosekunder.
   ********************************************************************************/
   uint16_t pwm_on_us(void) const
   {
      return this->pwm_on_us_;
   }

   /********************************************************************************
   * pwm_off_us: Returnerar aktuell off-tid f�r PWM-generering i mikrosekunder.
   ********************************************************************************/
   uint16_t pwm_off_us(void) const
   {
      return this->pwm_off_us_;
   }

   /********************************************************************************
   * max_val: Returnerar h�gsta m�jliga AD-omvandlade v�rde.
   ********************************************************************************/
   double max_val(void) const
   {
      return this->ADC_MAX_;
   }

   /********************************************************************************
   * read: L�ser av en analog insignal och returnerar motsvarande digitala
   *       motsvarighet mellan 0 - 1023.
   ********************************************************************************/
   uint16_t read(void) const
   {
      ADMUX = (1 << REFS0) | this->pin_;
      ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
      while ((ADCSRA & (1 << ADIF)) == 0);
      ADCSRA = (1 << ADIF);
      return ADC;
   }

   /********************************************************************************
   * duty_cycle: L�ser av en analog insignal och returnerar motsvarande duty cycle
   *             som ett flyttal mellan 0 - 1.
   ********************************************************************************/
   double duty_cycle(void) const
   {
      return this->read() / this->ADC_MAX_;
   }

   /********************************************************************************
   * get_pwm_values: L�ser av en analog insignal och ber�knar on- och off-tid f�r
   *                 f�r PWM-generering, avrundat till n�rmaste heltal.
   *
   *                 - pwm_period_us: PWM-perioden (on-tid + off-tid) m�tt i
   *                                  mikrosekunder (default = 10 000 us).
   ********************************************************************************/
   void get_pwm_values(const uint16_t pwm_period_us = 10000)
   {
      this->pwm_on_us_ = static_cast<uint16_t>(this->duty_cycle() * pwm_period_us + 0.5);
      this->pwm_off_us_ = pwm_period_us - this->pwm_on_us_;
      return;
   }

};

#endif /* ADC_HPP_ */