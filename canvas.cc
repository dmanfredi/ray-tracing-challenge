
#include <fstream>
#include <cstdint>

#include "tuple.h"
#include "canvas.h"

// Canvas constructor definition
Canvas::Canvas(int w, int h) : width(w), height(h), pixels(w * h) {
    for (auto& pixel : pixels) {
        pixel = Tuple::color(0.019, 0.029, 0.140);
    }
}

void Canvas::write_pixel(int x, int y, const Tuple& color, int size = 1) 
{
    for (int dy = 0; dy < size; dy++) {
        for (int dx = 0; dx < size; dx++) {
            int px = x + dx;
            int py = y + dy;
            if (px >= 0 && px < width && py >= 0 && py < height) {
                pixels[py * width + px] = color;
            }
        }
    }
}

void Canvas::save_to_PPM(const std::string& filename) {
	std::ofstream file(filename);
	
	// Write the header
	file << "P3\n" << width << " " << height << "\n255\n";
	
	// Write pixel data
	for (const auto& pixel : pixels) {
		Tuple::RGB rgb = pixel.to_RGB();

		file << static_cast<int>(rgb.r) << " "
			 << static_cast<int>(rgb.g) << " "
			 << static_cast<int>(rgb.b) << "\n";
	}
	file.close();
	// Open the file with IrfanView
	std::string command = "start \"\" \"C:\\Program Files (x86)\\IrfanView\\i_view32.exe\" \"" + filename + "\"";
	system(command.c_str());
}
