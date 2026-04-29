#ifndef AMYGDALA_H
#define AMYGDALA_H

#include "NeuralNetwork.h"

class Amygdala : public NeuralNetwork {
private:
    int excitatory;
    int inhibitory;

public:
    Amygdala(int numNeurons);
    void setFearState(bool fear);
};

#endif // AMYGDALA_H
