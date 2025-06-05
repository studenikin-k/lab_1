#include <iostream>
#include <string>
#include <chrono> // Для измерения времени
#include "methods/methods.h"

int main() {
    std::cout << "Hello, please enter name of file: ";
    std::string input;
    std::getline(std::cin, input);
    std::cout << std::endl;

    try {
        int kernelSize = 5;
        std::vector<std::vector<float>> kernel = {
            {1 / 256.0f, 4 / 256.0f, 6 / 256.0f, 4 / 256.0f, 1 / 256.0f},
            {4 / 256.0f, 16 / 256.0f, 24 / 256.0f, 16 / 256.0f, 4 / 256.0f},
            {6 / 256.0f, 24 / 256.0f, 36 / 256.0f, 24 / 256.0f, 6 / 256.0f},
            {4 / 256.0f, 16 / 256.0f, 24 / 256.0f, 16 / 256.0f, 4 / 256.0f},
            {1 / 256.0f, 4 / 256.0f, 6 / 256.0f, 4 / 256.0f, 1 / 256.0f}
        };

        std::cout << "--- Testing Rotate90 (Clockwise) ---" << std::endl;

        // Однопоточная версия
        Methods image_rotate90_single(input);
        auto start_single_rotate90 = std::chrono::high_resolution_clock::now();
        image_rotate90_single.Rotate90SingleThread();
        auto end_single_rotate90 = std::chrono::high_resolution_clock::now();
        image_rotate90_single.Save("output_clockwise_single.bmp");
        std::chrono::duration<double> duration_single_rotate90 = end_single_rotate90 - start_single_rotate90;
        std::cout << "Single-threaded Rotate90 took: " << duration_single_rotate90.count() << " seconds" << std::endl;

        // Многопоточная версия
        Methods image_rotate90_multi(input);
        auto start_multi_rotate90 = std::chrono::high_resolution_clock::now();
        image_rotate90_multi.Rotate90();
        auto end_multi_rotate90 = std::chrono::high_resolution_clock::now();
        image_rotate90_multi.Save("output_clockwise_multi.bmp");
        std::chrono::duration<double> duration_multi_rotate90 = end_multi_rotate90 - start_multi_rotate90;
        std::cout << "Multi-threaded Rotate90 took:  " << duration_multi_rotate90.count() << " seconds" << std::endl;
        std::cout << "Speedup (Single/Multi): " << duration_single_rotate90.count() / duration_multi_rotate90.count() << "x" << std::endl;
        std::cout << "------------------------------------" << std::endl << std::endl;


        std::cout << "--- Testing RotateCounter90 (Counter-clockwise) ---" << std::endl;

        // Однопоточная версия
        Methods image_rotate_counter90_single(input);
        auto start_single_rotate_counter90 = std::chrono::high_resolution_clock::now();
        image_rotate_counter90_single.RotateCounter90SingleThread();
        auto end_single_rotate_counter90 = std::chrono::high_resolution_clock::now();
        image_rotate_counter90_single.Save("output_counterclockwise_single.bmp");
        std::chrono::duration<double> duration_single_rotate_counter90 = end_single_rotate_counter90 - start_single_rotate_counter90;
        std::cout << "Single-threaded RotateCounter90 took: " << duration_single_rotate_counter90.count() << " seconds" << std::endl;

        // Многопоточная версия
        Methods image_rotate_counter90_multi(input);
        auto start_multi_rotate_counter90 = std::chrono::high_resolution_clock::now();
        image_rotate_counter90_multi.RotateCounter90();
        auto end_multi_rotate_counter90 = std::chrono::high_resolution_clock::now();
        image_rotate_counter90_multi.Save("output_counterclockwise_multi.bmp");
        std::chrono::duration<double> duration_multi_rotate_counter90 = end_multi_rotate_counter90 - start_multi_rotate_counter90;
        std::cout << "Multi-threaded RotateCounter90 took:  " << duration_multi_rotate_counter90.count() << " seconds" << std::endl;
        std::cout << "Speedup (Single/Multi): " << duration_single_rotate_counter90.count() / duration_multi_rotate_counter90.count() << "x" << std::endl;
        std::cout << "------------------------------------------" << std::endl << std::endl;



        std::cout << "--- Testing GaussianFilter ---" << std::endl;

        // Однопоточная версия
        Methods image_gaussian_single(input);
        auto start_single_gaussian = std::chrono::high_resolution_clock::now();
        image_gaussian_single.GaussianFilterSingleThread(kernelSize, kernel);
        auto end_single_gaussian = std::chrono::high_resolution_clock::now();
        image_gaussian_single.Save("output_gaussian_single.bmp");
        std::chrono::duration<double> duration_single_gaussian = end_single_gaussian - start_single_gaussian;
        std::cout << "Single-threaded GaussianFilter took: " << duration_single_gaussian.count() << " seconds" << std::endl;

        // Многопоточная версия
        Methods image_gaussian_multi(input);
        auto start_multi_gaussian = std::chrono::high_resolution_clock::now();
        image_gaussian_multi.GaussianFilter(kernelSize, kernel);
        auto end_multi_gaussian = std::chrono::high_resolution_clock::now();
        image_gaussian_multi.Save("output_gaussian_multi.bmp");
        std::chrono::duration<double> duration_multi_gaussian = end_multi_gaussian - start_multi_gaussian;
        std::cout << "Multi-threaded GaussianFilter took:  " << duration_multi_gaussian.count() << " seconds" << std::endl;
        std::cout << "Speedup (Single/Multi): " << duration_single_gaussian.count() / duration_multi_gaussian.count() << "x" << std::endl;
        std::cout << "----------------------------" << std::endl << std::endl;


    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}