#ifndef PARTICLE_H
#define PARTICLE_H

#include <Eigen/Core>

class Particle {
    public:
        Particle(double in_mass, const Eigen::Vector3d& position = Eigen::Vector3d::Zero(), const Eigen::Vector3d& velocity = Eigen::Vector3d::Zero(), const Eigen::Vector3d& acceleration = Eigen::Vector3d::Zero());
        Eigen::Vector3d getPosition() const;
        Eigen::Vector3d getVelocity() const;
        Eigen::Vector3d getAcceleration() const;
        double getMass() const;
        void update(double dt);

    private:
        Eigen::Vector3d position_;
        Eigen::Vector3d velocity_;
        Eigen::Vector3d acceleration_;
        double mass;
};

Eigen::Vector3d calcAcceleration(const Particle& i, const Particle& j, double epsilon = 0.0);
Eigen::Vector3d calcTotalAcceleration(const Particle& particle, const std::vector<Particle>& particles, double epsilon = 0.0);
std::vector<Particle> initial_condition_generator();
std::vector<Particle> evolution_Solar_System(std::vector<Particle> particles, double dt, int time_steps, double epsilon = 0.0);
double getEnergy(Particle particle, std::vector<Particle> particles);
double totalEnergy(std::vector<Particle> particles);

#endif