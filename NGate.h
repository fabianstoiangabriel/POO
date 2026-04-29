#ifndef NGATE_H
#define NGATE_H

#include "Gate.h"

class NGate : public Gate {
private:
    std::string gateType() const override;

public:
    NGate(double initialP);
    double alpha(double V) override;
    double beta(double V) override;
};

#endif // NGATE_H
