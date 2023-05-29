#pragma once
#include <vector>
#include "particle.hpp"

class InitialConditionGenerator {
public:
    // initial conditions for the simulation
    virtual std::vector<Particle> generateInitialConditions() = 0;
};