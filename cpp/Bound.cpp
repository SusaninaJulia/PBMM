#include "Bound.h"

Bound::Bound() {
    Bound(0,0,0,0);
}
Bound::Bound(int i1, int i2, int i3, int i4) {
    l1 = i1; m1 = i2; l2 = i3; m2 = i4;
    len = m1 - l1;
    av1 = (m1 + l1) / 2;
    av2 = (m2 + l2) / 2;
}


