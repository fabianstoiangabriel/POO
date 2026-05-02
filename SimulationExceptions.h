#ifndef SYSTEMERRORS_H
#define SYSTEMERRORS_H

#include <stdexcept>

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
public:    
    const char* what() const noexcept override {
        return "Voltage out of range!";
    }
};

class TimestepException : public SimulationException {
public:
    const char* what() const noexcept override {
        return "Timestep too large, simulation unstable!";
    }
};

#endif 
