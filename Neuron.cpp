#include "Neuron.h"
#include "SodiumChannel.h"
#include "PotassiumChannel.h"
#include "leakChannel.h"
#include "CaChannel.h"

int Neuron::neuronCount = 0;

Neuron::Neuron(double initialV) : V(initialV) {
    neuronCount++;
    if (initialV < -100 || initialV > 50) {
        throw InvalidVoltageException();
    }
    channels.push_back(new SodiumChannel(120.0, 50.0));
    channels.push_back(new PotassiumChannel(36.0, -77.0));
    channels.push_back(new leakChannel(0.3, -54.4));
    channels.push_back(new CaChannel(1.0, 120.0));
}

Neuron::Neuron(const Neuron& other) : V(other.V), C_m(other.C_m), I_ext(other.I_ext) {
    for (auto channel : other.channels) {
        SodiumChannel* sodium = dynamic_cast<SodiumChannel*>(channel);
        PotassiumChannel* potassium = dynamic_cast<PotassiumChannel*>(channel);
        leakChannel* leak = dynamic_cast<leakChannel*>(channel);
        CaChannel* ca = dynamic_cast<CaChannel*>(channel);

        if (sodium) {
            channels.push_back(new SodiumChannel(*sodium));
        } else if (potassium) {
            channels.push_back(new PotassiumChannel(*potassium));
        } else if (leak) {
            channels.push_back(new leakChannel(*leak));
        } else if (ca) {
            channels.push_back(new CaChannel(*ca));
        }
    }
}

Neuron::~Neuron() {
    for (auto channel : channels) {
        delete channel;
    }
}

int Neuron::getNeuronCount() {
    return neuronCount;
}

void Neuron::addChannel(Channel* channel) {
    channels.push_back(channel);
}

void Neuron::update(double dt) {
    if (dt > 0.1) throw TimestepException();
    for (auto channel : channels) {
        channel->updCurrent(V, dt);
    }
    double I_ion = 0.0;
    for (auto channel : channels) {
        I_ion += channel->current(V);
    }
    double dV = (I_ext - I_ion) / C_m;
    V += dV * dt;
    voltageTrace.push_back(V);
}

double Neuron::getVoltage() const {
    return V;
}

void Neuron::setExternalCurrent(double I) {
    I_ext = I;
}

const std::vector<double>& Neuron::getVoltageTrace() const {
    return voltageTrace;
}

void Neuron::clearVoltageTrace() {
    voltageTrace.clear();
}
