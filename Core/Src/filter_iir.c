#include "filter_iir.h"

void Filter_Init(Filter2ndOrder_t *f, float b0, float b1, float b2, float a1, float a2) {
    f->b0 = b0; f->b1 = b1; f->b2 = b2;
    f->a1 = a1; f->a2 = a2;
    f->x1 = 0; f->x2 = 0;
    f->y1 = 0; f->y2 = 0;
}

float Filter_Update(Filter2ndOrder_t *f, float input) {
    // Obliczenie wyjścia filtra
    float output = f->b0 * input + f->b1 * f->x1 + f->b2 * f->x2
                                 - f->a1 * f->y1 - f->a2 * f->y2;

    // Przesunięcie próbek w pamięci
    f->x2 = f->x1;
    f->x1 = input;
    f->y2 = f->y1;
    f->y1 = output;

    return output;
}
