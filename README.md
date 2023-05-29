# A Virtual Solar System

## Introduction

Welcome to the Virtual Solar System Simulator, which is a N-body simulator that models the dynamics of the Solar System. This project contains two main parts: **Solar System Simulator & Improving the simulation**.

## Installing dependencies

We are using the package manager Conan to install the dependencies Catch2 and Eigen. In order to use CMake's `Release` target for performance and `Debug` for debugging, the libraries must be installed twice with:

```
conan install . --output-folder=build --build=missing -s build_type=Debug
conan install . --output-folder=build --build=missing -s build_type=Release
```

If you delete the `build` directory to clean your build, you may have to install the dependencies again.

## Building

To build from the project root directory you should run:

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

If you wish to debug your code, you should replace `Release` with `Debug`. For performance measurements, ensure you have built with the `Release` target.

## Testing

Once the project has been built, it can be tested by running:

```
cd build
ctest
```

## A full year simulation of the Solar System

The initial data of the Solar Sytem is as follows:  
// Masses in order Sun, Mercury, Venus, etc  
{1.,1./6023600,1./408524,1./332946.038,1./3098710,1./1047.55,1./3499,1./22962,1./19352}  
// Distances from Sun  
{0.0, 0.4, 0.7, 1, 1.5, 5.2, 9.5, 19.2, 30.1}

After set the Sun’s position and velocity to zero and randomly choose an angle θ for each of the planets,
the following table show the position of the Solar System bodies ar the start and end of simulation. The timestep set here is 0.0001 (A full year is a time of 2π) to keep the accuracy of the simulation.

| Body | Position_start | Position_end |
|---|---|---|
|Sun|(0, 0, 0)|(-0.000392659, 0.000527146, 0)|
|Mercury|(-0.337478, 0.214729, 0)|(-0.14129, 0.383279, 0)|
|Venus|(0.431176, -0.551441, 0)|(-0.656156, -0.252146, 0)|
|Earth|(-0.978452, 0.206473, 0)|(-0.978818, 0.213061, 0)|
|Mars|(-1.43106, 0.449521, 0)|(1.49967, -0.0387928, 0)|
|Jupiter|(-2.74071, 4.41911, 0)|(-4.59836, 2.42787, 0)|
|Saturn|(8.9888, 3.07431, 0)|(8.12788, 4.91788, 0)|
|Uranus|(16.5126, -9.79672, 0)|(17.1975, -8.53729, 0)|
|Neptune|(-29.9028, 3.44012, 0)|(-30.012, 2.30016, 0)|

As shown in the table: the positions of Earth at the start and end are almost the same, which proves the accuracy of the simulator.

## Calculate the energy loss

The following table shows the energy loss after simulating 100 years of time (100 * 2π simulation time) using 8 different timesteps.

| Timestep | Total energy loss |
|---|---|
|0.5|1.14794e-04|
|0.1|3.74897e-05|
|0.05|2.68512e-05|
|0.01|9.92078e-06|
|0.005|6.36139e-06|
|0.001|2.60413e-06|
|0.0005|1.83347e-06|
|0.0001|7.21456e-07|

As shown in the table, it is easy to find that when timestep is large, more energy is lost during simulating, which means the simulation is less actuate.

## Benchmark the simulation

To run the code of this part with compiler optimisations, you should comment the following code in main.cpp:

```
#ifdef DEBUG
#endif
```

And run:

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Otherwise, if you want to test the runtime without compiler optimisations, you should run:

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

In this part, to test the runtime of the code, the 8 timestrp sizes are {0.5, 0.1, 0.05, 0.01, 0.005, 0.001, 0.0005, 0.0001}. And the performance results are shown below.

Without compiler optimisations:

| Timestep | Total time (ms) | Average time per timestep (ms) |
|---|---|---|
|0.5|146|0.116242|
|0.1|260|0.0413815|
|0.05|989|0.0787044|
|0.01|3312|0.0527128|
|0.005|6401|0.0509378|
|0.001|35875|0.0570969|
|0.0005|71950|0.057256|
|0.0001|471170|0.074989|

With compiler optimisations:

| Timestep | Total time (ms) | Average time per timestep (ms) |
|---|---|---|
|0.5|527|0.419586|
|0.1|160|0.0254655|
|0.05|169|0.013449|
|0.01|364|0.00579332|
|0.005|1145|0.00911167|
|0.001|4806|0.00764899|
|0.0005|9379|0.00746357|
|0.0001|38710|0.00616089|

As shown in the tables, the average time per timestep is about 0.007ms with compiler optimisations and 0.06ms without compiler optimisations. 

## Increasing the scale of the system

After generate simulations with 8, 64, 256, 1024 and 2048 particles and set a evolution time of 100 years. The performance of simulation with different timestep is as follows:

| Num of particles | Timestep | Energy loss |
|---|---|---|
|8|0.1|0.00144786|
|8|0.05|0.00128153|
|8|0.01|0.00105377|
|64|0.1|0.00518002|
|64|0.05|0.00290691|
|64|0.01|0.00306849|
|256|0.1|0.72493|
|256|0.05|0.319785|
|256|0.01|0.191881|
|1024|0.1|3.38697|
|1024|0.05|12.8158|
|1024|0.01|24.3949|
|2048|0.1|29.2159|
|2048|0.05|40.3453|
|2048|0.01|46.6674|

As the table shows, for small number of particles, the energy loss gets lower when timestep is small. However, for large number of particles, the energy loss turns bigger if timestep is small. There are two main reasons for this performance:

1. Cumulative rounding error. As the number of step gets bigger, rounding error may accumulate which causes bigger energy loss.
2. Stronger interaction. As the number of particles increases, the interaction can get more complicated which causes more instability as timestep gets smaller.

## Parallelising with OpenMP

In this part, we can set 2048 particles as initial conditions and use time step of 0.001 as the use-case.

Since there is no nested loop in the code, we can only use **schedule** clause to test the performance.

|Mode|Runtime (s)|
|---|---|
|No parallelising|102.44|
|Basic parallelising|12.32|
|Parallelising with schedule(dynamic, 4)|11.468|
|Parallelising with schedule(guided)|12.516|

From the table above, parallelising with schedule(dynamic, 4) has the best performance.

To run a strong scaling experiment, we can get result in the following table.

| OMP_NUM_THREADS | Time (s) | Speedup |
|---|---|---|
| 1 | 103.32 | 1 |
| 2 | 54.229 | 1.91 |
| 3 | 37.027 | 2.79 |
| 4 | 28.428 | 3.63 |
| 16 | 11.675 | 8.85 |

To run a weak scaling experiment, we can get result in the following table.

| OMP_NUM_THREADS | Num Particles | Time (s) | Speedup |
|---|---|---|---|
| 1 | 1024 | 27.36 | 1 |
| 2 | 2048 | 54.005 | 0.51 |
| 3 | 3072 | 84.836 | 0.32 |
| 4 | 4096 | 118.316 | 0.23 |
| 16 | 16384 | 815.869 | 0.03 |