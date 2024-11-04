#include <iostream>
#include "methods.h"
int main() {
    try {
        BMP image("input.bmp");

        BMP imageCounterClockwise("input.bmp");

        image.Rotate90();
        image.Save("output_clockwise.bmp");

        imageCounterClockwise.RotateCounter90();
        imageCounterClockwise.Save("output_counterclockwise.bmp");


        image.GaussianFilter();
        image.Save("output_gaussian.bmp");

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
