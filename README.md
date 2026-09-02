# N-Body Simulation

A simple interactive 2D N-body gravitational simulation made using C++ and raylib.

I made this project to understand how gravitational interactions work between multiple objects and to get more comfortable with physics simulations in C++.

## How It Works

Each body in the simulation has:

- Position
- Velocity
- Mass
- Radius
- Trajectory trail

For every simulation step, each body calculates the gravitational force from every other body using Newton's law of universal gravitation:

\[
F = G\frac{m_1m_2}{r^2}
\]

The forces acting on each body are added together to find the net force.

The net force is then used to calculate acceleration:

\[
a = \frac{F}{m}
\]

The acceleration is used to update the velocity and position of each body.

The gravitational constant `G` is scaled up from the real-world value so that the movement is visible and suitable for a 2D simulation.

## Features

- Multiple-body gravitational simulation
- Create bodies interactively with the mouse
- Drag bodies to change their position and give them velocity
- Randomized body sizes
- Mass calculated from body size and density
- Gravitational interaction between all bodies
- Collision detection
- Trajectory trails
- Pause and resume the simulation
- Reset the simulation
- Real-time FPS counter

## Controls

| Input | Action |
|---|---|
| Left Mouse Button | Create a new body |
| Click + Drag | Move a body and give it velocity |
| Space | Pause / Resume |
| R | Reset the simulation |

## Implementation

The bodies are stored using a C++ `vector`.

For every body, the program checks every other body and calculates the gravitational force between them.

Because every body interacts with every other body, the current implementation has approximately:

\[
O(N^2)
\]

time complexity, where `N` is the number of bodies.

The trajectory of each body is stored using a `deque` of previous positions. These positions are then connected with lines to create the visible trails.

## Collision Detection

The simulation checks the distance between two bodies and compares it with the sum of their radii.

If:

\[
d \leq r_1 + r_2
\]

the bodies are considered to have collided and the simulation stops.

## Random Body Generation

When a new body is created, its radius is randomly generated.

The mass is then calculated using:

\[
m = \pi r^2 \rho
\]

where:

- `r` is the body's radius
- `ρ` is the density
- `π` is pi

This gives larger bodies a greater mass and therefore a stronger gravitational influence.

## Built With

- C++
- raylib
- C++ STL
  - `vector`
  - `deque`

## Future Improvements

Some improvements I would like to add in the future:

- [ ] Barnes-Hut algorithm
- [ ] Quadtree optimization
- [ ] Better collision physics
- [ ] Body merging
- [ ] Adjustable gravitational constant
- [ ] Simulation speed controls
- [ ] Camera zoom and panning
- [ ] Orbit prediction
- [ ] Solar system presets
- [ ] Energy conservation tracking
- [ ] More advanced visual effects

## Author

**Lalith**

This project was developed from scratch as a personal C++ and raylib project.

**All source code in this repository was written by me.**

The project was created to learn and experiment with gravitational physics, numerical simulation, and interactive graphics programming.

## License

This project is currently intended as a personal learning and portfolio project.