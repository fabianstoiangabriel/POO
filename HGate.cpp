#include "HGate.h"
#include <cmath>

std::string HGate::gateType() const {
    return "H (K activation)";
}

HGate::HGate(double initialP) : Gate(initialP) {}

double HGate::alpha(double V) {
    return 0.07 * exp(-(V + 65) / 20);
}

double HGate::beta(double V) {
    return 1 / (exp(-(V + 35) / 10) + 1);
}
