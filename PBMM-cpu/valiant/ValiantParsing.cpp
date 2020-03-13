#include <m4ri/m4ri_config.h>
#include <m4ri/m4ri.h>
#include <m4ri/semiring/mzd_sr.h>
#include <m4ri/semiring/russian.h>

#include <iostream>

#include "ValiantParsing.h"


void ValiantParsing::valiantComplete(Bound bnd) {

    if (bnd.len > 1) {
        
        auto mtxsz = bnd.len / 2;

        mzd_t *A = mzd_init(mtxsz, mtxsz);
        mzd_t *B = mzd_init(mtxsz, mtxsz);
        mzd_t *C = mzd_init(mtxsz, mtxsz);

        Bound b1 = Bound(bnd.l1, bnd.av1, bnd.av1, bnd.m1);
        Bound b2 = Bound(bnd.l2, bnd.av2, bnd.av2, bnd.m2);
        Bound c = Bound(bnd.av1, bnd.m1, bnd.l2, bnd.av2);
        Bound d1 = Bound(bnd.l1, bnd.av1, bnd.l2, bnd.av2);
        Bound d2 = Bound(bnd.av1, bnd.m1, bnd.av2, bnd.m2);
        Bound e = Bound(bnd.l1, bnd.av1, bnd.av2,  bnd.m2);

        valiantComplete(c);
        subMatricesMult(b1, c, d1, A, B, C);
        valiantComplete(d1);
        subMatricesMult(c, b2, d2, A, B, C);
        valiantComplete(d2);
        subMatricesMult(b1, d2, e, A, B, C);
        subMatricesMult(d1, b2, e, A, B, C);
        valiantComplete(e);


        mzd_free(A);
        mzd_free(B);
        mzd_free(C);
    }
}
void ValiantParsing::valiantCompute(int l, int m){
    int av = (l + m)/ 2;
    if (m - l  >= 4) {
        valiantCompute(l, av);
        valiantCompute(av, m);
    }
    valiantComplete({l, av, av, m});
}

bool ValiantParsing::parseString() {
    valiantCompute(0, n);
    return static_cast<bool>(mzd_read_bit(Ts[0], 0, n - 1));
}
