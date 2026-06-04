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

class SimulationException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Simulation error!";
    }
};
class InvalidConductanceException : public SimulationException {
public:
    const char* what() const noexcept override {
        return "Conductance cannot be negative!";
    }
};
class InvalidVoltageException : public SimulationException {
public:    const char* what() const noexcept override {
        return "Voltage out of range!";
    }
};

class TimestepException : public SimulationException {
public:
    const char* what() const noexcept override {
        return "Timestep too large, simulation unstable!";
    }
};

class Gate
{private:
      virtual std::string gateType() const = 0;

protected:
double P;
    public:
    Gate(double initialP):P(initialP){}
    virtual ~Gate() = default;
    Gate(const Gate& other): P(other.P) {}
virtual double alpha(double V) = 0;
virtual double beta(double V) = 0;
void updD(double V, double dt)
{  double dP = alpha(V) * (1 - P) - beta(V) * P;
    P += dP * dt;
}
double steadyState(double V) { return alpha(V) / (alpha(V) + beta(V)); }

double timeConstant(double V) { return 1.0 / (alpha(V) + beta(V)); }
    double getP() const { return P; }

    void print(std::ostream& os) const {
    os << "Gate [P=" << P << ", type=" << gateType() << "]";
}
};

class MGate : public Gate
{private:
std::string gateType() const override { return "M (Na activation)";}
     public:
    MGate(double initialP): Gate(initialP){}
    double alpha(double V) override
    {return 0.1 * (V + 40) / (1 - exp(-(V + 40) / 10));}
    double beta(double V) override
    {return 4 * exp(-(V + 65) / 18);}
};


class HGate : public Gate

{ private:
    std::string gateType() const override { return "H (K activation)"; }
    public:
    HGate(double initialP): Gate(initialP){}
    double alpha(double V) override
    {return 0.07 * exp(-(V + 65) / 20);}
    double beta(double V) override
    {return 1 / (exp(-(V + 35) / 10) + 1);}
};


class NGate : public Gate

{private:
    std::string gateType() const override { return "N (K inactivation)"; }
     public:
    NGate(double initialP): Gate(initialP){}
    double alpha(double V) override
{return 0.01 * (V + 55) / (1 - exp(-(V + 55) / 10));}
    double beta(double V) override
    {return 0.125 * exp(-(V + 65) / 80);}
};


class CaMGate : public Gate {
private:
    std::string gateType() const override { return "CaM (Ca activation)"; }

public:
    CaMGate(double initialP): Gate(initialP){}
    double alpha(double V) override {
        return 0.055 * (V + 27) / (1 - exp(-(V + 27) / 3.8));
    }
    double beta(double V) override {
        return 0.94 * exp(-(V + 75) / 17);
    }
};

class CaHGate : public Gate {
private:
    std::string gateType() const override { return "CaH (Ca inactivation)"; }
public:
    CaHGate(double initialP): Gate(initialP){}
    double alpha(double V) override {
        return 0.000457 * exp(-(V + 13) / 50);
    }
    double beta(double V) override {
        return 0.0065 / (1 + exp(-(V + 15) / 28));
    }
};



class Channel{
    protected:
std::vector<Gate*> gates;
double gIon;
double EIon;
public:
Channel(double gIon, double EIon): gIon(gIon), EIon(EIon) {if (gIon < 0) {  throw InvalidConductanceException();  }}
virtual double current (double V) = 0;
void updCurrent(double V, double dt) {
    for (auto gate : gates)
        gate->updD(V, dt);
}
double gateOpenProbability(){
    double P = 1.0;
    for (auto gate : gates) { P *= gate->getP(); }
    return P;
}

double getGIon() const { return gIon; }
double getEIon() const { return EIon; }

virtual Channel* clone() const = 0;

virtual ~Channel() {
    for (auto gate : gates) { delete gate; }
}

protected:
Channel(const Channel& other): gIon(other.gIon), EIon(other.EIon) {}
};


class SodiumChannel : public Channel
{
private:
    MGate* mGate;
    HGate* hGate;
public:
    SodiumChannel(double gIon, double EIon): Channel(gIon, EIon) {
        mGate = new MGate(0.05);
        hGate = new HGate(0.6);
        gates.push_back(mGate);
        gates.push_back(hGate);
    }
    SodiumChannel(const SodiumChannel& other): Channel(other) {
        mGate = new MGate(0.05);
        hGate = new HGate(0.6);
        gates.push_back(mGate);
        gates.push_back(hGate);
    }
    double current(double V) override {
        double m = mGate->getP();
        double h = hGate->getP();
        return gIon * m * m * m * h * (V - EIon);
    }
    Channel* clone() const override {
    return new SodiumChannel(*this);
}
};


class PotassiumChannel : public Channel
{
private:
    NGate* nGate;
public:
    PotassiumChannel(double gIon, double EIon): Channel(gIon, EIon) {
        nGate = new NGate(0.32);
        gates.push_back(nGate);
    }
    PotassiumChannel(const PotassiumChannel& other): Channel(other) {
        nGate = new NGate(0.32);
        gates.push_back(nGate);
    }
    double current(double V) override {
        double n = nGate->getP();
        return gIon * n * n * n * n * (V - EIon);
    }
    Channel* clone() const override {
    return new PotassiumChannel(*this);
}
};

