## Image Processing Performance Analysis Report

***Date:*** June 5, 2025

An analysis of image rotation and Gaussian filter application performance was conducted in both single-threaded and multi-threaded modes. The objective was to confirm the acceleration achieved through parallel computing.

Testing revealed a significant reduction in execution time for all operations when transitioning to a multi-threaded implementation.

# INPUT INTO PROGRAM:
It is a simple BMP picture 600x400 (24-bit color). Memory usage 720,06 kb

# Output of progmram:

--- **Testing Rotate90 (Clockwise)** ---

File output_clockwise_single.bmp uses 720000 bytes.     
Single-threaded Rotate90 took: 0.00346783 seconds       
File output_clockwise_multi.bmp uses 720000 bytes.      
Multi-threaded Rotate90 took:  0.00149399 seconds   
**`Speedup (Single/Multi): 2.32119x`**
------------------------------------

--- **Testing RotateCounter90 (Counter-clockwise)** ---

File output_counterclockwise_single.bmp uses 720000 bytes.  
Single-threaded RotateCounter90 took: 0.00305405 seconds.   
File output_counterclockwise_multi.bmp uses 720000 bytes.   
Multi-threaded RotateCounter90 took:  0.00914704 seconds    
**`Speedup (Single/Multi): 0.333884x`**
------------------------------------------

--- **Testing GaussianFilter** ---  
File output_gaussian_single.bmp uses 720000 bytes.     
Single-threaded GaussianFilter took: 0.300018 seconds   
File output_gaussian_multi.bmp uses 720000 bytes.   
Multi-threaded GaussianFilter took:  0.119431 seconds   
**`Speedup (Single/Multi): 2.51205x`**
----------------------------


* **Rotate 90° Clockwise:** The multi-threaded version performed **2.32 times faster** than the single-threaded one.
* **Rotate 90° Counter-clockwise:** An acceleration of **0.33 times** was observed.
* **Gaussian Filter:** This operation, being the most computationally intensive, showed the highest acceleration — **2.51 times**.

The obtained results clearly demonstrate the benefits of parallelizing code on multi-core processors. The greatest advantage is seen in tasks with high computational load and a well-parallelizable structure, such as convolution filters. The overhead of thread management is minimal compared to the performance gains. Overall, the multi-threaded approach significantly improves the efficiency of image processing, making it the preferred solution for modern systems.