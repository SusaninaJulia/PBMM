# PBMM
# PARSING BY MATRIX MULTIPLICATION


Implementations of Valiant's algorithm and its modification:

- CPU-based solution (valiantCPU, modificationCPU, modificationCPU-substrings): 
One of the most efficient implementations of the Method of the Four Russians from the library
M4RI is used for Boolean matrix multiplication.

- GPU-based solution (valiantGPU, modificationGPU, modificationGPU-substrings):
A naive Boolean matrix multiplication in CUDA C with Boolean values treated as bits and packed
into uint 32 is implemented.

Start:

* Install m4ri library for CPU-based solutions and CUDA for GPU-based solutions.
* Run the usual commands:
  - cmake CMakeLists.txt -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc
  - make
