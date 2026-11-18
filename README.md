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
- **GNU gprof instrumentation** as an optional build setting

## Project Structure

```text
RobotSimulator/
├── assets/
│   └── fonts/        # Font assets
├── include/          # Header files
├── src/              # C++ implementation files
└── CMakeLists.txt    # Build configuration
```

## Main discovery loop

```mermaid
flowchart TD
    A([Start loop]) --> B{"foundNewObstacle = 1<br/>and ≥ 300 ms passed?"}
    B -- yes --> C["Find new path (A*)<br/>foundNewObstacle = 0<br/>restart 300 ms timer"]
    B -- no --> D
    C --> D["Scan sensors<br/>get occupied points"]
    D --> E{"Point not in<br/>mask yet?"}
    E -- yes --> F["Update occupancy mask<br/>foundNewObstacle = 1"]
    E -- no --> G
    F --> G["Move robot<br/>along the path"]
    G -- next tick --> A

    classDef always fill:#E1F5EE,stroke:#0F6E56,color:#085041
    classDef conditional fill:#EEEDFE,stroke:#534AB7,color:#3C3489
    class D,G always
    class C,F conditional
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
