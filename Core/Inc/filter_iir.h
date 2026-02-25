/*
 * filter_iir.h
 *
 *  Created on: Feb 25, 2026
 *      Author: WneQ
 */

#ifndef FILTER_IIR_H
#define FILTER_IIR_H

#include <stdint.h>

// Struktura filtra II rzędu (Biquad - Direct Form I)
typedef struct {
    // Współczynniki (wyliczone np. w MATLAB/Python)
    float b0, b1, b2;
    float a1, a2;

    // Bufory opóźniające (pamięć filtra)
    float x1, x2; // poprzednie wejścia
    float y1, y2; // poprzednie wyjścia
} Filter2ndOrder_t;

// Inicjalizacja filtra konkretnymi współczynnikami
void Filter_Init(Filter2ndOrder_t *f, float b0, float b1, float b2, float a1, float a2);

// Obliczenie kolejnej próbki
float Filter_Update(Filter2ndOrder_t *f, float input);

#endif // FILTER_IIR_H
