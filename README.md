# Space Invaders (1978) Emulator
<img src = "/screenshots/SpaceInvaders.gif?raw=true">
A fully-functional complete emulation of the 1978 Space Invaders arcade machine as well as the Intel 8080 microprocessor.

## Features
* Complete Intel 8080 CPU emulator with accurate cycle count that passes multiple test ROMs
* Space Invaders arcade machine emulator with accurate display and interrupt timing

## Requirements
* SDL2
* SDL2_mixer
* CMake (for automatic build)

## Build
`cmake -S . -B build -DCMAKE_BUILD_TYPE=Release`  
`make invaders` (to build the space invaders emulator)  
`make intel-8080-tests` (to build the CPU tester)

## Run
### Linux
`./invaders` (Space Invaders Emulator)  
`./intel-8080-tests` (CPU Tester)

### Windows
`invaders.exe` (Space Invaders Emulator)  
`intel-8080-tests.exe` (CPU Tester)

## References
[Intel 8080 User/Assembly Manuals](docs)  
[Computer Archaeology](https://www.computerarcheology.com/Arcade/SpaceInvaders/)  
[Emulator 101](http://www.emulator101.com/welcome.html)  
[Superzazu's Implementation (for tests and debugging)](https://github.com/superzazu/8080)
