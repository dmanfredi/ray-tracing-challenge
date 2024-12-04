#include <iostream>
#include <cmath>
#include <limits>
#include <vector>
#include <fstream>
#include <cstdint>

#include "tuple.h"

#ifndef CANVAS_H
#define CANVAS_H

class Canvas {

public:
    Canvas(int w, int h, Tuple c = Tuple::color(0.0, 0.0, 0.0));

    int width, height;
    Tuple color;
    std::vector<Tuple> pixels;

    void write_pixel(int x, int y, const Tuple& color, int size);
    void save_to_PPM(const std::string& filename);
    Tuple pixel_at(int x, int y) const;

};

#endif // CANVAS_H