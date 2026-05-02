#include <iostream>
#include <vector>
#include<cmath>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include "SystemErrors.h"
#include "Gate.h"
#include "Channel.h"
#include "Neuron.h"
#include "Synapse.h"
#include "NeuralNetwork.h"
#include "Amygdala.h"





int main() {
    try {
        Amygdala amygdala(100);
        amygdala.setFearState(true);
        std::cout << "Number of neurons in Amygdala: " << Neuron::getNeuronCount() << std::endl;
        for (int i = 0; i < 1000; i++) {
            amygdala.update(0.01);
        }
        const auto& eeg = amygdala.getEEGSignal();
        for (size_t i = 0; i < eeg.size(); i += 100) {
            std::cout << "Time: " << i * 0.01 << "s, EEG: " << eeg[i] << std::endl;
        }
    } catch (const SimulationException& e) {
        std::cerr << "Simulation error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
    return 0;
}
