#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include "Channel.h"
#include "SystemErrors.h"

class Neuron {
private:
    double V;
    double C_m = 1.0;
    double I_ext = 0.0;
    std::vector<Channel*> channels;
    std::vector<double> voltageTrace;
    static int neuronCount;

public:
    Neuron(double initialV) : V(initialV) {
        neuronCount++;
        if (initialV < -100 || initialV > 50) {
            throw InvalidVoltageException();
        }
        channels.push_back(new SodiumChannel(120.0, 50.0));
        channels.push_back(new PotassiumChannel(36.0, -77.0));
        channels.push_back(new leakChannel(0.3, -54.4));
        channels.push_back(new CaChannel(1.0, 120.0));
    }

    Neuron(const Neuron& other) : V(other.V), C_m(other.C_m), I_ext(other.I_ext) {
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

    static int getNeuronCount() { 
        return neuronCount; 
    }

    ~Neuron() {
        for (auto channel : channels) {
            delete channel;
        }
    }

    void addChannel(Channel* channel) { 
        channels.push_back(channel); 
    }

    void update(double dt) {
        if (dt > 0.1) 
            throw TimestepException();
        
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

    double getVoltage() const { 
        return V; 
    }

    void setExternalCurrent(double I) { 
        I_ext = I; 
    }

    const std::vector<double>& getVoltageTrace() const { 
        return voltageTrace; 
    }

    void clearVoltageTrace() { 
        voltageTrace.clear(); 
    }
};

int Neuron::neuronCount = 0;

#endif 
