#ifndef PARSING_VALIANTPARSING_H
#define PARSING_VALIANTPARSING_H

#include "ParsingTables.h"


class ValiantParsing : public ParsingTables {
protected:
    void valiantComplete(Bound bnd);
    void valiantCompute(int l, int m);
public:
    using ParsingTables::ParsingTables;
    bool parseString() override;
};


#endif //PARSING_VALIANTPARSING_H
