#include "CaMGate.h"
#include <cmath>

std::string CaMGate::gateType() const {
    return "CaM (Ca activation)";
}

CaMGate::CaMGate(double initialP) : Gate(initialP) {}

double CaMGate::alpha(double V) {
    return 0.055 * (V + 27) / (1 - exp(-(V + 27) / 3.8));
}

double CaMGate::beta(double V) {
    return 0.94 * exp(-(V + 75) / 17);
}
