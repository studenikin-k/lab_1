#include "bmp.h"
#include "methods.h"
#include <iostream>
#include <fstream>
#include <cstdint>


class BMP {
public:
    BMP (const std::string &filename);
    ~BMP();

private:
    BMPHeader header;
    BMPInfoHeader infoHeader;
    Pixel** data;
    bool Memory(int height, int width);
    void FreeMemory(int height);
  };

bool BMP::Memory(int height,int width) {
    data = new (std::nothrow) Pixel*[height];
    if (!data) return false;

    for (int i = 0; i < height; ++i) {
        data[i] = new (std::nothrow) Pixel[width];
        if (!data[i]) {
            for (int j = 0; j < i; ++j) {
                delete[] data[j];
            }
            delete[] data;
            data = nullptr;
            return false;
        }
    }
    return true;
}

void BMP::FreeMemory(int height) {
    if (data) {
        for (int i = 0; i < height; ++i) {
            delete[] data[i];
        }
        delete[] data;
        data = nullptr;
    }
}

BMP::BMP(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Open file error.");
    }

    file.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (header.fileType != 0x4D42) {
        throw std::runtime_error("File have not a BMP file.");
    }

    file.read(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));

    infoHeader.height = std::abs(infoHeader.height);
    infoHeader.width = std::abs(infoHeader.width);

    if (infoHeader.width == 0 || infoHeader.height == 0) {
        throw std::runtime_error("Invalid file size.");
    }
    file.seekg(header.offsetData, file.beg);

    Memory(infoHeader.height, infoHeader.width);

    for (int i = 0; i < infoHeader.height; ++i) {
        for (int j = 0; j < infoHeader.width; ++j) {
            file.read(reinterpret_cast<char *>(&data[i][j]), sizeof(Pixel));
            if (!file) {
                throw std::runtime_error("Error reading BMP file.");
            }
        }
    }

    file.close();
}