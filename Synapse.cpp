#include "Synapse.h"

Synapse::Synapse(Neuron* pre, Neuron* post, double strength)
    : pre(pre), post(post), strength(strength) {}

Synapse::Synapse(const Synapse& other, Neuron* newPre, Neuron* newPost)
    : pre(newPre), post(newPost), strength(other.strength), synapticDelay(other.synapticDelay) {}

void Synapse::transmit(double time) {
    double V_pre = pre->getVoltage();
    if (V_pre > 0 && releaseTime < 0) {
        releaseTime = time + synapticDelay;
    }
    if (time >= releaseTime && releaseTime >= 0) {
        post->setExternalCurrent(strength);
        releaseTime = -1.0;
    }
}

Neuron* Synapse::getPre() const {
    return pre;
}

Neuron* Synapse::getPost() const {
    return post;
}

double Synapse::getStrength() const {
    return strength;
}

void Synapse::setSynapticDelay(double delay) {
    synapticDelay = delay;
}
