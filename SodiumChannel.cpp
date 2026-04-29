#include "SodiumChannel.h"

SodiumChannel::SodiumChannel(double gIon, double EIon) : Channel(gIon, EIon) {
    mGate = new MGate(0.05);
    hGate = new HGate(0.6);
    gates.push_back(mGate);
    gates.push_back(hGate);
}

SodiumChannel::SodiumChannel(const SodiumChannel& other) : Channel(other) {
    mGate = new MGate(0.05);
    hGate = new HGate(0.6);
    gates.push_back(mGate);
    gates.push_back(hGate);
}

double SodiumChannel::current(double V) {
    double m = mGate->getP();
    double h = hGate->getP();
    return gIon * m * m * m * h * (V - EIon);
}

Channel* SodiumChannel::clone() const {
    return new SodiumChannel(*this);
}
