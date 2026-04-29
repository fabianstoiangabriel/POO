// ===== HEADER INCLUDES =====
#include "SimulationExceptions.h"
#include "Gate.h"
#include "MGate.h"
#include "HGate.h"
#include "NGate.h"
#include "CaMGate.h"
#include "CaHGate.h"
#include "Channel.h"
#include "SodiumChannel.h"
#include "PotassiumChannel.h"
#include "leakChannel.h"
#include "CaChannel.h"
#include "Neuron.h"
#include "Synapse.h"
#include "NeuralNetwork.h"
#include "Amygdala.h"

#include <iostream>
#include <stdexcept>
#include <cmath>
#include <algorithm>

// ===== EXCEPTION IMPLEMENTATIONS =====
const char* SimulationException::what() const noexcept {
    return "Simulation error!";
}

const char* InvalidConductanceException::what() const noexcept {
    return "Conductance cannot be negative!";
}

const char* InvalidVoltageException::what() const noexcept {
    return "Voltage out of range!";
}

const char* TimestepException::what() const noexcept {
    return "Timestep too large, simulation unstable!";
}

// ===== GATE IMPLEMENTATIONS =====
Gate::Gate(double initialP) : P(initialP) {}

Gate::Gate(const Gate& other) : P(other.P) {}

void Gate::updD(double V, double dt) {
    double dP = alpha(V) * (1 - P) - beta(V) * P;
    P += dP * dt;
}

double Gate::steadyState(double V) {
    double sum = alpha(V) + beta(V);
    if (fabs(sum) < 1e-10) return 0.5;
    return alpha(V) / sum;
}

double Gate::timeConstant(double V) {
    double sum = alpha(V) + beta(V);
    if (fabs(sum) < 1e-10) return 1.0;
    return 1.0 / sum;
}

double Gate::getP() const {
    return P;
}

void Gate::print(std::ostream& os) const {
    os << "Gate [P=" << P << ", type=" << gateType() << "]";
}

// ===== MGATE IMPLEMENTATIONS =====
std::string MGate::gateType() const {
    return "M (Na activation)";
}

MGate::MGate(double initialP) : Gate(initialP) {}

double MGate::alpha(double V) {
    double denom = 1 - exp(-(V + 40) / 10);
    if (fabs(denom) < 1e-6) return -0.4;
    return 0.1 * (V + 40) / denom;
}

double MGate::beta(double V) {
    return 4 * exp(-(V + 65) / 18);
}

// ===== HGATE IMPLEMENTATIONS =====
std::string HGate::gateType() const {
    return "H (Na inactivation)";
}

HGate::HGate(double initialP) : Gate(initialP) {}

double HGate::alpha(double V) {
    return 0.07 * exp(-(V + 65) / 20);
}

double HGate::beta(double V) {
    return 1 / (exp(-(V + 35) / 10) + 1);
}

// ===== NGATE IMPLEMENTATIONS =====
std::string NGate::gateType() const {
    return "N (K activation)";
}

NGate::NGate(double initialP) : Gate(initialP) {}
double denom = 1 - exp(-(V + 55) / 10);
    if (fabs(denom) < 1e-6) return -0.1;
    return 0.01 * (V + 55) / denom
double NGate::alpha(double V) {
    return 0.01 * (V + 55) / (1 - exp(-(V + 55) / 10));
}

double NGate::beta(double V) {
    return 0.125 * exp(-(V + 65) / 80);
}

// ===== CAMGATE IMPLEMENTATIONS =====
std::string CaMGate::gateType() const {
    return "CaM (Ca activation)";
}

CaMGate::CaMGate(double initialP) : Gate(initialP) {}
double denom = 1 - exp(-(V + 27) / 3.8);
    if (fabs(denom) < 1e-6) return -0.2090;
    return 0.055 * (V + 27) / denom
double CaMGate::alpha(double V) {
    return 0.055 * (V + 27) / (1 - exp(-(V + 27) / 3.8));
}

double CaMGate::beta(double V) {
    return 0.94 * exp(-(V + 75) / 17);
}

// ===== CAHGATE IMPLEMENTATIONS =====
std::string CaHGate::gateType() const {
    return "CaH (Ca inactivation)";
}

CaHGate::CaHGate(double initialP) : Gate(initialP) {}

double CaHGate::alpha(double V) {
    return 0.000457 * exp(-(V + 13) / 50);
}

double CaHGate::beta(double V) {
    return 0.0065 / (1 + exp(-(V + 15) / 28));
}

// ===== CHANNEL IMPLEMENTATIONS =====
Channel::Channel(double gIon, double EIon) : gIon(gIon), EIon(EIon) {
    if (gIon < 0) {
        throw InvalidConductanceException();
    }
}

