#pragma once
#include "InitialConditionGenerator.hpp"

class SolarSystemGenerator : public InitialConditionGenerator {
public:
    std::vector<Particle> generateInitialConditions() override;
};