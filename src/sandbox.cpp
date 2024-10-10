#include <SimTKcommon/Testing.h>
#include <SimTKcommon/internal/ReferencePtr.h>
#include <cstdio>
#include "Simbody.h"
#include <SimTKcommon.h>
#include <SimTKcommon/internal/BigMatrix.h>
#include <memory>
#include <simbody/internal/MultibodySystem.h>

using namespace SimTK;

extern int enzyme_dup;
extern int enzyme_dupnoneed;
extern int enzyme_out;
extern int enzyme_const;

template <typename return_type, typename... T>
return_type __enzyme_fwddiff(void *, T...);

template <typename return_type, typename... T>
return_type __enzyme_autodiff(void *, T...);



// class VectorMultiply {
// public:
//     VectorMultiply(SimTK::Vec3 vec1) : vec1(vec1) {}
//     SimTK::Real f(const SimTK::Vec3& vec2) const {
//         // std::cout << "vec1 = " << *vec1 << std::endl;
//         return vec2.transpose() * vec1;
//     }
// private:
//     // SimTK::ReferencePtr<SimTK::Vec3> vec1;
//     SimTK::Vec3 vec1;
// };

// void testReferencePtr() {
//     SimTK::Vec3 vec1(1.0, 2.0, 3.0);
//     VectorMultiply vecNorm(vec1);

//     SimTK::Vec3 vec2(4.0, 5.0, 6.0);
//     SimTK::Vec3 dvec2(1.0, 1.0, 1.0);

//     printf("VectorMultiply::f() = %f\n", vecNorm.f(vec2));

//     SimTK::Real dfdvec2 = __enzyme_fwddiff<SimTK::Real>((void*)wrapper<VectorMultiply, SimTK::Vec3>, 
//         enzyme_const, vecNorm, 
//         enzyme_dup, &vec2, &dvec2);

//     printf("dfdvec2 = %f\n", dfdvec2);
//     printf("vec2 = %f, %f, %f\n", vec2[0], vec2[1], vec2[2]);
//     printf("dvec2 = %f, %f, %f\n", dvec2[0], dvec2[1], dvec2[2]);

// }

// class KineticEnergyCalculator { 
// public:
//     KineticEnergyCalculator(const MultibodySystem& system) : m_system(system) {
//         m_state = m_system.realizeTopology();
//     }

//     double f(double u0, double u1) {
//         SimTK::Vector u = m_state.getU();
//         u[0] = u0;
//         u[1] = u1;
//         m_state.setU(u);
//         m_system.realize(m_state, Stage::Acceleration);
//         return m_system.calcKineticEnergy(m_state);
//     }

// private:
//     const MultibodySystem& m_system;
//     State m_state;
// };



Real wrapper(MultibodySystem* system, State* state) {
    system->realize(*state, Stage::Velocity);
    return system->calcKineticEnergy(*state);
}

void testKineticEnergy() {

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

// class KineticEnergyCalculator {
// public:
//     KineticEnergyCalculator() {

//         // Define the system.
//         MultibodySystem system;
//         SimbodyMatterSubsystem matter(system);
//         GeneralForceSubsystem forces(system);
//         Force::Gravity gravity(forces, matter, -YAxis, 9.8);

//         // Describe mass and visualization properties for a generic body.
//         Body::Rigid bodyInfo(MassProperties(1.0, Vec3(0), UnitInertia(1)));
//         bodyInfo.addDecoration(Transform(), DecorativeSphere(0.1));

//         // Create the moving (mobilized) bodies of the pendulum.
//         MobilizedBody::Pin pendulum1(matter.Ground(), Transform(Vec3(0)),
//                 bodyInfo, Transform(Vec3(0, 1, 0)));
//         MobilizedBody::Pin pendulum2(pendulum1, Transform(Vec3(0)),
//                 bodyInfo, Transform(Vec3(0, 1, 0)));

//         m_system = &system;

//         m_state = m_system->realizeTopology();
//         m_state.updQ() = 0;
//         m_state.updU() = 0;
//         m_system->realize(m_state, Stage::Time); 
//     }

//     Real calcKineticEnergy(const Real& u0, const Real u1) const {
//         m_state.updU()[0] = u0;
//         m_state.updU()[1] = u1;
//         m_system->realize(m_state, Stage::Velocity);
//         return m_system->calcKineticEnergy(m_state);
//     }

// private:
//     ReferencePtr<MultibodySystem> m_system;
//     mutable State m_state;

// };

// template < typename T, typename ... arg_types >
// auto wrapper(T obj, arg_types && ... args) {
//     return obj.calcKineticEnergy(args ...);
// }

// void testKineticEnergy2() {

//     KineticEnergyCalculator calculator;
//     Real u0 = 1.0;
//     Real u1 = 2.0;
//     Real energy;
//     Real KE = calculator.calcKineticEnergy(u0, u1);
//     printf("KE = %f\n", KE); 

//     Real du0 = 1.0;
//     Real du1 = 0.0;
//     Real dKE = __enzyme_fwddiff<Real>((void*)wrapper<KineticEnergyCalculator, Real, Real>, 
//             enzyme_const, calculator, 
//             enzyme_dup, &u0, &du0,
//             enzyme_dup, &u1, &du1);
//     printf("dKE = %f\n", dKE); 

// }

int main () {
    testKineticEnergy();
    return 0;
}