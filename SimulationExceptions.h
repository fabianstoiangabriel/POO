#ifndef SIMULATION_EXCEPTIONS_H
#define SIMULATION_EXCEPTIONS_H

#include <stdexcept>

class SimulationException : public std::exception {
public:
    const char* what() const noexcept override;
};

class InvalidConductanceException : public SimulationException {
public:
    const char* what() const noexcept override;
};

class InvalidVoltageException : public SimulationException {
public:
    const char* what() const noexcept override;
};

class TimestepException : public SimulationException {
public:
    const char* what() const noexcept override;
};

#endif // SIMULATION_EXCEPTIONS_H
