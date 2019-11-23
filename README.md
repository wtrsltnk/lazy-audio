# lazy-audio

This repo contains a few program examples generating sound the lazy way: with RtAudio.

## How to compile this code

* Create a build directory at the root of this repo
* Open a commandline window from this directory with CMake and a compiler supported by CMake in its path
* Generate the build files with CMake:
    * cmake -G "MinGW Makefiles" ..\oscillator
* Build the project with CMake again:
    * cmake --build .