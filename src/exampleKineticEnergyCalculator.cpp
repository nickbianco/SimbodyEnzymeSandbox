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

// class KineticEnergyCalculator {
// public:
//     KineticEnergyCalculator(const MultibodySystem& system, 
//             const SimTK::State& state) : m_system(system), m_state(state) {
//         m_state.updQ() = 0;
//         m_state.updU() = 0;
//         m_system.realize(m_state, Stage::Time); 
//     }

//     Real calcKineticEnergy(const Real& u0, const Real& u1) const {
//         m_state.updU()[0] = u0;
//         m_state.updU()[1] = u1;
//         m_system.realize(m_state, Stage::Velocity);
//         return m_system.calcKineticEnergy(m_state);
//     }

// private:
//     const MultibodySystem& m_system;
//     mutable State m_state;

// };

class KineticEnergyCalculator {
public:
    KineticEnergyCalculator(const MultibodySystem& system) : m_system(system) {}

    Real calcKineticEnergy(const Real& u0, const Real& u1,
            SimTK::State& state) const {
        state.updU()[0] = u0;
        state.updU()[1] = u1;
        m_system.realize(state, Stage::Velocity);
        return m_system.calcKineticEnergy(state);
    }

private:
    const MultibodySystem& m_system;
};

template < typename T, typename ... arg_types >
auto wrapper(T obj, arg_types && ... args) {
    return obj.calcKineticEnergy(args ...);
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

    KineticEnergyCalculator calculator(system);
    Real u0 = 1.0;
    Real u1 = 2.0;
    state.updU()[0] = u0;
    state.updU()[1] = u1;
    Real energy;
    Real KE = calculator.calcKineticEnergy(u0, u1, state);
    printf("KE = %f\n", KE); 

    state.updQ() = 0.0;
    state.updU() = 0.0;

    Real du0 = 1.0;
    Real du1 = 0.0;
    Real dKE = __enzyme_fwddiff<Real>((void*)wrapper<KineticEnergyCalculator, Real, Real, SimTK::State>, 
            enzyme_const, calculator, 
            enzyme_dup, &u0, &du0,
            enzyme_dup, &u1, &du1,
            enzyme_const, &state);
    printf("dKE = %f\n", dKE); 

    return 0;
}
