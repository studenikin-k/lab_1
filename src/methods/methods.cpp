#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "../../include/bmp_h/bmp.h"
#include "../../include/methods/methods.h"
#include <algorithm>
#include <thread> // Для std::thread
#include <chrono> // Для измерения времени

// Константа для определения количества потоков.
const unsigned int NUM_THREADS = std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 4;


bool Methods::allocateMemory(int height, int width) {
    try {
        data.resize(height);
        for (int i = 0; i < height; ++i) {
            data[i].resize(width);
        }
        return true;
    } catch (const std::bad_alloc&) {
        std::cerr << "Failed to allocate memory for image data." << std::endl;
        data.clear();
        return false;
    }
}

// Новая вспомогательная функция для копирования data в original_data
void Methods::copyDataToOriginal() {
    original_data = data; // Простая копия
}


Methods::Methods(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error opening file.");
    }

    // Читаем заголовки BMP
    file.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (header.fileType != 0x4D42) {
        throw std::runtime_error("File format is not BMP.");
    }

    file.read(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));

    infoHeader.height = std::abs(infoHeader.height);
    infoHeader.width = std::abs(infoHeader.width);

    if (infoHeader.width == 0 || infoHeader.height == 0) {
        throw std::runtime_error("Unexpected file size (width or height is zero).");
    }


    file.seekg(header.offsetData, file.beg);

    // Выделяем память построчно
    if (!allocateMemory(infoHeader.height, infoHeader.width)) {
        throw std::runtime_error("Failed to allocate memory for image.");
    }

    // Считываем данные построчно
    for (int i = 0; i < infoHeader.height; ++i) {
        file.read(reinterpret_cast<char *>(data[i].data()), infoHeader.width * sizeof(Pixel));
        if (!file) {
            throw std::runtime_error("Error reading file or EOF reached prematurely.");
        }
    }

    file.close();
    copyDataToOriginal(); // Сохраняем исходные данные после загрузки
}

Methods::~Methods() = default;

void Methods::Save(const std::string &filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error saving file.");
    }


    header.fileSize = sizeof(BMPHeader) + sizeof(BMPInfoHeader) + infoHeader.width * infoHeader.height * sizeof(Pixel);
    header.offsetData = sizeof(BMPHeader) + sizeof(BMPInfoHeader);
    infoHeader.sizeImage = infoHeader.width * infoHeader.height * sizeof(Pixel);


    std::cout << "File " << filename << " uses " << infoHeader.sizeImage << " bytes." << std::endl;

    file.write(reinterpret_cast<const char *>(&header), sizeof(header));
    file.write(reinterpret_cast<const char *>(&infoHeader), sizeof(infoHeader));

    for (int i = 0; i < infoHeader.height; ++i) {
        file.write(reinterpret_cast<const char *>(data[i].data()), infoHeader.width * sizeof(Pixel));
    }

    file.close();
}



// Вспомогательная функция для Rotate90, обрабатывающая часть строк
void Methods::processRotate90Chunk(int startRow, int endRow, const std::vector<std::vector<Pixel>>& originalData, std::vector<std::vector<Pixel>>& rotatedData) {
    int originalHeight = originalData.size(); // Используем размер исходных данных
    int originalWidth = originalData[0].size();

    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < originalWidth; ++j) {
            rotatedData[j][originalHeight - i - 1] = originalData[i][j];
        }
    }
}

void Methods::Rotate90() {
    std::vector<std::vector<Pixel>> rotatedData(infoHeader.width, std::vector<Pixel>(infoHeader.height));

    std::vector<std::thread> threads;
    int rowsPerThread = infoHeader.height / NUM_THREADS;
    int remainingRows = infoHeader.height % NUM_THREADS;

    for (unsigned int i = 0; i < NUM_THREADS; ++i) {
        int startRow = i * rowsPerThread;
        int endRow = startRow + rowsPerThread + (i == NUM_THREADS - 1 ? remainingRows : 0);
        threads.emplace_back(&Methods::processRotate90Chunk, this, startRow, endRow, std::cref(data), std::ref(rotatedData));
    }

    for (std::thread& t : threads) {
        t.join();
    }

    data = std::move(rotatedData);
    std::swap(infoHeader.width, infoHeader.height);
}

