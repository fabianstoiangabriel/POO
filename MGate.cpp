#include "MGate.h"
#include <cmath>

std::string MGate::gateType() const {
    return "M (Na activation)";
}

MGate::MGate(double initialP) : Gate(initialP) {}

double MGate::alpha(double V) {
    return 0.1 * (V + 40) / (1 - exp(-(V + 40) / 10));
}

double MGate::beta(double V) {
    return 4 * exp(-(V + 65) / 18);
}
