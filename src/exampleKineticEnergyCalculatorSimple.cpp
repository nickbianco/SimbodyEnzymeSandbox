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

struct MyState {
    std::vector<double> u;
};

class MySystem {
public:
    MySystem() = default;

    double calcKineticEnergy(const MyState& state) const {
        return 0.5 * state.u[0] * state.u[0] + 0.5 * state.u[1] * state.u[1];
    }
};

class KineticEnergyCalculatorSimple {
public:
    KineticEnergyCalculatorSimple(const MySystem& system) : m_system(system) {
        m_state.u = std::vector<double>(2, 0.0);
    }

    double calcKineticEnergy(double u0, double u1) const {
        m_state.u[0] = u0;
        m_state.u[1] = u1;
        return m_system.calcKineticEnergy(m_state);
    }

private:
    const MySystem& m_system;
    mutable MyState m_state;

};

template < typename T, typename ... arg_types >
auto wrapper(T obj, arg_types && ... args) {
    return obj.calcKineticEnergy(args ...);
}

int main() {

    MySystem system;
    KineticEnergyCalculatorSimple calculator(system);
    double u0 = 1.7;
    double u1 = 2.3;
    double energy;
    double KE = calculator.calcKineticEnergy(u0, u1);

    printf("KE_analytic = %f\n", 0.5 * u0 * u0 + 0.5 * u1 * u1);
    printf("KE = %f\n", KE); 

    double du0 = 1.0;
    double du1 = 0.0;
    double dKEdu0 = __enzyme_fwddiff<double>((void*)wrapper<KineticEnergyCalculatorSimple, double, double>, 
            enzyme_const, calculator, 
            enzyme_dup, &u0, &du0,
            enzyme_dup, &u1, &du1);

    printf("dKEdu0_analytic = %f\n", u0);
    printf("dKEdu0_enzyme = %f\n", dKEdu0); 

    return 0;
}
