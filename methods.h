#ifndef METHODS_H
#define METHODS_H
#include <string>

class BMP{
public:
  BMP (const std::string &filename);
  ~BMP();
  void save(const std::string &filename);
  void rotate90();
  void rotateCounter90();
  void gaussianBlur();

private:
  BMPHeader header;
  BMPInfoHeader infoHeader;
  Pixel** data;
  bool Memory(int height, int width);
  void FreeMemory(int height);
};


#endif //METHODS_H
