#include <iostream>
#include "particle.hpp"
#include "InitialConditionGenerator.hpp"
#include "SolarSystemGenerator.hpp"
#include "RandomSystemGenerator.hpp"
#include <cmath>
#include <vector>
#include <random>
#include <chrono>
#include <cstring>
#include <memory>

// Function to print help messages
void print_usage(char* program) {
  std::cerr << "Usage: " << program << "\n";
  std::cerr << "Options: \n";
  std::cerr << "  --generator solar <dt> <time_steps> Generate Solar system with initial & evolution condition\n";
  std::cerr << "  --generator random <dt> <time_steps> <num_particles> Generate random system with initial & evolution condition\n";
  std::cerr << "  --softening <epsilon> Set the value of epsilon\n";
  std::cerr << "  --openMP <dt> <time_steps> <num_particles> Generate random system with initial & evolution condition to check performance using openMP\n";
  std::cerr << "  --help, -h  Show help messages\n";
  std::cerr << "\n";
  std::cerr << "Examples:\n";
  std::cerr << "  " << program << " --generator solar 0.001 1000\n";
  std::cerr << "  " << program << " --generator random 0.001 1000 2048\n";
  std::cerr << "  " << program << " --generator solar 0.001 1000 --softening 0.001\n";
  std::cerr << "  " << program << " --generator random 0.001 1000 2048 --softening 0.001\n";
  std::cerr << "  " << program << " --openMP 0.001 1000 2048\n";
}

// Function to print position & energy changes for a list of particles
void print_position_energy(std::string type, double dt, int time_steps, double epsilon = 0.0, int num_particles = 0){
  std::vector<Particle> particles_start;
  std::vector<Particle> particles_end;
  if (type == "solar") {
    InitialConditionGenerator *generator = new SolarSystemGenerator();
    particles_start = generator->generateInitialConditions();
    particles_end = evolution_Solar_System(particles_start, dt, time_steps, epsilon);
  }
  else if (type == "random") {
    InitialConditionGenerator *generator = new RandomSystemGenerator(num_particles);
    particles_start = generator->generateInitialConditions();
    particles_end = evolution_Solar_System(particles_start, dt, time_steps, epsilon);
  }
  else {
    std::cerr << "Type should be either 'solar' or 'random'\n";
  }
  for (int i = 0; i < particles_start.size(); i++) {
    std::cout << "Body No." << i + 1 << " Start position: " << particles_start[i].getPosition().transpose() << "\n";
    std::cout << "Body No." << i + 1 << " End position: " << particles_end[i].getPosition().transpose() << "\n";
  }
  double total_energy_start = totalEnergy(particles_start);
  double total_energy_end = totalEnergy(particles_end);
  std::cout << "Total start energy: " << total_energy_start << "\n";
  std::cout << "Total end energy: " << total_energy_end << "\n";
  std::cout << "Energy loss: " << total_energy_start - total_energy_end << "\n";
}

// Function to print the time cost of the evolution of system
void print_openMP_performance(double dt, int time_steps, int num_particles) {
  InitialConditionGenerator *generator = new RandomSystemGenerator(num_particles);
  std::vector<Particle> particles_start = generator->generateInitialConditions();
  auto start = std::chrono::high_resolution_clock::now();
  std::vector<Particle> particles_end = evolution_Solar_System(particles_start, dt, time_steps, 0.001);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  double total_time = duration / 1000.0;
  std::cout << "| " << omp_get_max_threads() << " | " << total_time << " |\n";
}

int main(int argc, char* argv[]) {
  
  if (argc == 1 || (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))) {
    print_usage(argv[0]);
  }
  
  if (argc == 5 && strcmp(argv[1], "--generator" ) == 0 && strcmp(argv[2], "solar") == 0) {
    std::string type = argv[2];
    double dt = std::atof(argv[3]);
    int time_steps = std::stoi(argv[4]);
      
    print_position_energy(type, dt, time_steps);
  }

  if (argc == 6 && strcmp(argv[1], "--generator") == 0 && strcmp(argv[2], "random") == 0) {
    double dt = std::atof(argv[3]);
    int time_steps = std::stoi(argv[4]);
    int num_particles = std::stoi(argv[5]);
      
    print_position_energy("random", dt, time_steps, 0.0, num_particles);
  }

  if (argc == 7 && strcmp(argv[1], "--generator" ) == 0 && strcmp(argv[2], "solar") == 0 && strcmp(argv[5], "--softening") == 0) {
    std::string type = argv[2];
    double dt = std::atof(argv[3]);
    int time_steps = std::stoi(argv[4]);
    double epsilon = std::atof(argv[6]);
      
    print_position_energy(type, dt, time_steps, epsilon);
  }

  if (argc == 8 && strcmp(argv[1], "--generator" ) == 0 && strcmp(argv[2], "random") == 0 && strcmp(argv[6], "--softening") == 0) {
    std::string type = argv[2];
    double dt = std::atof(argv[3]);
    int time_steps = std::stoi(argv[4]);
    int num_particles = std::stoi(argv[5]);
    double epsilon = std::atof(argv[7]);
      
    print_position_energy(type, dt, time_steps, epsilon, num_particles);
  }

  if (argc == 5 && strcmp(argv[1], "--openMP" ) == 0) {
    double dt = std::atof(argv[2]);
    int time_steps = std::stoi(argv[3]);
    int num_particles = std::stoi(argv[4]);

    print_openMP_performance(dt, time_steps, num_particles);
  }

  // You should be able to enable or disable any debug during the CMake configure process with cmake -S. -B build -DCMAKE_BUILD_TYPE=Debug ... or -DCMAKE_BUILD_TYPE=Release.
  #ifdef DEBUG
  std::vector<Particle> particles_start = initial_condition_generator();
  std::vector<double> dt_vec = {0.5, 0.1, 0.05, 0.01, 0.005, 0.001, 0.0005, 0.0001};
  double year_100 = 100 * 2 * M_PI;
  for (int i = 0; i < dt_vec.size(); i++) { 
    double dt = dt_vec[i];
    int time_steps = int(year_100 / dt);

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Particle> particles_end = evolution_Solar_System(particles_start, dt, time_steps);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    double avg_time_per_timestep = static_cast<double>(duration) / time_steps;

    std::cout << "Timestep size: " << dt << ", Total time: " << duration << "ms, Average time per timestep: " << avg_time_per_timestep << "ms" << std::endl;
  }
  #endif

}