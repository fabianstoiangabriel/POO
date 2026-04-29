#ifndef HGATE_H
#define HGATE_H

#include "Gate.h"

class HGate : public Gate {
private:
    std::string gateType() const override;

public:
    HGate(double initialP);
    double alpha(double V) override;
    double beta(double V) override;
};

#endif // HGATE_H
