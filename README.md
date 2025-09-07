**Updated September 2025**

This update includes:
- Fixes for internal changes to CC65
- The C runtime code uses a more reliable method to return to the Neo6502 BASIC command prompt
- An improved build system (libraries no longer have to be copied to the project directory)
- All standard C functions in the C runtime library "none.lib"
- Additional functions in the Neo6502 API library "api.lib"
- Documentation for the API library and the "none" library

# The Neo6502 Development Environment

**Include files are kept in the "Neo6502inc" directory.**

**neo6502.h**  Definitions and macros for Neo6502 API access from C programs. It would be included in any *.c source file that uses Neo6502 API or calls API helper functions.

**neo6502.asm.inc** Definitions and macros for NEO6502 API access from CC65 assembly language programs.

**neo6502apilib.h** This has definitions for the Neo6502 API access functions in the Neo6502 API library, "api.lib". The file also includes convenience definitions. For example, apiKeyWait() can also be called with KeyWait(). Another example is that apiGetInkPaperColour() can be used as GetInkPaperColour() and GetInkPaperColor().



# Contents
- Neo6502_nonelib: a library to connect CC65 functions to the Neo6502 API
- HelloNeo6502: demonstrates Neo6502 features using CC65
- Test-CC65-IO: tests and demonstrates CC65 console I/O functions on Neo6502
- See Readme.md in the directory for details on how to build.

## HelloNeo6502-CC65
**This is a sample CC65 project that:**
- Includes the headers and libraries required to use CC65 for Neo6502
- Shows how to build CC65 projects for Neo6502 (build.bat/buildrun.bat, and build.sh/buildrun.sh)
- See Readme.md in the directory for details on how to build.

## Test-CC65-IO
**This is a sample CC65 project that:**
- Is a demonstration/test of the Neo6502 console and file I/O functions for CC65
- Shows how to build CC65 projects for Neo6502 (build.bat/buildrun.bat, and build.sh/buildrun.sh)
- To start your own project, consider copying this directory and editing the name of the *.c and *.nee and *.neo files.
- Note: as of this writing, Test 13 fails on the Linux version of the emulator

## Neo6502_nonelib
**This is a custom library that connects CC65 functions to the Neo6502 API (hardware):**
- Creates none.lib, which is used in CC65 projects for the "none target" (-t none) where the device is not officially supported
- Includes the headers and functions required to use CC65 for Neo6502
- See Readme.md in the directory for details on how to build.

## Neo6502_libs
**Contains libraries that connect CC65 programs to the Neo6502 API**
- Includes the headers and functions required to use CC65 for Neo6502
- See Readme.md in the directory for the contents of the library


### Neo6502.inc

**Contains header files for Neo6502 development and the use of libraries**

**neo6502.h**
Note that this file is different from a previous version with the same name. Provides definitions and macros for Neo6502 API access for C functions. It would be included
in any *.c source file that uses Neo6502 API or calls API helper functions.

**neo6502.asm.inc**
Note that this file is different from a previous version with the same name. Provides definitions and macros for Neo6502 API access for 65C02 assembly language functions. It would be
included in any *.s source file that uses Neo6502 API or calls API helper functions.


**neo6502lib.h**
Use this with "api.lib". See the readme.md file in Neo6502_libs for details.

# Origins and Changes

This project started with "examples/C" and "examples/semi-working-cc65". I replaced the _write() function and added a _read() function by adapting the coresponding functions from from the kim1 library in CC65. I modified crt0 to return to NeoBASIC on exit. To make the neo6502.h file work with multi-source projects, I changed the API access method to use macros instead of defining pointer variables in the header. Finally, I changed the definition of the API access registers from uint16_t to unit8_t.

Neo6502 is not an official target in CC65. This is only a minor issue. When we build CC65 projects for the Neo6502, we have to set a target of "none". This causes the CC65 build tools to look for C runtime functions (e.g. _write, _read, crt0, etc.) in "none.lib". To put the Neo6502 specific versions of these functions into "none.lib", we keep our own custom version of "none.lib" in the "nonelib" directory. This directory has all the components and build scripts (buildlib.bat/buildlib.sh) to update our verion of none.lib. This library is linked into the project during the build process. It does not have to be copied to the project directory.

# Future Development

- I have a substantial portion of a graphics API library completed and will be releasing it soon
- A demo program to go with the graphics API library will also be released

# Contact

I welcome your questions and suggestions. I'm on the Neo6502 Discord as Gollan.
