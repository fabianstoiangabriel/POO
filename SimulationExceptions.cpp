#include "SimulationExceptions.h"

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
