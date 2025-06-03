### Image Processing Performance Analysis Report

**Date:** June 3, 2025

An analysis of image rotation and Gaussian filter application performance was conducted in both single-threaded and multi-threaded modes. The objective was to confirm the acceleration achieved through parallel computing.

Testing revealed a significant reduction in execution time for all operations when transitioning to a multi-threaded implementation.

* **Rotate 90° Clockwise:** The multi-threaded version performed **2.57 times faster** than the single-threaded one.
* **Rotate 90° Counter-clockwise:** An acceleration of **1.73 times** was observed.
* **Gaussian Filter:** This operation, being the most computationally intensive, showed the highest acceleration — **3.25 times**.

The obtained results clearly demonstrate the benefits of parallelizing code on multi-core processors. The greatest advantage is seen in tasks with high computational load and a well-parallelizable structure, such as convolution filters. The overhead of thread management is minimal compared to the performance gains. Overall, the multi-threaded approach significantly improves the efficiency of image processing, making it the preferred solution for modern systems.