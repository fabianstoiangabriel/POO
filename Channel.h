#ifndef CHANNEL_H
#define CHANNEL_H

#include <vector>
#include "Gate.h"
#include "SystemErrors.h"

class Channel {
protected:
    std::vector<Gate*> gates;
    double gIon;
    double EIon;

public:
    Channel(double gIon, double EIon) : gIon(gIon), EIon(EIon) {
        if (gIon < 0) {
            throw InvalidConductanceException();
        }
    }

    virtual double current(double V) = 0;

    void updCurrent(double V, double dt) {
        for (auto gate : gates)
            gate->updD(V, dt);
    }

    double gateOpenProbability() {
        double P = 1.0;
        for (auto gate : gates) {
            P *= gate->getP();
        }
        return P;
    }

    double getGIon() const { return gIon; }
    double getEIon() const { return EIon; }

    virtual Channel* clone() const = 0;

    virtual ~Channel() {
        for (auto gate : gates) {
            delete gate;
        }
    }

protected:
    Channel(const Channel& other) : gIon(other.gIon), EIon(other.EIon) {}
};

class SodiumChannel : public Channel {
private:
    MGate* mGate;
    HGate* hGate;

public:
    SodiumChannel(double gIon, double EIon) : Channel(gIon, EIon) {
        mGate = new MGate(0.05);
        hGate = new HGate(0.6);
        gates.push_back(mGate);
        gates.push_back(hGate);
    }

    SodiumChannel(const SodiumChannel& other) : Channel(other) {
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

class PotassiumChannel : public Channel {
private:
    NGate* nGate;

public:
    PotassiumChannel(double gIon, double EIon) : Channel(gIon, EIon) {
        nGate = new NGate(0.32);
        gates.push_back(nGate);
    }

    PotassiumChannel(const PotassiumChannel& other) : Channel(other) {
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

class leakChannel : public Channel {
public:
    leakChannel(double gIon, double EIon) : Channel(gIon, EIon) {
    }

    leakChannel(const leakChannel& other) : Channel(other) {
    }

    double current(double V) override {
        return gIon * (V - EIon);
    }

    Channel* clone() const override {
        return new leakChannel(*this);
    }
};

