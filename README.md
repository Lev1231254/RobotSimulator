# RobotSimulator

A C++17 simulator for robot navigation in initially unknown 2D
environments. Select a target cell and watch the robot discover
obstacles, update its internal map, and adjust its planned route.

<!-- Add a screenshot or demo GIF here. -->

## Overview

RobotSimulator demonstrates grid-based mapping and path planning
in an interactive 2D environment.

The robot begins without knowledge of the surrounding obstacles.
As it moves toward a selected destination, simulated distance
measurements reveal the environment, allowing it to update its map
and replan its route.

## Technologies

- **C++17**
- **SFML 3** for graphics and window handling
- **CMake 3.22 or newer**


## Environment assumptions
- Static obstacles.
- Initially unknown environment.
- Perfect sensors with infinite range.
- All cells initially assumed to be free.


## Algorithms and structures
- Memory grids:

  The whole screen is devided into a grid. Each cell is 16x16 pixels square. 

  Obstacle grid:
  
    Each square can have values:
      if 0 no obstacle
      if > 0 time in seconds, when the obstacle was discovered (was made only to make cool color fading in inner map)
  
    Initially all cells are 0, which means that robot thinks, there are no obstacles.
  
    Each time the robot moves, it updates the inner map, if the new obstacle was discovered. It builds the inner map gradually.
  
    The obstacles are not moving in this version yet, which is why it's enough to only mark the obstacles to build the map.
    Since all cells are initially assumed to be free, the sensor only needs to mark the cell where an obstacle is detected. All cells traversed by the ray before the obstacle are already considered free.

  Pass grid:

    In order to make robot avoid the walls we kinda inflate the obstacles by around the radius of the robot. That is represented through a pass grid.
    (picture with obstacle, pass zone and all lengths)
    
    Pass grid has the same size as obstacle grid. 
    
    1 - passable
      
    0 - not passable

    Each time we update the obstacle grid, we update the pass grid. We don't update the whole grid but rather update the cells around the newly discovered obstacle-cell.

  
- Pathfinding:
  Main idea of the algorithm is to use A* to find the shortest path to the destination on the pass grid. A* is like dijkstra, but it first checks the cells, that are closer to the destination. When A* finds the path to the destination it stops.

  If it doesn't find it (for example the destination is unreachable) it will find the closest free cell to the destination and go there. It gradually checks bigger and bigger squares around the destination. Finds the first square with free cells and picks the closest cell using heuristic distance. 
  This method is up to change, because if the robot picks a free, but an unreachable cell, it will try to make a path to this new destination. But since it's unreachable it will try to find new destination again and it could be exaclty the first cell, that we already dropped. Thus creating infinite loop. It will be fixed in the future.

  

  If the robot is already too deep in the unpassable zone (maybe it discovered the obstacles too late) and it tries to make a new path, the robot will add the nearest to the goal free cell to the beginning of the path and traverse the path normally. So it doesn't affect the 

  Robot automatically updates the path when 300ms passed since the last update and a new obstacle was discovered since the last update.

  The robot updates the path, even if the discovered obstacle is not close to a path, because i didn't have the time to make the solution.

- Sensors:
  The robot has 3 ideal infinite distance sensors.

  They are using ray-casting the find the nearest obstacle in line and return it's coordinates.

  For ray-casting I used Amanatides-Woo voxel traversal method, because it is a very effective algrorithm for voxel grid for finding the first hit of the ray. This way we remove unnecessary geometrical calculations.

  To avoid the sensor pointing directly along a grid boundary and not being able to decide the end square, we add a very small distance along the ray to the endpoint. Thus getting an endpoint nudged to the next cell along the ray.

| Component | Implementation |
|---|---|
| Environment representation | Grid-based map |
| Mapping | Incremental obstacle discovery |
| Navigation | A* + a few changes |
| Collision avoidance | Obstacle inflation |
| Perception | Three raycasting sensors |
| Ray traversal | Amanatides-Woo |
| Dynamic adaptation | Periodic, event-triggered replanning |


## Navigation loop

<img width="707" height="451" alt="image" src="https://github.com/user-attachments/assets/8829f957-0fc2-4891-bf59-2d933f0cca1d" />

## Project Structure

```text
RobotSimulator/
├── assets/
│   └── fonts/        # Font assets
├── include/          # Header files
├── src/              # C++ implementation files
└── CMakeLists.txt    # Build configuration
```



## Build

### Requirements

- A compiler supporting C++17
- CMake 3.22 or newer
- Git and internet access for the initial SFML download
- Platform-specific dependencies required to build SFML

SFML is fetched from its upstream repository during CMake
configuration.

### Configure and compile

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
```

The executable is named `robot_simulator`. Its location depends
on the selected CMake generator and platform.

### Optional profiling build

GNU gprof instrumentation can be enabled when building with GCC:

```sh
cmake -S . -B build-profile \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DROBOT_SIMULATOR_ENABLE_PROFILING=ON

cmake --build build-profile --parallel
```

Collecting and inspecting profiling results also requires a
compatible platform and the `gprof` tool.
