#ifndef SYNAPSE_H
#define SYNAPSE_H

#include "Neuron.h"

class Synapse {
private:
    Neuron* pre;
    Neuron* post;
    double strength;
    double synapticDelay = 0.0;
    double releaseTime = -1.0;

public:
    Synapse(Neuron* pre, Neuron* post, double strength)
        : pre(pre), post(post), strength(strength) {}

    Synapse(const Synapse& other, Neuron* newPre, Neuron* newPost)
        : pre(newPre), post(newPost), strength(other.strength), 
          synapticDelay(other.synapticDelay) {}

    void transmit(double time = 0.0) {
        double V_pre = pre->getVoltage();
        if (V_pre > 0 && releaseTime < 0) {
            releaseTime = time + synapticDelay;
        }
        if (time >= releaseTime && releaseTime >= 0) {
            post->setExternalCurrent(strength);
            releaseTime = -1.0;
        }
    }

    Neuron* getPre() const { 
        return pre; 
    }

    Neuron* getPost() const { 
        return post; 
    }

    double getStrength() const { 
        return strength; 
    }

    void setSynapticDelay(double delay) { 
        synapticDelay = delay; 
    }
};

#endif 
