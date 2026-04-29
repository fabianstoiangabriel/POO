#ifndef SODIUM_CHANNEL_H
#define SODIUM_CHANNEL_H

#include "Channel.h"
#include "MGate.h"
#include "HGate.h"

class SodiumChannel : public Channel {
private:
    MGate* mGate;
    HGate* hGate;

public:
    SodiumChannel(double gIon, double EIon);
    SodiumChannel(const SodiumChannel& other);
    double current(double V) override;
    Channel* clone() const override;
};

#endif // SODIUM_CHANNEL_H
