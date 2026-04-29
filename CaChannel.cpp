#include "CaChannel.h"

CaChannel::CaChannel(double gIon, double EIon) : Channel(gIon, EIon) {
    mGate = new CaMGate(0.0);
    hGate = new CaHGate(1.0);
    gates.push_back(mGate);
    gates.push_back(hGate);
}

CaChannel::CaChannel(const CaChannel& other) : Channel(other) {
    mGate = new CaMGate(0.0);
    hGate = new CaHGate(1.0);
    gates.push_back(mGate);
    gates.push_back(hGate);
}

double CaChannel::current(double V) {
    double m = mGate->getP();
    double h = hGate->getP();
    return gIon * m * m * h * (V - EIon);
}

Channel* CaChannel::clone() const {
    return new CaChannel(*this);
}
