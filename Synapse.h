#ifndef SYNAPSE_H
#define SYNAPSE_H

#include "Neuron.h"

class Synapse {
private:
    Neuron* pre;
    Neuron* post;
    double strength;
    double synapticDelay;
    double releaseTime;

public:
    Synapse(Neuron* pre, Neuron* post, double strength);
    Synapse(const Synapse& other, Neuron* newPre, Neuron* newPost);

    void transmit(double time);
    Neuron* getPre() const;
    Neuron* getPost() const;
    double getStrength() const;
    void setSynapticDelay(double delay);
};

#endif // SYNAPSE_H
