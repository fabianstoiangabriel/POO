#ifndef AMYGDALA_H
#define AMYGDALA_H

#include "NeuralNetwork.h"

class Amygdala : public NeuralNetwork {
private:
    int excitatory;
    int inhibitory;

public:
    Amygdala(int numNeurons) : NeuralNetwork() {
        excitatory = numNeurons * 0.8;
        inhibitory = numNeurons * 0.2;

        std::vector<Neuron*> excNeurons;
        std::vector<Neuron*> inhNeurons;

        for (int i = 0; i < excitatory; i++) {
            Neuron* n = new Neuron(-65.0);
            n->setExternalCurrent(10.0);
            addNeuron(n);
            excNeurons.push_back(n);
        }

        for (int i = 0; i < inhibitory; i++) {
            Neuron* n = new Neuron(-65.0);
            addNeuron(n);
            inhNeurons.push_back(n);
        }

        for (auto pre : excNeurons)
            for (auto post : excNeurons)
                if (pre != post)
                    addSynapse(new Synapse(pre, post, 0.5));

        for (auto pre : inhNeurons)
            for (auto post : excNeurons)
                addSynapse(new Synapse(pre, post, -0.5));
    }

    void setFearState(bool fear) {
        for (auto neuron : neurons)
            neuron->setExternalCurrent(fear ? 15.0 : 2.0);
    }
};

#endif 
