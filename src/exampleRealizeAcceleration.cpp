#include <cstdio>
#include "Simbody.h"

using namespace SimTK;

extern int enzyme_dup;
extern int enzyme_dupnoneed;
extern int enzyme_out;
extern int enzyme_const;

template <typename return_type, typename... T>
return_type __enzyme_fwddiff(void *, T...);

template <typename return_type, typename... T>
return_type __enzyme_autodiff(void *, T...);

void wrapper(MultibodySystem* system, State* state) {
    system->realize(*state, Stage::Acceleration);
}

int main() {

    // Define the system.
    MultibodySystem system;
    SimbodyMatterSubsystem matter(system);
    GeneralForceSubsystem forces(system);
    Force::Gravity gravity(forces, matter, -YAxis, 9.8);

    // Describe mass and visualization properties for a generic body.
    Body::Rigid bodyInfo(MassProperties(1.0, Vec3(0), UnitInertia(1)));
    bodyInfo.addDecoration(Transform(), DecorativeSphere(0.1));

    // Create the moving (mobilized) bodies of the pendulum.
    MobilizedBody::Pin pendulum1(matter.Ground(), Transform(Vec3(0)),
            bodyInfo, Transform(Vec3(0, 1, 0)));
    MobilizedBody::Pin pendulum2(pendulum1, Transform(Vec3(0)),
            bodyInfo, Transform(Vec3(0, 1, 0)));

    SimTK::State state = system.realizeTopology();

    state.updQ()[0] = 1.8;
    state.updQ()[1] = 2.3;
    state.updU()[0] = -0.6;
    state.updU()[1] = 0.7;
    wrapper(&system, &state);
    printf("state.getQ()[0] = %f\n", state.getQ()[0]);
    printf("state.getQ()[1] = %f\n", state.getQ()[1]);
    printf("state.getU()[0] = %f\n", state.getU()[0]);
    printf("state.getU()[1] = %f\n", state.getU()[1]);
    printf("state.getUDot()[0] = %f\n", state.getUDot()[0]);
    printf("state.getUDot()[1] = %f\n", state.getUDot()[1]);

    State dstate = state;
    dstate.updQ() = 0.0;
    dstate.updU() = 0.0;
    __enzyme_fwddiff<Real>((void*)wrapper, 
            enzyme_const, &system, 
            enzyme_dup, &state, &dstate);
    printf("dstate.getQ()[0] = %f\n", dstate.getQ()[0]);
    printf("dstate.getQ()[1] = %f\n", dstate.getQ()[1]);
    printf("dstate.getU()[0] = %f\n", dstate.getU()[0]);
    printf("dstate.getU()[1] = %f\n", dstate.getU()[1]);
    printf("dstate.getUDot()[0] = %f\n", dstate.getUDot()[0]);
    printf("dstate.getUDot()[1] = %f\n", dstate.getUDot()[1]);

    return 0;
}