// Вспомогательная функция для RotateCounter90, обрабатывающая часть строк
void Methods::processRotateCounter90Chunk(int startRow, int endRow, const std::vector<std::vector<Pixel>>& originalData, std::vector<std::vector<Pixel>>& rotatedData) {
    int originalHeight = originalData.size();
    int originalWidth = originalData[0].size();

    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < originalWidth; ++j) {
            rotatedData[originalWidth - j - 1][i] = originalData[i][j];
        }
    }
}

void Methods::RotateCounter90() {
    std::vector<std::vector<Pixel>> rotatedData(infoHeader.width, std::vector<Pixel>(infoHeader.height));

    std::vector<std::thread> threads;
    int rowsPerThread = infoHeader.height / NUM_THREADS;
    int remainingRows = infoHeader.height % NUM_THREADS;

    for (unsigned int i = 0; i < NUM_THREADS; ++i) {
        int startRow = i * rowsPerThread;
        int endRow = startRow + rowsPerThread + (i == NUM_THREADS - 1 ? remainingRows : 0);
        threads.emplace_back(&Methods::processRotateCounter90Chunk, this, startRow, endRow, std::cref(data), std::ref(rotatedData));
    }

    for (std::thread& t : threads) {
        t.join();
    }

    data = std::move(rotatedData);
    std::swap(infoHeader.width, infoHeader.height);
}


//функция для GaussianFilter, обрабатывающая часть строк
void Methods::processGaussianChunk(int startRow, int endRow, int kernelSize, const std::vector<std::vector<float>>& kernel, const std::vector<std::vector<Pixel>>& originalData, std::vector<std::vector<Pixel>>& tempData) {
    int offset = kernelSize / 2; // Смещение от центра ядра
    int currentWidth = originalData[0].size();
    int currentHeight = originalData.size();

    for (int y = startRow; y < endRow; ++y) {
        for (int x = 0; x < currentWidth; ++x) {
            float sumRed = 0.0f, sumGreen = 0.0f, sumBlue = 0.0f;

            // Проход по ядру свертки
            for (int ky = -offset; ky <= offset; ++ky) {
                for (int kx = -offset; kx <= offset; ++kx) {
                    // Координаты пикселей с учетом границ изображения
                    int nx = std::clamp(x + kx, 0, currentWidth - 1);
                    int ny = std::clamp(y + ky, 0, currentHeight - 1);

                    // Координаты ядра
                    int kernelX = kx + offset;
                    int kernelY = ky + offset;

                    // Применение ядра
                    sumRed   += originalData[ny][nx].red * kernel[kernelY][kernelX];
                    sumGreen += originalData[ny][nx].green * kernel[kernelY][kernelX];
                    sumBlue  += originalData[ny][nx].blue * kernel[kernelY][kernelX];
                }
            }

            // Запись результатов с учетом допустимого диапазона значений
            tempData[y][x].red   = static_cast<uint8_t>(std::clamp(sumRed, 0.0f, 255.0f));
            tempData[y][x].green = static_cast<uint8_t>(std::clamp(sumGreen, 0.0f, 255.0f));
            tempData[y][x].blue  = static_cast<uint8_t>(std::clamp(sumBlue, 0.0f, 255.0f));
        }
    }
}

