#ifndef METHODS_H
#define METHODS_H

#include "../bmp_h/bmp.h"
#include <vector>
#include <thread>
#include <string> // Убедитесь, что это включено

class Methods {
public:
    explicit Methods(const std::string &filename);
    ~Methods();

    void Save(const std::string &filename);

    // Многопоточные версии
    void Rotate90();
    void RotateCounter90();
    void GaussianFilter(int kernelSize, const std::vector<std::vector<float>>& kernel);

    // Однопоточные версии (для сравнения производительности)
    void Rotate90SingleThread();
    void RotateCounter90SingleThread();
    void GaussianFilterSingleThread(int kernelSize, const std::vector<std::vector<float>>& kernel);

    // ******************************************************
    // *** ЭТО САМОЕ ВАЖНОЕ ИЗМЕНЕНИЕ: ПУБЛИЧНЫЕ ГЕТТЕРЫ ***
    // ******************************************************
    const BMPHeader& getHeader() const {
        return header;
    }

    const BMPInfoHeader& getInfoHeader() const {
        return infoHeader;
    }

    const std::vector<std::vector<Pixel>>& getData() const {
        return data;
    }

    const std::vector<std::vector<Pixel>>& getOriginalData() const {
        return original_data;
    }

private:
    BMPHeader header;
    BMPInfoHeader infoHeader;
    std::vector<std::vector<Pixel>> data;

    // Исходные данные изображения
    std::vector<std::vector<Pixel>> original_data;


    bool allocateMemory(int height, int width);
    void copyDataToOriginal();


    // Вспомогательные функции для распараллеливания
    void processRotate90Chunk(int startRow, int endRow, const std::vector<std::vector<Pixel>>& originalData, std::vector<std::vector<Pixel>>& rotatedData);
    void processRotateCounter90Chunk(int startRow, int endRow, const std::vector<std::vector<Pixel>>& originalData, std::vector<std::vector<Pixel>>& rotatedData);
    void processGaussianChunk(int startRow, int endRow, int kernelSize, const std::vector<std::vector<float>>& kernel, const std::vector<std::vector<Pixel>>& originalData, std::vector<std::vector<Pixel>>& tempData);
};

#endif // METHODS_H