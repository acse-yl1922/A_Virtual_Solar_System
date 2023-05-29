#include "SolarSystemGenerator.hpp"

std::vector<Particle> SolarSystemGenerator::generateInitialConditions() {
    
    // Refactor the existing Solar System initial conditions generator
    const std::vector<double> masses = {1.,1./6023600,1./408524,1./332946.038,1./3098710,1./1047.55,1./3499,1./22962,1./19352};
    const std::vector<double> distances = {0.0, 0.4, 0.7, 1, 1.5, 5.2, 9.5, 19.2, 30.1};

    std::vector<Particle> particles {Particle(1.0)};

    for (int i = 1; i < masses.size(); i++) {
        double m = masses[i];
        double theta = ((double)std::rand() / RAND_MAX) * 2 * M_PI;
        double r = distances[i];
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