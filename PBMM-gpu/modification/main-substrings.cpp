#include <iostream>
#include <sstream> 
#include <chrono>
#include "ParsingTable.h"

int main(int argc, char *argv[]) {

    ParsingTable table = ParsingTable(argv[1]);
    Graph graph = Graph(argv[2]);
    int s = 0;
    std::stringstream sublen(argv[3]);
    sublen >> s;

    auto begin = std::chrono::high_resolution_clock::now();
    std::cout << "modification:" << std::endl;
    
    std::vector<std::pair<int, int>> res = table.modification_for_substrings(graph, s);
    std::cout << res.size() << std::endl;
//    for(int i = 0; i < res.size(); i++)
//    {
//         std::cout << res[i].first << ", " << res[i].second << std::endl;
//    }

    auto end = std::chrono::high_resolution_clock::now();
    auto dur = end - begin;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    std::cout << "time: " << ms << std::endl;

    return 0;
}
