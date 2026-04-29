#ifndef POTASSIUM_CHANNEL_H
#define POTASSIUM_CHANNEL_H

#include "Channel.h"
#include "NGate.h"

class PotassiumChannel : public Channel {
private:
    NGate* nGate;

public:
    PotassiumChannel(double gIon, double EIon);
    PotassiumChannel(const PotassiumChannel& other);
    double current(double V) override;
    Channel* clone() const override;
};

#endif // POTASSIUM_CHANNEL_H
