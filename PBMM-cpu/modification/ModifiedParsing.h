#ifndef PARSING_MODIFIEDPARSING_H
#define PARSING_MODIFIEDPARSING_H

#include "Layer.h"
#include "ParsingTables.h"

class ModifiedParsing : public ParsingTables {
protected:
    void completeLayer(Layer subM);
    void completeVLayer(Layer subVM);
    void constructLayer(int i);
public:
    using ParsingTables::ParsingTables;
    bool parseString() override;
    std::vector<std::pair<int, int>> parseSubString(int s);
};


#endif //PARSING_MODIFIEDPARSING_H
