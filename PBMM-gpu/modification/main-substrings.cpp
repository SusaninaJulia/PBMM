
#include <iostream>
#include <chrono>
#include "ParsingTable.h"

int main(int argc, char *argv[]) {

    ParsingTable table = ParsingTable(argv[1]);
    Graph graph = Graph(argv[2]);

    auto begin = std::chrono::high_resolution_clock::now();
    std::cout << "modification:" << std::endl;
    
    auto times = table.modification(graph);
    std::cout << times << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    auto dur = end - begin;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    std::cout << "time: " << ms << std::endl;

    return 0;
}
