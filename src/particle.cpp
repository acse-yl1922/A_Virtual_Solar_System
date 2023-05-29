#include "particle.hpp"
#include <iostream>
#include <Eigen/Core>
#include <cmath>
#include <vector>
#include <random>

// Particle class constructor
Particle::Particle(double in_mass, const Eigen::Vector3d& position, const Eigen::Vector3d& velocity, const Eigen::Vector3d& acceleration) :
    mass{in_mass}, position_(position), velocity_(velocity), acceleration_(acceleration)
    {}

// Functions for Particle class
Eigen::Vector3d Particle::getPosition() const {
    return position_;
}

Eigen::Vector3d Particle::getVelocity() const {
    return velocity_;
}

Eigen::Vector3d Particle::getAcceleration() const {
    return acceleration_;
}

double Particle::getMass() const {
    return mass;
}

void Particle::update(double dt) {
    position_ += dt * velocity_;
    velocity_ += dt * acceleration_;
}

// Function to calculate acceleration between two particles
Eigen::Vector3d calcAcceleration(const Particle& i, const Particle& j, double epsilon) {
    Eigen::Vector3d r = j.getPosition() - i.getPosition();
    double d = r.norm();
    double m = j.getMass();
    Eigen::Vector3d acc = m * r / std::pow(d * d + epsilon * epsilon, 1.5);
    return acc;
}

// Function to calculate total acceleration on a particle due to all other particles in the system
Eigen::Vector3d calcTotalAcceleration(const Particle& particle, const std::vector<Particle>& particles, double epsilon) {
    Eigen::Vector3d total_acc = Eigen::Vector3d::Zero();

    for (int i = 0; i < particles.size(); i++) {
        // Check that the particle does not interact with itself
        if ((particle.getPosition().isApprox(particles[i].getPosition())) == 0) {
            Eigen::Vector3d acc = calcAcceleration(particle, particles[i], epsilon);
            total_acc += acc;
        }
    }
    return total_acc;
}

// Function to generate initial conditions for the Solar system simulation
std::vector<Particle> initial_condition_generator() {
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

// Function to generate evolution conditions for the system simulation
std::vector<Particle> evolution_Solar_System(std::vector<Particle> particles, double dt, int time_steps, double epsilon) {
    for (int i = 0; i < time_steps; i++) {
        std::vector<Eigen::Vector3d> accelerations(particles.size());

        #pragma omp parallel for
        for (int j = 0; j < accelerations.size(); j++) {
            Particle& p = particles[j];
            Eigen::Vector3d acc = calcTotalAcceleration(p, particles, epsilon);
            accelerations[j] = acc;
        }

        #pragma omp parallel for
        for (int k = 0; k < accelerations.size(); k++) {
            Particle& p = particles[k];
            p = Particle(p.getMass(), p.getPosition(), p.getVelocity(), accelerations[k]);
            p.update(dt);
        }
    }
    return particles;
}

// Function to calculate the total energy of a particle
double getEnergy(Particle particle, std::vector<Particle> particles) {
    double kinetic_energy = 0.5 * particle.getMass() * particle.getVelocity().norm() * particle.getVelocity().norm();
    double potential_energy = 0.0;
    for (int i = 0; i < particles.size(); i++) {
        if (particle.getPosition().isApprox(particles[i].getPosition()) == 0) {
            potential_energy += -0.5 * particle.getMass() * particles[i].getMass() / (particle.getPosition() - particles[i].getPosition()).norm();
        }
    }
    double total_energy = kinetic_energy + potential_energy;
    return total_energy;
}

// Function to calculate the total energy of a list of particles
double totalEnergy(std::vector<Particle> particles) {
    double total_energy = 0.0;
    for (int i = 0; i < particles.size(); i++) {
        double energy = getEnergy(particles[i], particles);
        total_energy += energy;
    }
    return total_energy;
}