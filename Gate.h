#ifndef GATE_H
#define GATE_H

#include <string>
#include <cmath>

class Gate {
private:
    virtual std::string gateType() const = 0;

protected:
    double P;

public:
    Gate(double initialP) : P(initialP) {}
    virtual ~Gate() = default;
    Gate(const Gate& other) : P(other.P) {}
    
    virtual double alpha(double V) = 0;
    virtual double beta(double V) = 0;
    
    void updD(double V, double dt) {
        double dP = alpha(V) * (1 - P) - beta(V) * P;
        P += dP * dt;
    }
    
    double steadyState(double V) { 
        return alpha(V) / (alpha(V) + beta(V)); 
    }

    double timeConstant(double V) { 
        return 1.0 / (alpha(V) + beta(V)); 
    }
    
    double getP() const { return P; }

    void print(std::ostream& os) const {
        os << "Gate [P=" << P << ", type=" << gateType() << "]";
    }
};

class MGate : public Gate {
private:
    std::string gateType() const override { 
        return "M (Na activation)"; 
    }
public:
    MGate(double initialP) : Gate(initialP) {}
    
    double alpha(double V) override {
        return 0.1 * (V + 40) / (1 - exp(-(V + 40) / 10));
    }
    
    double beta(double V) override {
        return 4 * exp(-(V + 65) / 18);
    }
};

class HGate : public Gate {
private:
    std::string gateType() const override { 
        return "H (K activation)"; 
    }
public:
    HGate(double initialP) : Gate(initialP) {}
    
    double alpha(double V) override {
        return 0.07 * exp(-(V + 65) / 20);
    }
    
    double beta(double V) override {
        return 1 / (exp(-(V + 35) / 10) + 1);
    }
};

class NGate : public Gate {
private:
    std::string gateType() const override { 
        return "N (K inactivation)"; 
    }
public:
    NGate(double initialP) : Gate(initialP) {}
    
    double alpha(double V) override {
        return 0.01 * (V + 55) / (1 - exp(-(V + 55) / 10));
    }
    
    double beta(double V) override {
        return 0.125 * exp(-(V + 65) / 80);
    }
};

//boom
