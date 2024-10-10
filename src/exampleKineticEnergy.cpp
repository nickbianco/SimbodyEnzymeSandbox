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

Real wrapper(MultibodySystem* system, State* state) {
    system->realize(*state, Stage::Velocity);
    return system->calcKineticEnergy(*state);
}

int main () {
    
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

    State state = system.realizeTopology();
    system.realize(state, Stage::Velocity);

    // Calculate the kinetic energy of the system.
    double u0 = 1.0;
    double u1 = 2.0;
    Vector u = state.getU();
    u[0] = u0;
    u[1] = u1;
    state.setU(u);
    Real KE = wrapper(&system, &state);
    printf("KE = %f\n", KE); 

    State dstate = state;
    state.updU()[0] = 0.0;
    state.updU()[1] = 1.0;
    Real dKE = __enzyme_fwddiff<Real>((void*)wrapper, 
            enzyme_const, &system, 
            enzyme_dup, &state, &dstate);
    printf("dKE = %f\n", dKE); 

    return 0;
}