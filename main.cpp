#include "bmp.h"
#include <iostream>
#include "methods.h"

int main(){
  try {
    std::cout << "Hello, please enter name of file: ";
    std::string nameFile;
    std::getline (std::cin,nameFile);
    std::cout << std::endl;

    BMP image(nameFile);

    image.rotate90();
    image.save("output_rotate_90.bmp");
  }
  catch (const std::exception &e) {
      std::cerr << "Ошибка: " << e.what() << std::endl;
  }
    return 0;
}