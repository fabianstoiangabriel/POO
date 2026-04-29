#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <vector>
#include <algorithm>
#include "Neuron.h"
#include "Synapse.h"

class NeuralNetwork {
protected:
    std::vector<Neuron*> neurons;
    std::vector<Synapse*> synapses;
    std::vector<double> eegSignal;
    double simTime;

public:
    NeuralNetwork();
    NeuralNetwork(const NeuralNetwork& other);
    NeuralNetwork& operator=(const NeuralNetwork& other);
    virtual ~NeuralNetwork();

    void addNeuron(Neuron* neuron);
    void addSynapse(Synapse* synapse);
    void update(double dt);
    double getPopulationVoltage() const;
    const std::vector<double>& getEEGSignal() const;
    void clearEEGSignal();
};

#endif // NEURAL_NETWORK_H
