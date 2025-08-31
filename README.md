# X‑Plane Companion Lite (PC)

This repository provides a minimal desktop companion that talks to an X‑Plane instance on the **same machine** via UDP.

## Lite Feature Set
- **Aircraft** – browse installed aircraft and transmit the selection.
- **Start** – pick an airport and ramp/runway for the initial location.
- **Map** – moving map with aircraft position and basic navaids.
- **Commands** – searchable X‑Plane command list with instant sending.

## Build
Install the SDL2 development libraries if they are not already present:
```bash
sudo apt-get update
sudo apt-get install -y libsdl2-dev libsdl2-ttf-dev
```

Compile the sample GUI stub:
```bash
g++ -std=c++17 pc_lite/main.cpp -o pc_lite_app $(sdl2-config --cflags --libs) -lSDL2_ttf
```

## Run
1. Launch X‑Plane and ensure **UDP** is enabled on port **49000**.
2. Edit `pc_lite/commands.txt` to list the X‑Plane commands you want buttons for.
3. Start the app without arguments for the button GUI:
```bash
./pc_lite_app
```
   Clicking a button sends its command to `localhost:49000` using the `CMND` message.
4. Or send a single command directly from the command line:
```bash
./pc_lite_app "sim/operation/pause_toggle"
```
