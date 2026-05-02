#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <vector>
#include <algorithm>
#include "Synapse.h"

class NeuralNetwork {
protected:
    std::vector<Neuron*> neurons;
    std::vector<Synapse*> synapses;
    std::vector<double> eegSignal;
    double simTime = 0.0;

public:
    NeuralNetwork() {}

    NeuralNetwork(const NeuralNetwork& other) {
        std::vector<Neuron*> oldToNewMap(other.neurons.size());
        for (size_t i = 0; i < other.neurons.size(); i++) {
            neurons.push_back(new Neuron(*other.neurons[i]));
            oldToNewMap[i] = other.neurons[i];
        }
        for (auto synapse : other.synapses) {
            Neuron* oldPre = synapse->getPre();
            Neuron* oldPost = synapse->getPost();
            Neuron* newPre = nullptr;
            Neuron* newPost = nullptr;
            for (size_t i = 0; i < other.neurons.size(); i++) {
                if (oldToNewMap[i] == oldPre) newPre = neurons[i];
                if (oldToNewMap[i] == oldPost) newPost = neurons[i];
            }
            synapses.push_back(new Synapse(*synapse, newPre, newPost));
        }
    }

    NeuralNetwork& operator=(const NeuralNetwork& other) {
        NeuralNetwork temp(other);
        std::swap(neurons, temp.neurons);
        std::swap(synapses, temp.synapses);
        return *this;
    }

    void addNeuron(Neuron* neuron) { 
        neurons.push_back(neuron); 
    }

    void addSynapse(Synapse* synapse) { 
        synapses.push_back(synapse); 
    }

    void update(double dt) {
        for (auto synapse : synapses) { 
            synapse->transmit(simTime); 
        }
        for (auto neuron : neurons) { 
            neuron->update(dt); 
        }
        double popVoltage = getPopulationVoltage();
        eegSignal.push_back(popVoltage);
        simTime += dt;
    }

    double getPopulationVoltage() const {
        if (neurons.empty()) return 0.0;
        double sum = 0.0;
        for (auto neuron : neurons) { 
            sum += neuron->getVoltage(); 
        }
        return sum / neurons.size();
    }

    const std::vector<double>& getEEGSignal() const { 
        return eegSignal; 
    }

    void clearEEGSignal() { 
        eegSignal.clear(); 
        simTime = 0.0; 
    }

    ~NeuralNetwork() {
        for (auto neuron : neurons) { 
            delete neuron; 
        }
        for (auto synapse : synapses) { 
            delete synapse; 
        }
    }
};

#endif 