void Methods::GaussianFilter(int kernelSize, const std::vector<std::vector<float>>& kernel) {
    if (kernelSize % 2 == 0 || static_cast<int>(kernel.size()) != kernelSize || static_cast<int>(kernel[0].size()) != kernelSize) {
        throw std::invalid_argument("Kernel size must be odd and match the dimensions of the kernel matrix.");
    }

    std::vector<std::vector<Pixel>> tempData(infoHeader.height, std::vector<Pixel>(infoHeader.width));

    std::vector<std::thread> threads;
    int rowsPerThread = infoHeader.height / NUM_THREADS;
    int remainingRows = infoHeader.height % NUM_THREADS;

    for (unsigned int i = 0; i < NUM_THREADS; ++i) {
        int startRow = i * rowsPerThread;
        int endRow = startRow + rowsPerThread + (i == NUM_THREADS - 1 ? remainingRows : 0);
        threads.emplace_back(&Methods::processGaussianChunk, this, startRow, endRow, kernelSize, std::cref(kernel), std::cref(data), std::ref(tempData));
    }

    for (std::thread& t : threads) {
        t.join();
    }

    data = std::move(tempData);
}



void Methods::Rotate90SingleThread() {

    int original_width = infoHeader.width;
    int original_height = infoHeader.height;


    std::vector<std::vector<Pixel>> rotatedData(original_height, std::vector<Pixel>(original_width));
    // временная копия original_data, чтобы потом получить правильные размеры для rotatedData


    std::vector<std::vector<Pixel>> temp_original_data = original_data; // Копия для избежания модификации original_data


    std::vector<std::vector<Pixel>> finalRotatedData(original_width, std::vector<Pixel>(original_height));

    for (int i = 0; i < original_height; ++i) {
        for (int j = 0; j < original_width; ++j) {
            finalRotatedData[j][original_height - i - 1] = temp_original_data[i][j];
        }
    }

    data = std::move(finalRotatedData);

    infoHeader.width = original_height;
    infoHeader.height = original_width;
}

void Methods::RotateCounter90SingleThread() {
    int original_width = infoHeader.width;
    int original_height = infoHeader.height;

    std::vector<std::vector<Pixel>> temp_original_data = original_data;

    std::vector<std::vector<Pixel>> finalRotatedData(original_width, std::vector<Pixel>(original_height));

    for (int i = 0; i < original_height; ++i) {
        for (int j = 0; j < original_width; ++j) {
            finalRotatedData[original_width - j - 1][i] = temp_original_data[i][j];
        }
    }

    data = std::move(finalRotatedData);
    infoHeader.width = original_height;
    infoHeader.height = original_width;
}

void Methods::GaussianFilterSingleThread(int kernelSize, const std::vector<std::vector<float>>& kernel) {
    if (kernelSize % 2 == 0 || static_cast<int>(kernel.size()) != kernelSize || static_cast<int>(kernel[0].size()) != kernelSize) {
        throw std::invalid_argument("Kernel size must be odd and match the dimensions of the kernel matrix.");
    }


    std::vector<std::vector<Pixel>> tempData(infoHeader.height, std::vector<Pixel>(infoHeader.width));
    int offset = kernelSize / 2;

    for (int y = 0; y < infoHeader.height; ++y) {
        for (int x = 0; x < infoHeader.width; ++x) {
            float sumRed = 0.0f, sumGreen = 0.0f, sumBlue = 0.0f;

            for (int ky = -offset; ky <= offset; ++ky) {
                for (int kx = -offset; kx <= offset; ++kx) {
                    int nx = std::clamp(x + kx, 0, infoHeader.width - 1);
                    int ny = std::clamp(y + ky, 0, infoHeader.height - 1);

                    int kernelX = kx + offset;
                    int kernelY = ky + offset;

                    sumRed   += original_data[ny][nx].red * kernel[kernelY][kernelX];
                    sumGreen += original_data[ny][nx].green * kernel[kernelY][kernelX];
                    sumBlue  += original_data[ny][nx].blue * kernel[kernelY][kernelX];
                }
            }

            tempData[y][x].red   = static_cast<uint8_t>(std::clamp(sumRed, 0.0f, 255.0f));
            tempData[y][x].green = static_cast<uint8_t>(std::clamp(sumGreen, 0.0f, 255.0f));
            tempData[y][x].blue  = static_cast<uint8_t>(std::clamp(sumBlue, 0.0f, 255.0f));
        }
    }

    data = std::move(tempData);
}