class leakChannel : public Channel
{public:
    leakChannel(double gIon, double EIon): Channel(gIon, EIon) {
    }
    leakChannel(const leakChannel& other): Channel(other) {
    }
    double current(double V) override {
        return gIon * (V - EIon);
    }
    Channel* clone() const override {
    return new leakChannel(*this);}
};

class CaChannel : public Channel {
private:
    CaMGate* mGate;
    CaHGate* hGate;
public:
    CaChannel(double gIon, double EIon): Channel(gIon, EIon) {
        mGate = new CaMGate(0.0);
        hGate = new CaHGate(1.0);
        gates.push_back(mGate);
        gates.push_back(hGate);
    }
    CaChannel(const CaChannel& other): Channel(other) {
        mGate = new CaMGate(0.0);
        hGate = new CaHGate(1.0);
        gates.push_back(mGate);
        gates.push_back(hGate);
    }
    double current(double V) override {
        double m = mGate->getP();
        double h = hGate->getP();
        return gIon * m * m * h * (V - EIon);
    }
    Channel* clone() const override {
        return new CaChannel(*this);
    }
};

class Neuron {
    private:
    double V;
    double C_m = 1.0;
    double I_ext = 0.0;
    std::vector<Channel*> channels;
    std::vector<double> voltageTrace;
    static int neuronCount;
    public:
    Neuron(double initialV): V(initialV){
        neuronCount++;
        if (initialV < -100 || initialV > 50) { throw InvalidVoltageException(); }
        channels.push_back(new SodiumChannel(120.0, 50.0));
        channels.push_back(new PotassiumChannel(36.0, -77.0));
        channels.push_back(new leakChannel(0.3, -54.4));
        channels.push_back(new CaChannel(1.0, 120.0));
    }

    Neuron(const Neuron& other): V(other.V), C_m(other.C_m), I_ext(other.I_ext) {
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
    static int getNeuronCount() { return neuronCount; }

    ~Neuron() {
        for (auto channel : channels) { delete channel; }
    }

    void addChannel(Channel* channel) { channels.push_back(channel); }
    void update(double dt) {
        if (dt > 0.1) throw TimestepException();
        for (auto channel : channels) {
            channel->updCurrent(V, dt);
        }
        double I_ion = 0.0;
        for (auto channel : channels) { I_ion += channel->current(V); }
        double dV = (I_ext - I_ion) / C_m;
        V += dV * dt;
        voltageTrace.push_back(V);
    }
    double getVoltage() const { return V; }
    void setExternalCurrent(double I) { I_ext = I; }
    const std::vector<double>& getVoltageTrace() const { return voltageTrace; }
    void clearVoltageTrace() { voltageTrace.clear(); }
};

int Neuron::neuronCount = 0;



class Synapse
{
    private:
    Neuron* pre;
    Neuron* post;
    double strength;
    double synapticDelay = 0.0;
    double releaseTime = -1.0;
    public:
    Synapse(Neuron* pre, Neuron* post, double strength): pre(pre), post(post), strength(strength) {}

    Synapse(const Synapse& other, Neuron* newPre, Neuron* newPost):
        pre(newPre), post(newPost), strength(other.strength), synapticDelay(other.synapticDelay) {}

    void transmit(double time = 0.0) {
        double V_pre = pre->getVoltage();
        if (V_pre > 0 && releaseTime < 0) {
            releaseTime = time + synapticDelay;
        }
        if (time >= releaseTime && releaseTime >= 0) {
            post->setExternalCurrent(strength);
            releaseTime = -1.0;
        }
    }

    Neuron* getPre() const { return pre; }
    Neuron* getPost() const { return post; }
    double getStrength() const { return strength; }
    void setSynapticDelay(double delay) { synapticDelay = delay; }
};


class NeuralNetwork
{
    protected:
    std::vector<Neuron*> neurons;
    std::vector<Synapse*> synapses;
    std::vector<double> eegSignal;
    double simTime = 0.0;
    public:
    NeuralNetwork(){}
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
    void addNeuron(Neuron* neuron) { neurons.push_back(neuron); }
    void addSynapse(Synapse* synapse) { synapses.push_back(synapse); }
    void update(double dt) {
        for (auto synapse : synapses) { synapse->transmit(simTime); }
        for (auto neuron : neurons) { neuron->update(dt); }
        double popVoltage = getPopulationVoltage();
        eegSignal.push_back(popVoltage);
        simTime += dt;
    }
    double getPopulationVoltage() const {
        if (neurons.empty()) return 0.0;
        double sum = 0.0;
        for (auto neuron : neurons) { sum += neuron->getVoltage(); }
        return sum / neurons.size();
    }
    const std::vector<double>& getEEGSignal() const { return eegSignal; }
    void clearEEGSignal() { eegSignal.clear(); simTime = 0.0; }
    ~NeuralNetwork() {
        for (auto neuron : neurons) { delete neuron; }
        for (auto synapse : synapses) { delete synapse; }
    }
};


class Amygdala : public NeuralNetwork
{
    private:
    int excitatory ;
    int inhibitory ;
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
