# Bread Physics
A simple rigidbody physics engine with a graphical demo made with openGL. Currently under development don't expect a working physics demo yet.


# Build Bread Physics Library
(TODO)

# Build Bread Physics Demo

Dependencies are included as submodules to the project. If you prefer to use precompiled static or dynamic libraries then it's up to you to re-configure the build system as I haven't supported that as of yet.

Clone project: `git clone --recursive https://github.com/Ethan-Mulwee/Bread-Physics.git`

 Linux build requirements for debian
 ```
 sudo apt install libxmu-dev libxi-dev libgl-dev dos2unix git wget # GLEW
 sudo apt install libwayland-dev libxkbcommon-dev xorg-dev # GLFW
 ```
 Build with CMake
 ```
 cd Bread-Physics/
 cmake -S . -B build/ # or cmake -DCMAKE_BUILD_TYPE:STRING=Release -S . -B build/ for release builds
 cmake --build build/
 ```


 Binary is generated at build/App to run: `cd /build && ./Bread-Physics-Demo`
 Note: its important that the working directory is that of build for the moment
