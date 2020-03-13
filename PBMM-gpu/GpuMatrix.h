#ifndef GPU_MATRIX_H
#define GPU_MATRIX_H

#include "Multiplication.h"
#include "Parameters.h"
#include "Graph.h"
#include "Grammar.h"

#include <iostream>

class GpuMatrix {
    
public:

    TYPE *matrix_host;
    TYPE *matrix_device;

    TYPE *d_rules;
    TYPE *d_tasks;

    static int N;
    static int rows_N;
    static int cols_N;

    int mem_size;
    int nonterminals_count;
    int rules_count;

    GpuMatrix(Grammar grammar, Graph graph) {
        
        GpuMatrix::set_N(graph.vertices_count);

        nonterminals_count = grammar.nonterminals_count;
        rules_count = grammar.rules_count;      

        mem_size = rows_N * cols_N * nonterminals_count * sizeof(TYPE);
        matrix_host = gpu_lib::host_matrix_calloc(mem_size);

        for (auto &edge : graph.edges) {
            for (unsigned int nonterm : grammar.terminal_to_nonterminals[edge.first]) {
                set_bit(edge.second.first, edge.second.second, nonterm);
            }
        }

        d_rules = gpu_lib::device_matrix_alloc(3 * rules_count * sizeof(TYPE));
        gpu_lib::cpu_to_gpu_transfer_async(3 * rules_count * sizeof(TYPE), grammar.rules, d_rules);
        
        matrix_device = gpu_lib::device_matrix_alloc(mem_size);
        gpu_lib::cpu_to_gpu_transfer_async(mem_size, matrix_host, matrix_device);
        gpu_lib::synchronize();   

    };

    ~GpuMatrix() {    
        /*
        for (int k = 0; k < nonterminals_count; k++){
            for (int i = 0; i < cols_N; i++) {
                for (int j = 0; j < rows_N; j++){
                    int o = 0;
                    if (matrix_host[k * rows_N * cols_N + i * rows_N + j] != 0) {o = 1;} 
                    std::cout << matrix_host[k * rows_N * cols_N + i * rows_N + j] << " ";
               }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        */
        gpu_lib::device_matrix_dealloc(matrix_device);
        gpu_lib::host_matrix_dealloc(matrix_host);
        gpu_lib::device_matrix_dealloc(d_rules);
    };

    static void set_N(int n);

    void set_bit(unsigned int row, unsigned int col, int matrix_number);

    unsigned int get_bit(unsigned int row, unsigned int col, int matrix_number);

    void sub_mult(TYPE *tasks, int task_count);

    void gpu_to_cpu();
};


#endif //GPU_MATRIX_H
