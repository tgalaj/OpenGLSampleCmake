# OpenGLSampleCmake

Sample CMake for OpenGL based projects.

For more information about how all of these was done please refer to this [link](https://shot511.github.io/2018-05-29-how-to-setup-opengl-project-with-cmake/).

## Build dependencies
 - CMake (to download click [here](https://cmake.org/download/))
 - IDE (e.g. Visual Studio)

## Build instructions
**Note**: this CMake was only tested on Windows platform. Linux and Mac platforms weren't tested.

1. Make sure that CMake is installed and added to the system PATH variable.
2. Open terminal in the CMakeLists.txt directory and run:

 ```
 mkdir build
 cd build
 cmake -A Win32 ..
 ```

NOTE: Project must be configured as Win32 due to precompiled x86 binaries of thirdparty libraries.

 ## Additional info
 Any improvements to CMake script are greatly welcome.
