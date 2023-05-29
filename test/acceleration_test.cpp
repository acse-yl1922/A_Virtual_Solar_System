#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "particle.hpp"


TEST_CASE("Gravitational force between two particles") {
    Particle p1(1.0, Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(0.0, 0.0, 0.0));
    Particle p2(1.0, Eigen::Vector3d(1.0, 1.0, 1.0), Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(0.0, 0.0, 0.0));
    Particle p3(1.0, Eigen::Vector3d(-1.0, -2.0, -3.0), Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(0.0, 0.0, 0.0));
    Particle p4(2.0, Eigen::Vector3d(1.0, 1.0, 1.0), Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(0.0, 0.0, 0.0));

    Eigen::Vector3d expected_acc = Eigen::Vector3d(0.1925, 0.1925, 0.1925);
    Eigen::Vector3d actual_acc = calcAcceleration(p1, p2);
    REQUIRE(expected_acc.isApprox(actual_acc, 0.001));

    expected_acc = Eigen::Vector3d(0.125, 0.125, 0.125);
    actual_acc = calcAcceleration(p1, p2, 1.0);
    REQUIRE(expected_acc.isApprox(actual_acc, 0.001));

    expected_acc = Eigen::Vector3d(-0.0191, -0.0381, -0.0572);
    actual_acc = calcAcceleration(p1, p3, 0.1);
    REQUIRE(expected_acc.isApprox(actual_acc, 0.001));

    expected_acc = Eigen::Vector3d(0.25, 0.25, 0.25);
    actual_acc = calcAcceleration(p1, p4, 1.0);
    REQUIRE(expected_acc.isApprox(actual_acc, 0.001));
}

TEST_CASE("A list of particles that includes itself") {
    Particle p1(1.0, Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3d(1.0, 2.0, 3.0));

    std::vector<Particle> particles;
    particles.push_back(p1);

    Eigen::Vector3d expected_total_acc = Eigen::Vector3d(1.0, 2.0, 3.0);
    Eigen::Vector3d actual_total_acc = p1.getAcceleration() + calcTotalAcceleration(p1, particles);
    REQUIRE(expected_total_acc.isApprox(actual_total_acc, 0.001));
}

TEST_CASE("Zero acceleration between two equal particles") {
    double m = 1.0;
    Eigen::Vector3d pos0(0.0, 0.0, 0.0);
    Eigen::Vector3d pos1(1.0, 0.0, 0.0);
    Eigen::Vector3d pos2(-1.0, 0.0, 0.0);

    Particle p0(m, pos0);
    Particle p1(m, pos1);
    Particle p2(m, pos2);

    std::vector<Particle> particles{p0, p1, p2};
    Eigen::Vector3d actual_total_acc = calcTotalAcceleration(p0, particles);
    REQUIRE(Eigen::Vector3d::Zero().isApprox(actual_total_acc, 0.001));
}