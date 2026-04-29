#ifndef CAHGATE_H
#define CAHGATE_H

#include "Gate.h"

class CaHGate : public Gate {
private:
    std::string gateType() const override;

public:
    CaHGate(double initialP);
    double alpha(double V) override;
    double beta(double V) override;
};

#endif // CAHGATE_H
