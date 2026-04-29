#include "Gate.h"
#include <cmath>

Gate::Gate(double initialP) : P(initialP) {}

Gate::Gate(const Gate& other) : P(other.P) {}

void Gate::updD(double V, double dt) {
    double dP = alpha(V) * (1 - P) - beta(V) * P;
    P += dP * dt;
}

double Gate::steadyState(double V) {
    return alpha(V) / (alpha(V) + beta(V));
}

double Gate::timeConstant(double V) {
    return 1.0 / (alpha(V) + beta(V));
}

double Gate::getP() const {
    return P;
}

void Gate::print(std::ostream& os) const {
    os << "Gate [P=" << P << ", type=" << gateType() << "]";
}
