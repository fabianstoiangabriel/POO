#ifndef MGATE_H
#define MGATE_H

#include "Gate.h"

class MGate : public Gate {
private:
    std::string gateType() const override;

public:
    MGate(double initialP);
    double alpha(double V) override;
    double beta(double V) override;
};

#endif // MGATE_H
