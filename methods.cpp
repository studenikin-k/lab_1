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
    data = new (std::nothrow) Pixel*[height]; // Выделяем массив указателей на строки
    if (!data) return false; // Проверка на успешное выделение памяти

    for (int i = 0; i < height; ++i) {
        data[i] = new (std::nothrow) Pixel[width];
        if (!data[i]) { // Если выделение памяти не удалось
            // Освобождаем уже выделенную память
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