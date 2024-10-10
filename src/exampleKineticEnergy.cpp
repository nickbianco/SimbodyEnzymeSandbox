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

void exampleSlidingMass() {
    // Define the system.
    MultibodySystem system;
    SimbodyMatterSubsystem matter(system);
    GeneralForceSubsystem forces(system);
    Force::Gravity gravity(forces, matter, -YAxis, 9.8);

    // Describe mass and visualization properties for a generic body.
    Real mass = 2.3;
    Body::Rigid bodyInfo(MassProperties(mass, Vec3(0), UnitInertia(0)));
    bodyInfo.addDecoration(Transform(), DecorativeSphere(0.1));

    // Create a slider mobilized body in the x-direction.
    MobilizedBody::Slider slider(matter.Ground(), Transform(Vec3(0)),
            bodyInfo, Transform(Vec3(1, 0, 0)));

    // Initialize the system and state.
    State state = system.realizeTopology();

    // Calculate the kinetic energy of the system.
    //
    // KE = 0.5 * m * u^2
    //
    double u = 1.7;
    state.updY()= 0.0;
    state.updU()[0] = u;
    Real KE = wrapper(&system, &state);
    printf("KE_analytic = %f\n", 0.5 * mass * u * u);
    printf("KE_simbody = %f\n", KE); 

    // Calculate the derivative of the kinetic energy.
    //
    // dKEdu = m * u
    //
    State dstate = state;
    dstate.updY() = 0.0;
    Real dKEdu = __enzyme_fwddiff<Real>((void*)wrapper, 
            enzyme_const, &system, 
            enzyme_dup, &state, &dstate);
    printf("dKEdu_analytic = %f\n", mass * u);
    printf("dKEdu_enzyme = %f\n", dKEdu); // is this actually dKE_dstate? what does that mean?

    // Even with LLDEnzymeLooseTypeFlags enabled, the following error is thrown
    // when using __enzyme_autodiff:
    //
    // [build] ld.lld: error: <unknown>:0:0: in function preprocess__ZNK5SimTK6System4Guts7realizeERKNS_5StateENS_5StageE void (ptr, ptr, i32): Enzyme: No create nofree of unknown value
    // [build] ptr %0
    // [build]  at context:   %26 = tail call noundef i32 %25(ptr noundef nonnull align 8 dereferenceable(16) %0, ptr noundef nonnull align 8 dereferenceable(8) %1) #62 
    // State dstate = state;
    // dstate.updY() = 0.0;
    // __enzyme_autodiff<Real>((void*)wrapper, 
    //         enzyme_const, &system, 
    //         enzyme_dup, &state, &dstate);
}

void exampleDoublePendulum() {
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
}

int main () {

    exampleSlidingMass();

    exampleDoublePendulum();

    return 0;
}