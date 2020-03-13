#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <map>
#include <vector>
#include <unordered_map>
#include <string>

using nonterminals_pair = std::pair<unsigned int, unsigned int>;

class Grammar {

public:
    Grammar(const std::string &grammar_filename);
    ~Grammar();

    std::map<std::string, unsigned int> nonterminal_to_index;
    std::unordered_map<std::string, std::vector<int>> terminal_to_nonterminals;
    std::vector<std::pair<unsigned int, nonterminals_pair>> map_rules;

    unsigned int *rules;

    unsigned int nonterminals_count = 0;
    unsigned int rules_count = 0;
};


#endif //GRAMMAR_H
