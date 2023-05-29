#include <random>
#include "RandomSystemGenerator.hpp"

RandomSystemGenerator::RandomSystemGenerator(int num_particles) : num_particles_(num_particles) {}

std::vector<Particle> RandomSystemGenerator::generateInitialConditions() {
    std::vector<Particle> particles {Particle(1.0)};
    
    // Generate random value for mass, distance and angle

    // If want to generate different random particles, uncomment the following two comments
    // std::random_device rd;
    // std::mt19937 gen(rd());
    
    std::mt19937 gen(42);
    std::uniform_real_distribution<> mass_distribution(1. / 6000000, 1. / 1000);
    std::uniform_real_distribution<> distance_distribution(0.4, 30.);
    std::uniform_real_distribution<> angle_distribution(0., 2 * M_PI);

    for (int i = 1; i < num_particles_; i++) {
        double m = mass_distribution(gen);
        double r = distance_distribution(gen);
        double theta = angle_distribution(gen);
        double x_x = r * std::sin(theta);
        double x_y = r * std::cos(theta);
        double x_z = 0.0;
        double v_x = -1 / std::sqrt(r) * std::cos(theta);
        double v_y = 1 / std::sqrt(r) * std::sin(theta);
        double v_z = 0.0;
        Particle p(m, Eigen::Vector3d(x_x, x_y, x_z), Eigen::Vector3d(v_x, v_y, v_z));
        particles.push_back(p);
    }

    return particles;
}