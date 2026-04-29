#include "Channel.h"

Channel::Channel(double gIon, double EIon) : gIon(gIon), EIon(EIon) {
    if (gIon < 0) {
        throw InvalidConductanceException();
    }
}

Channel::~Channel() {
    for (auto gate : gates) {
        delete gate;
    }
}

Channel::Channel(const Channel& other) : gIon(other.gIon), EIon(other.EIon) {}

void Channel::updCurrent(double V, double dt) {
    for (auto gate : gates)
        gate->updD(V, dt);
}

double Channel::gateOpenProbability() {
    double P = 1.0;
    for (auto gate : gates) {
        P *= gate->getP();
    }
    return P;
}

double Channel::getGIon() const {
    return gIon;
}

double Channel::getEIon() const {
    return EIon;
}
