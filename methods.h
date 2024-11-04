#ifndef METHODS_H
#define METHODS_H
#include "bmp.h"
class BMP {
public:
    BMP(const std::string &filename);
    ~BMP();
    void Save(const std::string &filename);
    void Rotate90();
    void RotateCounter90();
    void GaussianFilter();

private:
    BMPHeader header;
    BMPInfoHeader infoHeader;
    Pixel** data;
    bool Memory(int height, int width);
    void FreeMemory(int height);
};




#endif //METHODS_H
