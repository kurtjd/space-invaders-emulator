# Space Invaders (1978) Emulator
<img src = "/screenshots/SpaceInvaders.gif?raw=true">
A fully-functional and complete emulation of the 1978 Space Invaders arcade machine as well as the Intel 8080 microprocessor.

## Features
* Complete Intel 8080 CPU emulator with accurate cycle count that passes multiple test ROMs
* Space Invaders arcade machine emulator with accurate display and interrupt timing

## Requirements
* SDL2
* SDL2_mixer
* CMake (for automatic build)
* Space Invaders ROM files (see roms/space-invaders/README.txt)
* Sound effect files (see sound/README.txt)

## Build
`cmake -S . -B build -DCMAKE_BUILD_TYPE=Release`  
`make invaders` (to build the space invaders emulator)  
`make cpu-tests` (to build the CPU tester)

## Run
### Linux
`./invaders` (Space Invaders Emulator)  
`./cpu-tests <0:1>` (CPU Tester (1 to enable debug output))

### Windows
`invaders.exe` (Space Invaders Emulator)  
`cpu-tests.exe <0:1>` (CPU Tester (1 to enable debug output))

## Controls
|Key|Action|
|---|------|
|`C`|Insert Coin|
|`ENTER`|1P Start|
|`2`|2P Start|
|`SPACE`|Shoot|
|`LEFT`|Move Left|
|`RIGHT`|Move Right|
|`T`|Tilt|

## References
Intel 8080 User/Assembly Manuals  
[Computer Archaeology](https://www.computerarcheology.com/Arcade/SpaceInvaders/)  
[Emulator 101](http://www.emulator101.com/welcome.html)  
[Superzazu's Implementation (for tests and debugging)](https://github.com/superzazu/8080)
