#ifndef LEAK_CHANNEL_H
#define LEAK_CHANNEL_H

#include "Channel.h"

class leakChannel : public Channel {
public:
    leakChannel(double gIon, double EIon);
    leakChannel(const leakChannel& other);
    double current(double V) override;
    Channel* clone() const override;
};

#endif // LEAK_CHANNEL_H
