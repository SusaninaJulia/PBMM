#include <utility>

#include <iostream>
#include <fstream>
#include <chrono>
#include <m4ri/m4ri_config.h>
#include <m4ri/m4ri.h>
#include <m4ri/semiring/mzd_sr.h>
#include <m4ri/semiring/russian.h>

#include "Grammar.h"
#include "Bound.h"
#include "Layer.h"
#include "ParsingTables.h"

#include "ValiantParsing.h"

using namespace std;

int main(int argc, char *argv[]) {

    Grammar grammar = Grammar(argv[1]);
    Graph graph = Graph(argv[2]);

    ValiantParsing val = ValiantParsing(grammar, graph);

    auto begin = std::chrono::high_resolution_clock::now();
    std::cout << "valiant:" << std::endl;
    
    bool res = val.parseString();
    std::cout << res << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    auto dur = end - begin;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    std::cout << "time: " << ms << std::endl;

    return 0;
}
