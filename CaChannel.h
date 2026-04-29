#ifndef CA_CHANNEL_H
#define CA_CHANNEL_H

#include "Channel.h"
#include "CaMGate.h"
#include "CaHGate.h"

class CaChannel : public Channel {
private:
    CaMGate* mGate;
    CaHGate* hGate;

public:
    CaChannel(double gIon, double EIon);
    CaChannel(const CaChannel& other);
    double current(double V) override;
    Channel* clone() const override;
};

#endif // CA_CHANNEL_H
