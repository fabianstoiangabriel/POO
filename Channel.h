#ifndef CHANNEL_H
#define CHANNEL_H

#include <vector>
#include "Gate.h"
#include "SimulationExceptions.h"

class Channel {
protected:
    std::vector<Gate*> gates;
    double gIon;
    double EIon;

public:
    Channel(double gIon, double EIon);
    virtual ~Channel();

    virtual double current(double V) = 0;
    void updCurrent(double V, double dt);
    double gateOpenProbability();
    double getGIon() const;
    double getEIon() const;
    virtual Channel* clone() const = 0;

protected:
    Channel(const Channel& other);
};

#endif // CHANNEL_H
