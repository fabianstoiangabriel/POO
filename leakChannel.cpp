#include "leakChannel.h"

leakChannel::leakChannel(double gIon, double EIon) : Channel(gIon, EIon) {}

leakChannel::leakChannel(const leakChannel& other) : Channel(other) {}

double leakChannel::current(double V) {
    return gIon * (V - EIon);
}

Channel* leakChannel::clone() const {
    return new leakChannel(*this);
}
