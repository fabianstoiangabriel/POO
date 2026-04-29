#ifndef GATE_H
#define GATE_H

#include <iostream>
#include <string>

class Gate {
private:
    virtual std::string gateType() const = 0;

protected:
    double P;

public:
    Gate(double initialP);
    virtual ~Gate() = default;
    Gate(const Gate& other);

    virtual double alpha(double V) = 0;
    virtual double beta(double V) = 0;
    void updD(double V, double dt);
    double steadyState(double V);
    double timeConstant(double V);
    double getP() const;
    void print(std::ostream& os) const;
};

#endif // GATE_H
