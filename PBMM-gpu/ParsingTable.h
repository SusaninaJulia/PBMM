#ifndef PARSINGTABLES_H
#define PARSINGTABLES_H

#include <vector>
#include "GpuMatrix.h"
#include "Graph.h"
#include "Grammar.h"
#include "Bound.h"
#include "Layer.h"

class ParsingTable {
public:
	ParsingTable(const std::string &grammar_filename);
	~ParsingTable();

	unsigned int valiant(Graph grahp);
	unsigned int modification(Graph graph);
	std::vector<std::pair<int, int>> modification_for_substrings(Graph graph, unsigned int s);


private:
    GpuMatrix *matrix;
	Grammar *grammar;
	unsigned int string_length;
    unsigned int rules_length;

	//valiant:
	void complete(unsigned int *bnd);
	void compute(int l, int m);

	//modification:
    TYPE *create_task(Layer layer, unsigned int layer_len, int task_num);
	void completeLayer(Layer subM);
	void completeVLayer(Layer subVM);	
    void constructLayer(int i);
};

#endif //PARSINGTABLES_H
