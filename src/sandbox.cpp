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

class VectorMultiply {
public:
    VectorMultiply(SimTK::Vec3 vec1) : vec1(vec1) {}
    SimTK::Real f(const SimTK::Vec3& vec2) const {
        // std::cout << "vec1 = " << *vec1 << std::endl;
        return vec2.transpose() * vec1;
    }
private:
    // SimTK::ReferencePtr<SimTK::Vec3> vec1;
    SimTK::Vec3 vec1;
};

template < typename T, typename ... arg_types >
auto wrapper(T obj, arg_types && ... args) {
    return obj.f(args ...);
}

void testReferencePtr() {
    SimTK::Vec3 vec1(1.0, 2.0, 3.0);
    VectorMultiply vecNorm(vec1);

    SimTK::Vec3 vec2(4.0, 5.0, 6.0);
    SimTK::Vec3 dvec2(1.0, 1.0, 1.0);

    printf("VectorMultiply::f() = %f\n", vecNorm.f(vec2));

    SimTK::Real dfdvec2 = __enzyme_fwddiff<SimTK::Real>((void*)wrapper<VectorMultiply, SimTK::Vec3>, 
        enzyme_const, vecNorm, 
        enzyme_dup, &vec2, &dvec2);

    printf("dfdvec2 = %f\n", dfdvec2);
    printf("vec2 = %f, %f, %f\n", vec2[0], vec2[1], vec2[2]);
    printf("dvec2 = %f, %f, %f\n", dvec2[0], dvec2[1], dvec2[2]);

}

int main () {
    testReferencePtr();
    return 0;
}