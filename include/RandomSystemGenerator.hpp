#pragma once
#include "InitialConditionGenerator.hpp"

class RandomSystemGenerator : public InitialConditionGenerator {
public:
    std::vector<Particle> generateInitialConditions() override;
    explicit RandomSystemGenerator(int num_particles);
    
private:
    int num_particles_;
};