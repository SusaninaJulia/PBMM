#include <stdio.h>
#include <iostream>
#include "Multiplication.h"
#include "Parameters.h"

#define cuda_handle_error(ans) { gpuAssert((ans), __FILE__, __LINE__); }

using namespace gpu_lib;

inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true) {
    if (code != cudaSuccess) {
        std::cout << "GPU assert: " << cudaGetErrorString(code) << " " << file << " " << line << std::endl;
        if (abort) {
            exit(code);
        }
    }
}

int gpu_lib::rows(int N) {
	return (N + TYPE_SIZE - 1) / TYPE_SIZE;
}

int gpu_lib::cols(int N) {
	return N;
}

//inline size_t matrix_memsize(int N) {
//	return rows(N) * cols(N) * sizeof(TYPE);
//}

__device__ void or_value(TYPE *M, TYPE val) {
	*M |= val;
}

void gpu_lib::synchronize() {
	cudaDeviceSynchronize();
}

void set_value(int mem_size, TYPE *d_M, int val) {
	cuda_handle_error(cudaMemset(d_M, val, mem_size));
}

TYPE * gpu_lib::device_matrix_alloc(int mem_size) {
	TYPE *d_M;
	cuda_handle_error(cudaMalloc(reinterpret_cast<void **>(&d_M), mem_size));
	return d_M;
}

void gpu_lib::device_matrix_dealloc(TYPE *M) {
	cuda_handle_error(cudaFree(M));
}

TYPE * gpu_lib::host_matrix_calloc(int mem_size) {
    TYPE *M;
    cuda_handle_error(cudaMallocHost(reinterpret_cast<void **>(&M), mem_size));
    set_value(mem_size, M, 0);
	return M;
}

void gpu_lib::host_matrix_dealloc(TYPE *M) {
	cuda_handle_error(cudaFreeHost(M));
}

void gpu_lib::gpu_to_cpu_transfer_async(int mem_size, TYPE *d_M, TYPE *h_M) {
	cuda_handle_error(cudaMemcpy(h_M, d_M, mem_size, cudaMemcpyDeviceToHost));
}

void gpu_lib::cpu_to_gpu_transfer_async(int mem_size, TYPE *h_M, TYPE *d_M) {
	cuda_handle_error(cudaMemcpy(d_M, h_M, mem_size, cudaMemcpyHostToDevice));
}

__device__ void sub_matrix_parallel_product(TYPE *matrices, int nA, int nB, int nC, int N, int l1, int l2, int l3, int d) {
    int ROWS = (N + TYPE_SIZE - 1) / TYPE_SIZE;
    int start = blockIdx.y * 32 * ROWS + blockIdx.z;
    int th = threadIdx.y;
	TYPE acc = 0;
    //#pragma unroll
	for (TYPE i = d; i > 0; --i) {

        //printf("a_el = %u \n", matrices[nA + start + (l1 + th) * ROWS + l2 / TYPE_SIZE]);
        //printf("ashift = %u \n", (TYPE_SIZE - (l2 % TYPE_SIZE + i)));
        //printf("b_el = %u \n", matrices[nB + start + l2 * ROWS + l3 / TYPE_SIZE + (i - 1) * ROWS]);
        //printf("bshift = %u \n", (TYPE_SIZE + th - (l3 % TYPE_SIZE + d)));
        //printf("a_el + shift = %u \n", matrices[nA + start + (l1 + th) * ROWS + l2 / TYPE_SIZE] >> (TYPE_SIZE -  (l2 % TYPE_SIZE + i)));
        //printf("b_el + shift = %u \n", matrices[nB + start + l2 * ROWS + l3 / TYPE_SIZE + (i - 1) * ROWS] >> (TYPE_SIZE + th - (l3 % TYPE_SIZE + d)));
        
        
        TYPE a_el = matrices[nA + start + (l1 + th) * ROWS + l2 / TYPE_SIZE] >> (TYPE_SIZE - (l2 % TYPE_SIZE + i));
		TYPE b_el = matrices[nB + start + (l2 + i - 1) * ROWS + l3 / TYPE_SIZE] >> (TYPE_SIZE + th - (l3 % TYPE_SIZE + d));
		if (a_el & b_el & 1)
			acc |= (1U << (TYPE_SIZE +  th - (l3 % TYPE_SIZE + i)));
	}
    
    //printf("c_el = %u \n", nC + start + (l1 + th) * ROWS + l3 / TYPE_SIZE);
    //printf("acc = %u \n", acc);
    if (acc == 0) return;
    atomicOr(&matrices[nC + start + (l1 + th) * ROWS + l3 / TYPE_SIZE], acc);
}



__global__ void parallel_product(int N, unsigned int *tasks, unsigned int *rules, TYPE *matrices) {
    
	int shift = (N + TYPE_SIZE - 1) / TYPE_SIZE * N;
	int task_num = blockIdx.x;
	int rule_num = threadIdx.x;

	int l1 = tasks[task_num * 4];
	int l2 = tasks[task_num * 4 + 1];
	int l3 = tasks[task_num * 4 + 2];
    //printf("blockid = %u, l1 = %u, l2 = %u, l3 = %u threadid = %u, A = %u, B = %u, C = %u \n", task_num, l1, l2, l3, rule_num, rules[rule_num * 3 + 1], rules[rule_num * 3 + 2] , rules[rule_num * 3]);


    int nC = rules[rule_num * 3] * shift;
	int nA = rules[rule_num * 3 + 1] * shift;
	int nB = rules[rule_num * 3 + 2] * shift;

    int d = (tasks[3] < THREADS_PER_BLOCK) ? tasks[3] : THREADS_PER_BLOCK;

	sub_matrix_parallel_product(matrices, nA, nB, nC, N, l1, l2, l3, d);
}

void gpu_lib::parallel_task_product(int N, int d, TYPE *tasks, TYPE *rules, TYPE *matrices, int tasks_len, int rules_len, int matrices_len) {
					// tasks_len и rules_len не равны длине массивов tasks и rules соответственно;
					// длина tasks = tasks_len / 4; длина rules = rules_len / 3;
    
	int matrices_size = d;

    //std::cout << "tasks_len = " << tasks_len << " rules_len = " << rules_len << " matrices_len = " << matrices_len << " matrices_size = " << matrices_size << std::endl;

	int blocks_num = (matrices_size < THREADS_PER_BLOCK) ? 1 : ((matrices_size + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK);
	int threads_num = (matrices_size < THREADS_PER_BLOCK) ? matrices_size : THREADS_PER_BLOCK;

	dim3 blocks(tasks_len, blocks_num, blocks_num);
	dim3 threads(rules_len, threads_num);

	parallel_product<<<blocks, threads>>>(N, tasks, rules, matrices);
    gpu_lib::synchronize();
}

