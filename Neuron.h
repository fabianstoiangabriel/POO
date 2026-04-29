#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include "Channel.h"
#include "SimulationExceptions.h"

class Neuron {
private:
    double V;
    double C_m;
    double I_ext;
    std::vector<Channel*> channels;
    std::vector<double> voltageTrace;
    static int neuronCount;

public:
    Neuron(double initialV);
    Neuron(const Neuron& other);
    ~Neuron();

    static int getNeuronCount();
    void addChannel(Channel* channel);
    void update(double dt);
    double getVoltage() const;
    void setExternalCurrent(double I);
    const std::vector<double>& getVoltageTrace() const;
    void clearVoltageTrace();
};

#endif // NEURON_H
