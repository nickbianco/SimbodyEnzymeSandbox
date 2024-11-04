#include <cstdio>
#include "Simbody.h"

using namespace SimTK;

extern int enzyme_dup;

template <typename return_type, typename... T>
return_type __enzyme_fwddiff(void *, T...);


Real foo(const Matrix& mat) {
    // Grab the second column as a VectorView and from that view,
    // grab a subvector (another VectorView) from the second element
    // to the third element. Finally, sum the elements of the subvector.
    VectorView vec = mat.col(1)(1, 2);
    return vec(0) + vec(1);
}

Real bar(const Matrix& mat) {
    // Grab and sum the same elements as in foo(), but directly.
    return mat.getElt(1, 1) + mat.getElt(1, 2);
}

int main () {

    Matrix mat = Test::randMatrix(4, 4);
    Matrix dmat(4, 4, 0.0);
    dmat(1, 1) = 1.0;
    dmat(1, 2) = 1.0;

    Real dfoo = __enzyme_fwddiff<Real>((void*)foo, 
            enzyme_dup, &mat, &dmat);

    return 0;
}