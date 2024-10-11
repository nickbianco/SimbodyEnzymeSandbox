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

int main () {
    double x = 1.0;
    double y = 2.0;
    double z = 3.0;

    double dx = 1.0;
    double dy = 0.0;
    double dz = 0.0;
    double dfdx = __enzyme_fwddiff<double>((void*)f,
                        enzyme_dup, x, dx,
                        enzyme_dup, y, dy,
                        enzyme_dup, z, dz);
    printf("dfdx_analytic = %f\n", x / f(x, y, z));
    printf("dfdx_enzyme = %f\n", dfdx);

    dx = 0.0;
    dy = 1.0;
    dz = 0.0;
    double dfdy = __enzyme_fwddiff<double>((void*)f,
                        enzyme_dup, x, dx,
                        enzyme_dup, y, dy,
                        enzyme_dup, z, dz);
    printf("dfdy_analytic = %f\n", y / f(x, y, z));
    printf("dfdy_enzyme = %f\n", dfdy);

    dx = 0.0;
    dy = 0.0;
    dz = 1.0;
    double dfdz = __enzyme_fwddiff<double>((void*)f,
                        enzyme_dup, x, dx,
                        enzyme_dup, y, dy,
                        enzyme_dup, z, dz);
    printf("dfdz_analytic = %f\n", z / f(x, y, z));
    printf("dfdz_enzyme = %f\n", dfdz);

    return 0;
}