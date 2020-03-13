#include "Multiplication.h"
#include "GpuMatrix.h"
#include "Parameters.h"
#include <iostream>

int GpuMatrix::N;
int GpuMatrix::rows_N;
int GpuMatrix::cols_N;


void GpuMatrix::set_N(int n) {
    N = n;
    rows_N = (N + TYPE_SIZE - 1) / TYPE_SIZE;
    cols_N = N;
}

void GpuMatrix::set_bit(unsigned int row, unsigned int col, int matrix_number) {
    auto tmp = (N < TYPE_SIZE) ? N : TYPE_SIZE;
    matrix_host[matrix_number * rows_N * cols_N + row * rows_N + col / TYPE_SIZE]
            |= 1U << (tmp - (col % TYPE_SIZE) - 1);
}

unsigned int GpuMatrix::get_bit(unsigned int row, unsigned int col, int matrix_number) {
    auto tmp = (N < TYPE_SIZE) ? N : TYPE_SIZE;
    return (matrix_host[matrix_number * rows_N * cols_N + row * rows_N + col / TYPE_SIZE] &
            (1U << (tmp - (col % TYPE_SIZE) - 1))) ? 1 : 0;
}

void GpuMatrix::sub_mult(TYPE *tasks, int tasks_count) {

    //std::cout << "tasks_count: " << tasks_count << " d: " << tasks[3] << std::endl;
    d_tasks = gpu_lib::device_matrix_alloc(tasks_count * 4 * sizeof(TYPE));
    gpu_lib::cpu_to_gpu_transfer_async(tasks_count * 4 * sizeof(TYPE), tasks, d_tasks);
    
    gpu_lib::parallel_task_product(N, tasks[3], d_tasks, d_rules, matrix_device, tasks_count, rules_count, nonterminals_count);
}

void GpuMatrix::gpu_to_cpu() {
    gpu_lib::gpu_to_cpu_transfer_async(mem_size, matrix_device, matrix_host);
}
