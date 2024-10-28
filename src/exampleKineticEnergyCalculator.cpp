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

class KineticEnergyCalculator {
public:
    KineticEnergyCalculator(const MultibodySystem& system) : m_system(system) {
        m_state = m_system.realizeTopology();
    }

    Real calcKineticEnergy(Real u0, Real u1) const {
        m_state.updU()[0] = u0;
        m_state.updU()[1] = u1;
        m_system.realize(m_state, Stage::Velocity);
        return m_system.calcKineticEnergy(m_state);
    }

private:
    const MultibodySystem& m_system;
    mutable State m_state;
};

auto wrapper(const KineticEnergyCalculator& obj, Real u0, Real u1) {
    return obj.calcKineticEnergy(u0, u1);
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

    KineticEnergyCalculator calculator(system);
    Real u0 = 1.0;
    Real u1 = 2.0;
    Real KE = calculator.calcKineticEnergy(u0, u1);
    printf("KE = %f\n", KE); 

    KineticEnergyCalculator dcalculator(system);
    Real du0 = 1.0;
    Real du1 = 0.0;
    Real dKE = __enzyme_fwddiff<Real>((void*)wrapper, 
            enzyme_dup, &calculator, &dcalculator, 
            enzyme_dup, u0, du0,
            enzyme_dup, u1, du1);
    printf("dKE = %f\n", dKE); 

    return 0;
}
