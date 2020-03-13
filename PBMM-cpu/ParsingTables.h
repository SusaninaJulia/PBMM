#ifndef PARSING_PARSINGTABLES_H
#define PARSING_PARSINGTABLES_H

#include "Grammar.h"
#include "Graph.h"
#include "Bound.h"


class ParsingTables {
public:
    int n;
    
    unsigned int nonterminals_count;
    unsigned int rules_count;
    unsigned int *rules;
   
    std::map<unsigned int, mzd_t*> Ts;

    ParsingTables(Grammar gr, Graph graph);
    ~ParsingTables();
    void subMatricesMult(Bound bnd1, Bound bnd2, Bound bnd3, mzd_t *A,mzd_t *B, mzd_t *C);
    virtual bool parseString() = 0;
};

#endif //PARSING_PARSINGTABLES_H
