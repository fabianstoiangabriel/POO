#ifndef CAMGATE_H
#define CAMGATE_H

#include "Gate.h"

class CaMGate : public Gate {
private:
    std::string gateType() const override;

public:
    CaMGate(double initialP);
    double alpha(double V) override;
    double beta(double V) override;
};

#endif // CAMGATE_H
