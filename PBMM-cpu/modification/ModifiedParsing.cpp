#include <m4ri/m4ri_config.h>
#include <m4ri/m4ri.h>
#include <m4ri/semiring/mzd_sr.h>
#include <m4ri/semiring/russian.h>

#include <iostream>

#include "ModifiedParsing.h"


void ModifiedParsing::completeLayer(Layer subM) {

    if (subM.getMtrxSize() > 1) {

        int tmpsz = static_cast<int>(subM.size);
        std::vector<Bound> layer(6*tmpsz);
        auto layerit = layer.begin();
        subM.getLayer(layerit, true);
        completeLayer(Layer(&layer[0] + 3*tmpsz, tmpsz));

        auto mtxsz = subM.getMtrxSize() / 2;

        mzd_t *A = mzd_init(mtxsz, mtxsz);
        mzd_t *B = mzd_init(mtxsz, mtxsz);
        mzd_t *C = mzd_init(mtxsz, mtxsz);

        int i;
        for(i = 0; i < tmpsz; i++)
            subMatricesMult(layer[4*tmpsz + i], layer[3*tmpsz + i], layer[i], A, B, C);

        for(i = 0; i < tmpsz; i++)
            subMatricesMult(layer[3*tmpsz + i], layer[5*tmpsz + i], layer[tmpsz + i], A, B, C);
        completeLayer(Layer(&layer[0], 2*tmpsz));

        for(i = 0; i < tmpsz; i++) {
            subMatricesMult(layer[4*tmpsz + i], layer[tmpsz + i], layer[2*tmpsz + i], A, B, C);
            subMatricesMult(layer[i], layer[5*tmpsz + i], layer[2*tmpsz + i], A, B, C);
        }

        completeLayer(Layer(&layer[0] + 2*tmpsz, tmpsz));

        mzd_free(A);
        mzd_free(B);
        mzd_free(C);
    }

}
void ModifiedParsing::completeVLayer(Layer subVM) {

    int tmpsz = static_cast<int>(subVM.size);
    std::vector<Bound> layer(6*tmpsz);
    auto layerit = layer.begin();
    subVM.getLayer(layerit, true);

    auto mtxsz = subVM.getMtrxSize() / 2;

    mzd_t *A = mzd_init(mtxsz, mtxsz);
    mzd_t *B = mzd_init(mtxsz, mtxsz);
    mzd_t *C = mzd_init(mtxsz, mtxsz);

    int i = 0;
    for (i = 0; i < tmpsz; i++) {
        subMatricesMult(layer[4 * tmpsz + i], layer[3 * tmpsz + i], layer[i], A, B, C);
        subMatricesMult(layer[3 * tmpsz + i], layer[5 * tmpsz + i], layer[tmpsz + i], A, B, C);
    }

    completeLayer(Layer(&layer[0], 2 * tmpsz));
    for (i = 0; i < tmpsz; i++) {
        subMatricesMult(layer[4 * tmpsz + i], layer[tmpsz + i], layer[2 * tmpsz + i], A, B, C);
        subMatricesMult(layer[i], layer[5 * tmpsz + i], layer[2 * tmpsz + i], A, B, C);
    }

    completeLayer(Layer(&layer[0] + 2 * tmpsz, tmpsz));

    mzd_free(A);
    mzd_free(B);
    mzd_free(C);
}
void ModifiedParsing::constructLayer(int i){
    std::vector<Bound> layer;
    int pow2i = pow(2, i);
    for(int k = 0; k < ((n / pow2i) - 1); k++) {
        Bound b = Bound(k*pow2i, (k + 1)*pow2i, (k + 1)*pow2i, (k + 2)*pow2i);
        layer.push_back(b);
    }
    completeVLayer(Layer(&layer[0], layer.size()));
}

bool ModifiedParsing::parseString() {
    for (int i = 1; i < log2(n); i++)
        constructLayer(i);
    return static_cast<bool>(mzd_read_bit(Ts[0], 0, n - 1));
}
std::vector<std::pair<int, int>> ModifiedParsing::parseSubString(int s) {
    double ln = ceil(log2(s));
    for(int i = 1; i <= ln; i++)
        constructLayer(i);
    std::vector<std::pair<int, int>> correctStrings;
    for(int j = 0; j <= n - s; j++) {
        if (static_cast<bool>(mzd_read_bit(Ts[0], j, s + j)))
            correctStrings.emplace_back(j, s + j);
    }
    return correctStrings;
}

