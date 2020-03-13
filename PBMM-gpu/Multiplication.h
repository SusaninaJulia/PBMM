
#ifndef CFPQ_CUDA_MULTIPLICATION_H
#define CFPQ_CUDA_MULTIPLICATION_H

#include "Parameters.h"

namespace gpu_lib {

    int rows(int N);

    int cols(int N);

    void synchronize();

    TYPE *device_matrix_alloc(int N);

    void device_matrix_dealloc(TYPE *M);

    TYPE *host_matrix_calloc(int N);

    void host_matrix_dealloc(TYPE *M);

    void gpu_to_cpu_transfer_async(int N, TYPE *d_M, TYPE *h_M);

    void cpu_to_gpu_transfer_async(int N, TYPE *h_M, TYPE *d_M);
    
    void parallel_task_product(int N, int d, TYPE *tasks, TYPE *rules, TYPE *matrices, int tasks_len, int rules_len, int matrices_len);

}

#endif //CFPQ_CUDA_MULTIPLICATION_H
