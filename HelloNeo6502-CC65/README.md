# HelloNeo6502

**This is a sample CC65 project that:**
- Includes the headers and libraries required to build CC65 for Neo6502
- Shows how to build CC65 projects for Neo6502 (build.bat and build.sh)
- Includes a Neo6502 API library

# To Build
The project is built either using build.bat files or build.sh files. These files must first be edited to add the indicated paths to CC65 and to the latest Neo6502 build tools.

The project produces two versions of the executable file:
- hello.nee
- hello.neo

As of this writing, the Neo6502 emulator cannot run *.neo files from the command line. Instead, use the *.nee file:

<path to emulator>neo.exe keys "hello.nee"@800 cold

To run the program from NeoBASIC (emulator or NEO6502), use the *.neo file:

run "hello.neo"


# The Neo6502 Development Environment

**neo6502.h**
Note that this file is different from a previous version with the same name. Provides definitions and macros for Neo6502 API access. It would be included in any *.c source file that uses Neo6502 API or calls API helper functions.

**neo6502lib.c** and **neo6502lib.h**
Provides a collection of Neo6502 API access functions. The *.h file provides definitions for the functions and would be included in any *.c source file that uses these functions. This is not a CC65 precompiled library, though it could be made into one.

# Origins and Changes

This project started with "examples/C" and "examples/semi-working-cc65". I replaced the _write() function and added a _read() function by adapting the coresponding functions from from the kim1 library in CC65. I modified crt0 to return to NeoBASIC on exit. To make the neo6502.h file work with multi-source projects, I changed the API access method to use macros instead of defining pointer variables in the header. Finally, I changed the definition of the API access registers from uint16_t to unit8_t.

Neo6502 is not an official target in CC65. This is only a minor issue. When we build CC65 projects for the Neo6502, we have to set a target of "none". This causes the CC65 build tools to look for C runtime functions (e.g. _write, _read, crt0, etc.) in "none.lib". To put the Neo6502 specific versions of these functions into "none.lib", we keep our own custom version of "none.lib" in the project build directory. There is a subdirectory "nonelib" that has all the components and build scripts (buildlib.bat/buildlib.sh) to update our verion of none.lib and then copy it into the main build directory. You can get an idea of the sorts of things that belong in here by looking at the CC65/libsrc/kim1 directory.

# Future Development

- There are other API functions that should be added to Neo6502Lib.c (e.g. graphics)
- More C runtime functions such as file I/O will round out the development environment
- Neo6502Lib can be turned into a CC65 library if compile times become an issue
- An issue with keyboard input not being echoed requires investigation
- It might be possible to add limited floating point support through the Neo6502 API

# Contact

I'm on the Neo6502 Discord as Gollan.


