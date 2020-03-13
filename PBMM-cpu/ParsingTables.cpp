#include <m4ri/m4ri_config.h>
#include <m4ri/m4ri.h>
#include <m4ri/semiring/mzd_sr.h>
#include <m4ri/semiring/russian.h>
#include <iostream>

#include "ParsingTables.h"

ParsingTables::ParsingTables(Grammar grammar, Graph graph) {

    nonterminals_count = grammar.nonterminals_count;
    rules_count = grammar.rules_count;
    rules = grammar.rules;
    n = graph.vertices_count;

    std::cout << "n = " << n << std::endl;

    for (unsigned int nt = 0 ; nt < nonterminals_count; nt++) {
        auto M = mzd_init(n, n);
        Ts.insert(std::make_pair(nt, M));
    }

    for (auto &edge : graph.edges) {
        for (unsigned int nonterm : grammar.terminal_to_nonterminals[edge.first]) {
            mzd_write_bit(Ts[nonterm], edge.second.first, edge.second.second, 1);
        }
    }
}
ParsingTables::~ParsingTables() {
    for (unsigned int nt = 0 ; nt < nonterminals_count; nt++)  {
        mzd_free(Ts[nt]);
    }
}

void ParsingTables::subMatricesMult(Bound bnd1, Bound bnd2, Bound bnd3, mzd_t *A,mzd_t *B, mzd_t *C) {

    int len = bnd1.m1 - bnd1.l1;

    for (int rc = 0; rc < rules_count; rc++) {

        mzd_submatrix(A, Ts[rules[rc * 3 + 1]], bnd1.l1, bnd1.l2, bnd1.m1, bnd1.m2);
        mzd_submatrix(B, Ts[rules[rc * 3 + 2]], bnd2.l1, bnd2.l2, bnd2.m1, bnd2.m2);
        mzd_sr_mul_m4rm(C, A, B, 0);

        int wordsNumber = (len - 1) / 64 + 1;
        int symbolsNumber = (len < 65) ? len : 64;
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < wordsNumber; j++) {
                mzd_or_bits(Ts[rules[rc * 3]], bnd3.l1 + i, bnd3.l2 + 64 * j, symbolsNumber, *(C->rows[i] + j));
            }
        }
    }
}
