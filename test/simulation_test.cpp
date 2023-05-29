#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "particle.hpp"

TEST_CASE("Masses & distances of the Solar System") {
    std::vector<Particle> particles = initial_condition_generator();
    const std::vector<double> masses = {1.,1./6023600,1./408524,1./332946.038,1./3098710,1./1047.55,1./3499,1./22962,1./19352};
    const std::vector<double> distances = {0.0, 0.4, 0.7, 1, 1.5, 5.2, 9.5, 19.2, 30.1};
    for (int i = 0; i < particles.size(); i++) {
        Particle p = particles[i];
        double actual_m = p.getMass();
        double actual_d = (particles[0].getPosition() - p.getPosition()).norm();
        double expected_m = masses[i];
        double expected_d = distances[i];
        REQUIRE(expected_m == actual_m);
        REQUIRE(std::abs(expected_d - actual_d) < 1e-6);
    }
}

TEST_CASE("A single body around the Sun") {
    Particle sun = Particle(1.0);

    Eigen::Vector3d posi = Eigen::Vector3d(0, 1, 0);
    Eigen::Vector3d velo = Eigen::Vector3d(-1, 0, 0);
    Particle earth = Particle(1./332946.038, posi, velo);

    std::vector<Particle> particles_start = {sun, earth};
    std::vector<Particle> particles_end = evolution_Solar_System(particles_start, 0.01, 1);

    Eigen::Vector3d expected_posi = Eigen::Vector3d(-0.01, 1, 0);
    Eigen::Vector3d actual_posi = particles_end[1].getPosition();
    REQUIRE(expected_posi.isApprox(actual_posi));
}

TEST_CASE("Position of Earth after a time of 2π") {
    double dt = 0.0001;
    int time_steps = int(2 * M_PI / dt);

    std::vector<Particle> particles_start = initial_condition_generator();
    std::vector<Particle> particles_end = evolution_Solar_System(particles_start, dt, time_steps);

    Eigen::Vector3d earth_posi_start = particles_start[3].getPosition();
    Eigen::Vector3d earth_posi_end = particles_end[3].getPosition();

    REQUIRE(earth_posi_start.isApprox(earth_posi_end, 0.01));
}