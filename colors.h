#ifndef COLORS_H
#define COLORS_H

#include "tuple.h"

namespace colors {
    inline const Tuple white = Tuple::color(1.0, 1.0, 1.0);

    inline const Tuple sky = Tuple::color(0.53, 0.81, 0.92);
    inline const Tuple orange = Tuple::color(1, 0.45, 0.05);
    inline const Tuple grass = Tuple::color(0.34, 0.68, 0.24);
    inline const Tuple sunset = Tuple::color(0.98, 0.54, 0.21);
    inline const Tuple bluehour = Tuple::color(0.019, 0.029, 0.140);
    inline const Tuple blood = Tuple::color(0.55, 0.0, 0.0);


    inline const Tuple black = Tuple::color(0.0, 0.0, 0.0);
    inline const Tuple evil_slate = Tuple::color(-0.1, -0.1, -0.1);
    inline const Tuple slate = Tuple::color(0.1, 0.1, 0.1);
    inline const Tuple midnight = Tuple::color(0.04, 0.04, 0.04);
}

#endif