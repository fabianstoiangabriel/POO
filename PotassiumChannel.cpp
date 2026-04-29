#include "PotassiumChannel.h"

PotassiumChannel::PotassiumChannel(double gIon, double EIon) : Channel(gIon, EIon) {
    nGate = new NGate(0.32);
    gates.push_back(nGate);
}

PotassiumChannel::PotassiumChannel(const PotassiumChannel& other) : Channel(other) {
    nGate = new NGate(0.32);
    gates.push_back(nGate);
}

double PotassiumChannel::current(double V) {
    double n = nGate->getP();
    return gIon * n * n * n * n * (V - EIon);
}

Channel* PotassiumChannel::clone() const {
    return new PotassiumChannel(*this);
}
