#include "Grammar.h"
#include <sstream>
#include <fstream>
#include <iostream>

using std::istringstream;
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;


Grammar::Grammar(const string &grammar_filename) {

    auto chomsky_stream = ifstream(grammar_filename, ifstream::in);

    string line, tmp;

    while (getline(chomsky_stream, line)) {
        vector<string> terms;
        istringstream iss(line);
        while (iss >> tmp) {
            terms.push_back(tmp);
        }
        if (!nonterminal_to_index.count(terms[0])) {
            nonterminal_to_index[terms[0]] = nonterminals_count++;
        }
        if (terms.size() == 2) {
            if (!terminal_to_nonterminals.count(terms[1])) {
                terminal_to_nonterminals[terms[1]] = {};
            }
            terminal_to_nonterminals[terms[1]].push_back(nonterminal_to_index[terms[0]]);
        } else if (terms.size() == 3) {
            if (!nonterminal_to_index.count(terms[1])) {
                nonterminal_to_index[terms[1]] = nonterminals_count++;
            }
            if (!nonterminal_to_index.count(terms[2])) {
                nonterminal_to_index[terms[2]] = nonterminals_count++;
            }
            map_rules.push_back({nonterminal_to_index[terms[0]], 
                            {nonterminal_to_index[terms[1]], nonterminal_to_index[terms[2]]}});
        }
    }
    chomsky_stream.close();
    
    rules_count = map_rules.size();    
    rules = new unsigned int[rules_count * 3]();   
    for (int i = 0; i < rules_count; i++) {
        rules[i * 3] = map_rules[i].first;
        rules[i * 3 + 1] = map_rules[i].second.first;
        rules[i * 3 + 2] = map_rules[i].second.second;
    }

}

Grammar::~Grammar() {}
