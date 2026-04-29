#include "CaHGate.h"
#include <cmath>

std::string CaHGate::gateType() const {
    return "CaH (Ca inactivation)";
}

CaHGate::CaHGate(double initialP) : Gate(initialP) {}

double CaHGate::alpha(double V) {
    return 0.000457 * exp(-(V + 13) / 50);
}

double CaHGate::beta(double V) {
    return 0.0065 / (1 + exp(-(V + 15) / 28));
}
