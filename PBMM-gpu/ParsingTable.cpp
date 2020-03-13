#include "ParsingTable.h"
#include "Grammar.h"
#include "Graph.h"
#include "Bound.h"
#include "Layer.h"

#include <cmath> 
#include <iostream> 


ParsingTable::ParsingTable(const std::string &grammar_filename) {
	this->grammar = new Grammar(grammar_filename);
}

ParsingTable::~ParsingTable() {}

unsigned int ParsingTable::valiant(Graph graph){

    this->string_length = graph.vertices_count;
    this->rules_length = (*(this->grammar)).rules_count;
    this->matrix = new GpuMatrix(*(this->grammar), graph);

    this->compute(0, string_length);

    (*(this->matrix)).gpu_to_cpu();
    unsigned int res = (*(this->matrix)).get_bit(0, this->string_length - 1, 0);

    delete matrix;

    return res;
}

unsigned int ParsingTable::modification(Graph graph) {


    this->string_length = graph.vertices_count;
    this->rules_length = (*(this->grammar)).rules_count;
    this->matrix = new GpuMatrix(*(this->grammar), graph);

    for (int i = 1; i < log2(string_length); i++)
        this->constructLayer(i);

    (*(this->matrix)).gpu_to_cpu();
    unsigned int res = (*(this->matrix)).get_bit(0, this->string_length - 1, 0);

    delete matrix;

    return res;
}

std::vector<std::pair<int, int>> ParsingTable::modification_for_substrings(Graph graph, unsigned int s) {


    this->string_length = graph.vertices_count;
    this->rules_length = (*(this->grammar)).rules_count;
    this->matrix = new GpuMatrix(*(this->grammar), graph);

    double ln = ceil(log2(s));
    for(int i = 1; i <= ln; i++)
        this->constructLayer(i);
    
    std::vector<std::pair<int, int>> correctStrings;        
    for(int j = 0; j <= string_length - s; j++) {
        correctStrings.emplace_back(j, s + j);
    }

    delete matrix;

    return correctStrings;
}


//valiant:
void ParsingTable::complete(unsigned int *bnd) {
     
	if (bnd[1] - bnd[0] > 1) {  

        //std::cout << " bnd: " << bnd[0] << bnd[1] << bnd[2] << bnd[3] << std::endl;

        unsigned int av1 = (bnd[1] + bnd[0]) / 2;
        unsigned int av2 = (bnd[3] + bnd[2]) / 2;

        unsigned int c[] = {av1, bnd[1], bnd[2], av2};
        unsigned int d1[] = {bnd[0], av1, bnd[2], av2};
        unsigned int d2[] = {av1, bnd[1], av2, bnd[3]};
        unsigned int e[] = {bnd[0], av1, av2,  bnd[3]};

        unsigned int len = (bnd[1] - bnd[0]) / 2;

        this->complete(c);

        unsigned int tasks[] = {bnd[0], av1, bnd[2], len, av1, bnd[2], av2, len}; 

        matrix->sub_mult(tasks, 2);
        this->complete(d1);
        this->complete(d2);
        
        tasks[2] = av2; tasks[4] = bnd[0]; tasks[5] = bnd[2]; tasks[6] = av2;
        matrix->sub_mult(tasks, 2);
        this->complete(e);
    }
}
void ParsingTable::compute(int l, int m) {
    //std::cout << " l: " << l << " m: " << m << std::endl;
	int av = (l + m)/ 2;
    if (m - l  >= 4) {
        this->compute(l, av);
        this->compute(av, m);
    }
    unsigned int bnd[] = {l, av, av, m};
    this->complete(bnd);
}

//modification:

TYPE *ParsingTable::create_task(Layer layer, unsigned int layer_len, int task_num) {  

    auto layerit = layer.layerBegin;
  
    TYPE *tasks = new unsigned int[2 * layer_len * 4]();  
    for (int i = 0; i < layer_len; i++) {
        tasks[i * 8 + 3] = layer.getMtrxSize() / 2;
        tasks[i * 8 + 7] = layer.getMtrxSize() / 2;
        switch (task_num) {
            case 1:
                tasks[i * 8] = layerit[i].l1;
                tasks[i * 8 + 1] = layerit[i].av1;
                tasks[i * 8 + 2] = layerit[i].l2;

                tasks[i * 8 + 4] = layerit[i].av1;
                tasks[i * 8 + 5] = layerit[i].l2;
                tasks[i * 8 + 6] = layerit[i].av2;
                break;
            case 2:
                tasks[i * 8] = layerit[i].l1;
                tasks[i * 8 + 1] = layerit[i].av1;
                tasks[i * 8 + 2] = layerit[i].av2;

                tasks[i * 8 + 4] = layerit[i].l1;
                tasks[i * 8 + 5] = layerit[i].l2;
                tasks[i * 8 + 6] = layerit[i].av2;         
                break;
            }
    }  
    return tasks;   
}
void ParsingTable::completeLayer(Layer subM) {
   
    if (subM.getMtrxSize() > 1) {

        int tmpsz = static_cast<int>(subM.size);
        std::vector<Bound> layer(4*tmpsz);
        auto layerit = layer.begin();
        subM.getLayer(layerit);
        Layer bottom = Layer(&layer[0] + 3*tmpsz, tmpsz);
        this->completeLayer(bottom);

        auto mtxsz = subM.getMtrxSize() / 2;

        int i;
        Layer leftright = Layer(&layer[0], 2*tmpsz);
        matrix->sub_mult(create_task(subM, tmpsz, 1), 2 * tmpsz);
        this->completeLayer(leftright);

        Layer top = Layer(&layer[0] + 2*tmpsz, tmpsz);
        matrix->sub_mult(create_task(subM, tmpsz, 2), 2 * tmpsz);
        this->completeLayer(top);
    }
}
void ParsingTable::completeVLayer(Layer subVM) {

    int tmpsz = static_cast<int>(subVM.size);
    std::vector<Bound> layer(4*tmpsz);
    auto layerit = layer.begin();
    subVM.getLayer(layerit);

    int i;
    Layer leftright = Layer(&layer[0], 2*tmpsz);
    matrix->sub_mult(create_task(subVM, tmpsz, 1), 2 * tmpsz);
    this->completeLayer(leftright);

    Layer top = Layer(&layer[0] + 2*tmpsz, tmpsz);
    matrix->sub_mult(create_task(subVM, tmpsz, 2), 2 * tmpsz);
    this->completeLayer(top);
}	

void ParsingTable::constructLayer(int i) {
    std::vector<Bound> layer;
    int pow2i = pow(2, i);
    for(int k = 0; k < ((string_length / pow2i) - 1); k++) {
        Bound b = Bound(k*pow2i, (k + 1)*pow2i, (k + 1)*pow2i, (k + 2)*pow2i);
        layer.push_back(b);
    }
    this->completeVLayer(Layer(&layer[0], layer.size()));
}
