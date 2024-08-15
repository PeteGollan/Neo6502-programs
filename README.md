# Contents
- Neo6502_nonelib: a library to connect CC65 functions to the Neo6502 API
- HelloNeo6502: demonstrates Neo6502 features using CC65
- Test-CC65-IO: tests and demonstrates CC65 console I/O functions on Neo6502

# Neo6502_nonelib
**This is a custom library that connects CC65 functions to the Neo6502 API (hardware):**
- Creates none.lib, which is used in CC65 projects for the "none target" (-t none) where the device is not officially supported
- This is a work-around until Neo6502 is officially added to CC65
- Includes the headers and functions required to use CC65 for Neo6502

# To Build none.lib
Use:
.\buildlib.bat **or** ./buildlib.sh
**These files must first be edited to add the indicated paths to CC65 and to the Neo6502 include files.** 
none.lib only has to be built once, unless the functions are changed. It can be copied to your project directory. For convenience, the project build scripts
copy a none.lib from the Neo6502-nonelib directory automatically.

# HelloNeo6502
**This is a sample CC65 project that:**
- Includes the headers and libraries required to use CC65 for Neo6502
- Shows how to build CC65 projects for Neo6502 (build.bat/buildrun.bat, and build.sh/buildrun.sh)
- Includes a Neo6502 API library

# Test-CC65-IO
**This is a sample CC65 project that:**
- Is a demonstration/test of the Neo6502 console and file I/O functions for CC65
- Shows how to build CC65 projects for Neo6502 (build.bat/buildrun.bat, and build.sh/buildrun.sh)
- To start your own project, consider copying this directory and editing the name of the *.c and *.nee and *.neo files.
- Note: as of this writing, Test 13 fails on the Linux version of the emulator

# To Build Hello.neo and Test-CC65-IO.neo
These projects are built either using build.bat files or build.sh files. **These files must first be edited to add the indicated paths to CC65 and to the latest Neo6502 build tools.** 
To build the project from the build envornment (Windows PowerShell or Terminal), use build.bat or build.sh:
.\build **or** ./build.sh

The build script produces the program file hello.neo or Test-CC65-IO.neo (the *.nee file is an intermediate file and is not needed after the build). To to simplify development,
the build scripts copy the *.neo file to the project storage directory.

For convenience, the build scripts also copy the latest none.lib from Neo6502_nonelib. These lines can be disabled if you want to manage copying the library yourself.

To run the program from the build enviornment (Windows PowerShell or Terminal), use buildrun.bat or buildrun.sh:
.\buildrun **or** ./buildrun.sh

To run the program from the Morpheous/NeoBASIC command line (emulator or NEO6502 hardware), use the *.neo file:
run "hello.neo" **or** run "Test-CC65-IO.neo"


# The Neo6502 Development Environment

**neo6502.h**
Note that this file is different from a previous version with the same name. Provides definitions and macros for Neo6502 API access for C functions. It would be included
in any *.c source file that uses Neo6502 API or calls API helper functions.

**neo6502.asm.inc**
Note that this file is different from a previous version with the same name. Provides definitions and macros for Neo6502 API access for 65C02 assembly language functions. It would be
included in any *.s source file that uses Neo6502 API or calls API helper functions.


**neo6502lib.c** and **neo6502lib.h**
Provides a collection of Neo6502 API access functions. The *.h file provides definitions for the functions and would be included in any *.c source file that uses these functions.
This is not a CC65 precompiled library, though it could be made into one.

# Origins and Changes

This project started with "examples/C" and "examples/semi-working-cc65". I replaced the _write() function and added a _read() function by adapting the coresponding functions
from from the kim1 library in CC65. I modified crt0 to return to NeoBASIC on exit. To make the neo6502.h file work with multi-source projects, I changed the API access method
to use macros instead of defining pointer variables in the header. Finally, I changed the definition of the API access registers from uint16_t to unit8_t.

Neo6502 is not an official target in CC65. This is only a minor issue. When we build CC65 projects for the Neo6502, we have to set a target of "none". This causes the
CC65 build tools to look for C runtime functions (e.g. _write, _read, crt0, etc.) in "none.lib". To put the Neo6502 specific versions of these functions into "none.lib",
we keep our own custom version of "none.lib" in a local directory. You can get an idea of the sorts of things that belong in here by looking at the CC65/libsrc/kim1 directory.

# Future Development

- There are other Neo6502 API functions that should be added (e.g. graphics and joypad support)
- Neo6502Lib can be turned into a CC65 library if compile times become an issue
- Other functions can be added; I welcome your ideas

# Contact

I'm on the Neo6502 Discord as Gollan.
