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
    Canvas(int w, int h);

    int width, height;
    std::vector<Tuple> pixels;

    void write_pixel(int x, int y, const Tuple& color, int size);
    void save_to_PPM(const std::string& filename);

};

#endif // CANVAS_H