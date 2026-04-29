#include "NGate.h"
#include <cmath>

std::string NGate::gateType() const {
    return "N (K inactivation)";
}

NGate::NGate(double initialP) : Gate(initialP) {}

double NGate::alpha(double V) {
    return 0.01 * (V + 55) / (1 - exp(-(V + 55) / 10));
}

double NGate::beta(double V) {
    return 0.125 * exp(-(V + 65) / 80);
}
