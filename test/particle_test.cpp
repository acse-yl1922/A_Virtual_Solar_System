#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "particle.hpp"

TEST_CASE("Particle moves with zero acceleration") {
    Eigen::Vector3d init_pos{1.0, 2.0, 3.0};
    Eigen::Vector3d init_vel{3.0, 2.0, 1.0};
    Particle p = Particle(1.0, init_pos, init_vel, Eigen::Vector3d::Zero());
    double dt = 0.01;

    for (int i = 1; i < 10; i++) {
        p.update(dt);
        Eigen::Vector3d expected_pos = init_pos + i * dt * init_vel;
        REQUIRE(p.getPosition().isApprox(expected_pos, 0.001));
    }
}

TEST_CASE("Particle moves with constant acceleration") {
    Eigen::Vector3d init_pos{1.0, 2.0, 3.0};
    Eigen::Vector3d init_vel{3.0, 2.0, 1.0};
    Eigen::Vector3d const const_acc{0.4, 0.5, 0.6};
    Particle p = Particle(1.0, init_pos, init_vel, const_acc);
    double dt = 0.01;

    for (int i = 1; i < 10; i++) {
        p.update(dt);
        Eigen::Vector3d expected_pos = init_pos + i * dt * init_vel + 0.5 * i * i * dt * dt * const_acc;
        REQUIRE(p.getPosition().isApprox(expected_pos, 0.001));
    }
}

TEST_CASE("Particle orbits in a circle around the origin") {
    Eigen::Vector3d init_pos{1, 0, 0};
    Eigen::Vector3d init_vel{0, 1, 0};
    Eigen::Vector3d acc{-1, 0, 0};
    Particle p = Particle(1.0, init_pos, init_vel, acc);
    double dt = 0.001;
    double t = 2 * M_PI;
    
    for (int i = 1; i <= t / dt; i++) {
        p.update(dt);
        p = Particle(1.0, p.getPosition(), p.getVelocity(), -p.getPosition());
        Eigen::Vector3d expected_pos = p.getPosition() + dt * init_vel + 0.5 * dt * dt * (-p.getPosition());
        REQUIRE(p.getPosition().isApprox(expected_pos, 0.001));
    }

    REQUIRE(p.getPosition().isApprox(init_pos, 1));
}