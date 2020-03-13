#ifndef BOUND_H
#define BOUND_H

struct Bound {
    int l1;
    int m1;
    int l2;
    int m2;

    int len;
    int av1;
    int av2;

    Bound();
    Bound(int i1, int i2, int i3, int i4);

};

#endif //BOUND_H
