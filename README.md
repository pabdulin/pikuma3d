## Repo info
Pikuma course [3D Computer Graphics Programming](https://pikuma.com/courses/learn-3d-computer-graphics-programming)

## Common setup for Windows
1. Assuming you have [Git for Windows](https://git-scm.com/download/win) installed with bash and linux tools in cmd (default settings).
2. Install [w64devkit](https://github.com/skeeto/w64devkit/releases) and put `bin` into system PATH.

## Notes on modules
1. Introduction
2. Compiling our Project
   1. Get [SDL2-devel-2.30.2-mingw.zip](https://github.com/libsdl-org/SDL/releases/tag/release-2.30.2)
      1. Put `SDL2-2.30.2\x86_64-w64-mingw32\include\SDL2` into `third_party\include\SDL2`
      2. Put `SDL2-2.30.2\x86_64-w64-mingw32\lib` into `third_party\lib`
   2. Ensure opeining `bash` with `Open Git Bash here` and/or `cmd.exe` and running `make` produces `renderer.exe`
3. SDL Setup and Color Buffer
4. 