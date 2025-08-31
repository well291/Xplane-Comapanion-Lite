# X‑Plane Companion Lite (PC)

This repository provides a minimal desktop companion that talks to an X‑Plane instance on the **same machine** via UDP.

## Lite Feature Set
- **Aircraft** – browse installed aircraft and transmit the selection.
- **Start** – pick an airport and ramp/runway for the initial location.
- **Map** – moving map with aircraft position and basic navaids.
- **Commands** – searchable X‑Plane command list with instant sending.

## Build
Compile the sample stub:
```bash
g++ -std=c++17 pc_lite/main.cpp -o pc_lite_app
```

## Run
1. Launch X‑Plane and ensure **UDP** is enabled on port **49000**.
2. Execute a command by passing its name on the command line:
```bash
./pc_lite_app "sim/operation/pause_toggle"
```
The app prefixes the argument with `CMND` and sends it to `localhost:49000`.

Replace the command string with any valid X‑Plane command.
