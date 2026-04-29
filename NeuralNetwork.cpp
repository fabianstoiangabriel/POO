#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork() : simTime(0.0) {}

NeuralNetwork::NeuralNetwork(const NeuralNetwork& other) : simTime(0.0) {
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

NeuralNetwork& NeuralNetwork::operator=(const NeuralNetwork& other) {
    NeuralNetwork temp(other);
    std::swap(neurons, temp.neurons);
    std::swap(synapses, temp.synapses);
    return *this;
}

NeuralNetwork::~NeuralNetwork() {
    for (auto neuron : neurons) {
        delete neuron;
    }
    for (auto synapse : synapses) {
        delete synapse;
    }
}

void NeuralNetwork::addNeuron(Neuron* neuron) {
    neurons.push_back(neuron);
}

void NeuralNetwork::addSynapse(Synapse* synapse) {
    synapses.push_back(synapse);
}

void NeuralNetwork::update(double dt) {
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

double NeuralNetwork::getPopulationVoltage() const {
    if (neurons.empty()) return 0.0;
    double sum = 0.0;
    for (auto neuron : neurons) {
        sum += neuron->getVoltage();
    }
    return sum / neurons.size();
}

const std::vector<double>& NeuralNetwork::getEEGSignal() const {
    return eegSignal;
}

void NeuralNetwork::clearEEGSignal() {
    eegSignal.clear();
    simTime = 0.0;
}