Channel::~Channel() {
    for (auto gate : gates) {
        delete gate;
    }
}

Channel::Channel(const Channel& other) : gIon(other.gIon), EIon(other.EIon) {}

void Channel::updCurrent(double V, double dt) {
    for (auto gate : gates)
        gate->updD(V, dt);
}

double Channel::gateOpenProbability() {
    double P = 1.0;
    for (auto gate : gates) {
        P *= gate->getP();
    }
    return P;
}

double Channel::getGIon() const {
    return gIon;
}

double Channel::getEIon() const {
    return EIon;
}

// ===== SODIUM CHANNEL IMPLEMENTATIONS =====
SodiumChannel::SodiumChannel(double gIon, double EIon) : Channel(gIon, EIon) {
    mGate = new MGate(0.05);
    hGate = new HGate(0.6);
    gates.push_back(mGate);
    gates.push_back(hGate);
}

SodiumChannel::SodiumChannel(const SodiumChannel& other) : Channel(other) {
    mGate = new MGate(0.05);
    hGate = new HGate(0.6);
    gates.push_back(mGate);
    gates.push_back(hGate);
}

double SodiumChannel::current(double V) {
    double m = mGate->getP();
    double h = hGate->getP();
    return gIon * m * m * m * h * (V - EIon);
}

Channel* SodiumChannel::clone() const {
    return new SodiumChannel(*this);
}

// ===== POTASSIUM CHANNEL IMPLEMENTATIONS =====
PotassiumChannel::PotassiumChannel(double gIon, double EIon) : Channel(gIon, EIon) {
    nGate = new NGate(0.32);
    gates.push_back(nGate);
}

PotassiumChannel::PotassiumChannel(const PotassiumChannel& other) : Channel(other) {
    nGate = new NGate(0.32);
    gates.push_back(nGate);
}

double PotassiumChannel::current(double V) {
    double n = nGate->getP();
    return gIon * n * n * n * n * (V - EIon);
}

Channel* PotassiumChannel::clone() const {
    return new PotassiumChannel(*this);
}

// ===== LEAK CHANNEL IMPLEMENTATIONS =====
leakChannel::leakChannel(double gIon, double EIon) : Channel(gIon, EIon) {}

leakChannel::leakChannel(const leakChannel& other) : Channel(other) {}

double leakChannel::current(double V) {
    return gIon * (V - EIon);
}

Channel* leakChannel::clone() const {
    return new leakChannel(*this);
}

// ===== CA CHANNEL IMPLEMENTATIONS =====
CaChannel::CaChannel(double gIon, double EIon) : Channel(gIon, EIon) {
    mGate = new CaMGate(0.0);
    hGate = new CaHGate(1.0);
    gates.push_back(mGate);
    gates.push_back(hGate);
}

CaChannel::CaChannel(const CaChannel& other) : Channel(other) {
    mGate = new CaMGate(0.0);
    hGate = new CaHGate(1.0);
    gates.push_back(mGate);
    gates.push_back(hGate);
}

double CaChannel::current(double V) {
    double m = mGate->getP();
    double h = hGate->getP();
    return gIon * m * m * h * (V - EIon);
}

Channel* CaChannel::clone() const {
    return new CaChannel(*this);
}

// ===== NEURON IMPLEMENTATIONS =====
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

// ===== SYNAPSE IMPLEMENTATIONS =====
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

// ===== NEURAL NETWORK IMPLEMENTATIONS =====
NeuralNetwork::NeuralNetwork() : simTime(0.0) {}

NeuralNetwork::NeuralNetwork(const NeuralNetwork& other) : simTime(0.0) {
    std::unordered_map<Neuron*, Neuron*> neuronMap;
    for (size_t i = 0; i < other.neurons.size(); i++) {
        Neuron* newN = new Neuron(*other.neurons[i]);
        neurons.push_back(newN);
        neuronMap[other.neurons[i]] = newN;
    }
    for (auto synapse : other.synapses) {
        synapses.push_back(new Synapse(
            *synapse,
            neuronMap[synapse->getPre()],
            neuronMap[synapse->getPost()]
        ));
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

// ===== AMYGDALA IMPLEMENTATIONS =====
Amygdala::Amygdala(int numNeurons) : NeuralNetwork() {
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

void Amygdala::setFearState(bool fear) {
    for (auto neuron : neurons)
        neuron->setExternalCurrent(fear ? 15.0 : 2.0);
}


// ===== MAIN FUNCTION =====
int main() {
    try {
        Amygdala amygdala(100);
        amygdala.setFearState(true);
        std::cout << "Number of neurons in Amygdala: " << Neuron::getNeuronCount() << std::endl;
        amygdala.printFirstNeuron(std::cout);
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
