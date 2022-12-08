/********************************************************************************
* vector.hpp: Implementering av dynamiska vektorer via klassen vector.
********************************************************************************/
#ifndef VECTOR_HPP_
#define VECTOR_HPP_

/* Inkluderingsdirektiv: */
#include "misc.hpp"

/********************************************************************************
* vector: Generisk klass f�r dynamiska vektorer av valfri datatyp.
********************************************************************************/
template<class T>
class vector
{
protected:
   T* data_ = nullptr; /* Pekare till ett f�lt inneh�llande lagrad data. */
   size_t size_ = 0;   /* Vektorns storlek, dvs. antalet lagrade element. */
public:

   /*****************************************************************************
   * vector: Tom konstruktor, initierar en ny tom vektor. 
   *****************************************************************************/
   vector(void) { }

   /*****************************************************************************
   * vector: Konstruktor, initierar ny vektor av angiven storlek med angivet
   *         startv�rde.
   * 
   *         - start_size: Vektorns nya storlek (antalet element).
   *         - start_val : Referens till startv�rde f�r samtliga element 
   *                       (default = 0).
   *****************************************************************************/
   vector(const size_t start_size,
          const T& start_val = static_cast<T>(0))
   {
      this->resize(start_size, start_val);
      return;
   }

   /*****************************************************************************
   * ~vector: Destruktor, frig�r minne allokerat f�r vektorn innan radering.
   *****************************************************************************/
   ~vector(void) 
   {
      this->clear();
      return;
   } 

   /*****************************************************************************
   * data: Returnerar en pekare till inneh�llet lagrat i angiven vektor.
   *****************************************************************************/
   T* data(void) const
   {
      return this->data_;
   }

   /*****************************************************************************
   * size: Returnerar arrayens storlek (antalet element) i angiven vektor.
   *****************************************************************************/
   size_t size(void) const 
   { 
      return this->size_; 
   }

   /*****************************************************************************
   * begin: Returnerar adressen till det f�rsta elementet i angiven vektor.
   *****************************************************************************/
   T* begin(void) const
   {
      return this->data_;
   }

   /*****************************************************************************
   * end: Returnerar adressen direkt efter det sista elementet i angiven vektor.
   *****************************************************************************/
   T* end(void) const
   {
      return this->data_ + this->size_;
   }

   /*****************************************************************************
   * last: Returnerar adressen till det sista elementet i angiven vektor.
   *****************************************************************************/
   T* last(void) const
   {
      return this->data_ ? this->end() - 1 : nullptr;
   }

   /*****************************************************************************
   * clear: T�mmer angiven vektor.
   *****************************************************************************/
   void clear(void)
   {
      free(this->data_);
      this->data_ = nullptr;
      this->size_ = 0;
      return;
   }

   /*****************************************************************************
   * resize: �ndrar storlek p� angiven vektor med angivet startv�rde, d�r
   *         startv�rdet �r satt till 0 som default. Ifall vektorns storlek
   *         lyckas �ndras till �nskad storlek returneras 0, annars felkod 1.
   * 
   *         - new_size : Vektorns nya storlek (antalet element).
   *         - start_val: Referens till startv�rde f�r respektive element 
   *                      (default = 0).
   *****************************************************************************/
   int resize(const size_t new_size,
              const T& start_val = static_cast<T>(0))
   {
      if (new_size == 0)
      {
         this->clear();
         return 0;
      }

      auto copy = (T*)realloc(this->data_, sizeof(T) * new_size);
      if (!copy) return 1;
      this->data_ = copy;
      this->size_ = new_size;

      for (auto& i : *this)
      {
         i = start_val;
      }

      return 0;
   }

   /*****************************************************************************
   * push: L�gger till ett nytt element l�ngst bak i angiven vektor. Ifall
   *       minnesallokeringen lyckas s� returneras 0, annars felkod 1.
   * 
   *       - new_element: Referens till det nya element som skall l�ggas till.
   *****************************************************************************/
   int push(const T& new_element)
   {
      auto copy = (T*)realloc(this->data_, sizeof(T) * (this->size_ + 1));
      if (!copy) return 1;
      copy[this->size_++] = new_element;
      this->data_ = copy;
      return 0;
   }

   /*****************************************************************************
   * pop: Tar bort det sista elementet i angiven vektor om ett s�dant finns.
   *      Om h�gst ett element finns lagrat i vektor s� t�ms den helt.
   *****************************************************************************/
   void pop(void)
   {
      if (this->size <= 1)
      {
         this->clear();
      }
      else
      {
         auto copy = (T*)realloc(this->data_, sizeof(T) * (this->size_ - 1));
         if (!copy) return;
         this->data_ = copy;
         this->size_--;
      }

      return;
   }
};

#endif /* VECTOR_HPP_ */