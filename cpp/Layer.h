#ifndef LAYER_H
#define LAYER_H

#include <vector>
#include "Bound.h"

struct Layer{
    unsigned long size;
    const Bound *layerBegin;
    const Bound *layerEnd;

    Layer(const Bound *rf, unsigned long sz);
    int getMtrxSize();
    void getLayer(std::vector<Bound>::iterator rf);
    void getLayer(std::vector<Bound>::iterator rf, bool full);

};

#endif //LAYER_H
