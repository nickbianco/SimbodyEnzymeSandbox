#include <cstdio>
#include <SimTKcommon.h>
#include <SimTKcommon/internal/BigMatrix.h>

extern int enzyme_dup;
extern int enzyme_dupnoneed;
extern int enzyme_out;
extern int enzyme_const;

template <typename return_type, typename... T>
return_type __enzyme_fwddiff(void *, T...);

template <typename return_type, typename... T>
return_type __enzyme_autodiff(void *, T...);

double f(double x, double y, double z) {
    SimTK::Vec3 vec(x, y, z);
    return vec.norm();
}

void testSimpleNorm() {
    double x = 1.0;
    double y = 2.0;
    double z = 3.0;
    double dx = 1.0;
    double dy = 1.0;
    double dz = 1.0;

    double df = __enzyme_fwddiff<double>((void*)f,
                        enzyme_dup, x, dx,
                        enzyme_dup, y, dy,
                        enzyme_dup, z, dz);

    printf("df = %f\n", df);
}

int main () {
    testSimpleNorm();
    return 0;
}