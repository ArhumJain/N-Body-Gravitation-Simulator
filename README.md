# N-Body Gravitational Particle Simulator
Uses SSBO's and transform feedback for GPU accelerated, real-time physics. Can simualte attractions between 25000-30000 particles at 60 FPS on a GTX 1080.

No formal parameter tweaking system so if you want to experiment you can play with the values in the vertex shader (where all the physics computations goes down).

Compiled with MSVC 2022 (Only on Windows 10) and support for OpenGL 4.3 and higher. 

## Controls
- WASD - move around
- Space - move up
- Ctrl - move down
- Shift - speed up
- Mouse to look around


![demo1](/res/demo1.png "demo1")


![demo2](/res/demo2.png "demo2")