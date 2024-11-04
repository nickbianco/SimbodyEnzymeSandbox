#include <cstdio>
#include "Simbody.h"

using namespace SimTK;

extern int enzyme_dup;
extern int enzyme_dupnoneed;
extern int enzyme_out;
extern int enzyme_const;

// extern double __enzyme_fwddiff(void *, ...);
// extern double __enzyme_autodiff(void *, ...);

template <typename return_type, typename... T>
return_type __enzyme_fwddiff(void *, T...);

template <typename return_type, typename... T>
return_type __enzyme_autodiff(void *, T...);

Real getUAtIndex(State* x, int idx) {
    return x->getU()[idx];
}

Real foo(State *x) { return getUAtIndex(x, 0) + getUAtIndex(x, 1); }

int main() {

    // Define the system.
    MultibodySystem system;
    SimbodyMatterSubsystem matter(system);
    GeneralForceSubsystem forces(system);
    Force::Gravity gravity(forces, matter, -YAxis, 9.8);

    // Describe mass and visualization properties for a generic body.
    Real mass = 1.0;
    Body::Rigid bodyInfo(MassProperties(mass, Vec3(0), UnitInertia(0)));
    bodyInfo.addDecoration(Transform(), DecorativeSphere(0.1));

    // Create a slider mobilized body in the x-direction.
    MobilizedBody::Slider slider(matter.Ground(), Transform(Vec3(0)),
            bodyInfo, Transform(Vec3(1, 0, 0)));

    // Initialize the system and state.
    State state = system.realizeTopology();
    state.updU()[0] = 2.7;
    state.updU()[1] = 3.1;
    State dstate = system.realizeTopology();
    dstate.updU()[0] = 0.2;
    dstate.updU()[1] = 0.5;

    Real res = __enzyme_fwddiff<Real>((void*)foo, enzyme_dup, &state, &dstate);
    printf("res=%f\n", res);
}