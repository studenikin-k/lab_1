#include "bmp.h"
#include <iostream>
#include "methods.h"

int main(){
  try {
    const std::string nameFile = "input.bmp";

    BMP image(nameFile);

    image.rotate90();
    image.save("output_rotate_90.bmp");

    const std::string nameFile2 = "tower.bmp";

    BMP image2(nameFile2);

    image2.rotateCounter90();
    image2.save("output_rotate_counter_90.bmp");

    image2.gaussianBlur();
    image2.save("output_gaussian_blur.bmp");

  }
  catch (const std::exception &e) {
      std::cerr << "Ошибка: " << e.what() << std::endl;
  }
    return 0;
}