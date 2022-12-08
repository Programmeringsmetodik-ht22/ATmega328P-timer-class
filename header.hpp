/********************************************************************************
* header.hpp: Innehåller globala objekt och deklarationer för implementering
*             av det inbyggda systemet.
********************************************************************************/
#ifndef HEADER_HPP_
#define HEADER_HPP_

/* Inkluderingsdirektiv: */
#include "led.hpp"
#include "button.hpp"
#include "timer.hpp"

/* Deklaration av globala objekt: */
extern led l1, l2;       /* Lysdioder. */
extern button b1, b2;    /* Tryckknappar. */
extern timer t0, t1, t2; /* Timerkretsar. */

/********************************************************************************
* setup: Initierar det inbyggda systemet.
********************************************************************************/
void setup(void);

#endif /* HEADER_HPP_ */