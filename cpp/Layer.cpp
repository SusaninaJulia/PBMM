#include <algorithm>

#include "Bound.h"
#include "Layer.h"

Layer::Layer(const Bound *rf, unsigned long sz) {
    size = sz;
    layerBegin = rf;
    layerEnd = rf + sz;
}

int Layer::getMtrxSize() {
    return (*layerBegin).len;
}

void Layer::getLayer(std::vector<Bound>::iterator rf) {

    transform(layerBegin, layerEnd, rf,
              [](Bound bnd) -> Bound {return Bound(bnd.l1, bnd.av1, bnd.l2, bnd.av2);});
    transform(layerBegin, layerEnd, rf + size,
              [](Bound bnd) -> Bound {return Bound(bnd.av1, bnd.m1, bnd.av2, bnd.m2);});
    transform(layerBegin, layerEnd, rf + 2*size,
              [](Bound bnd) -> Bound {return Bound(bnd.l1, bnd.av1, bnd.av2,  bnd.m2);});
    transform(layerBegin, layerEnd, rf + 3*size,
              [](Bound bnd) -> Bound {return Bound(bnd.av1, bnd.m1, bnd.l2, bnd.av2);});
}

void Layer::getLayer(std::vector<Bound>::iterator rf, bool full) {
    transform(layerBegin, layerEnd, rf,
              [](Bound bnd) -> Bound {return Bound(bnd.l1, bnd.av1, bnd.l2, bnd.av2);});
    transform(layerBegin, layerEnd, rf + size,
              [](Bound bnd) -> Bound {return Bound(bnd.av1, bnd.m1, bnd.av2, bnd.m2);});
    transform(layerBegin, layerEnd, rf + 2*size,
              [](Bound bnd) -> Bound {return Bound(bnd.l1, bnd.av1, bnd.av2,  bnd.m2);});
    transform(layerBegin, layerEnd, rf + 3*size,
              [](Bound bnd) -> Bound {return Bound(bnd.av1, bnd.m1, bnd.l2, bnd.av2);});
    transform(layerBegin, layerEnd, rf + 4*size,
              [](Bound bnd) -> Bound {return Bound(bnd.l1, bnd.av1, bnd.av1, bnd.m1);});
    transform(layerBegin, layerEnd, rf + 5*size,
              [](Bound bnd) -> Bound {return Bound(bnd.l2, bnd.av2, bnd.av2, bnd.m2);});

}
