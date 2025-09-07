# Test-CC65-IO

This program tests and demonstrates the many C language file I/O functions that are provided by the "none.lib". 

# To Build
The project is built either using build.bat (or buildrun.bat) files or build.sh files. These files must first be edited to add the indicated paths to CC65 and to the latest Neo6502 build tools and libraries. A project also needs a CC65 linker config file. "Use neo6502.cfg"in the sample directory.

The project produces an executable file "Test-CC65-IO" and copies it to the "storage" directory. To run this file in the emulator from the command line use:

`<path to emulator>neo.exe keys "Test-CC65-IO" cold`

To run the program from NeoBASIC (emulator or NEO6502):

`run "Test-CC65-IOo"`

The emulator loads files from the "storage" director. The Neo6502 hardware loads files from the SD card.

# The Neo6502 Development Environment

**neo6502.h** Include files are kept in the "Neo6502inc" directory. This file provides definitions and macros for Neo6502 API access. It would be included in any *.c source file that uses Neo6502 API or calls API helper functions.

**neo6502apilib.h** This has definitions for the Neo6502 API access functions in the Neo6502 API library, "api.lib". The file also includes convenience definitions. For example, apiKeyWait() can also be called with KeyWait(). Another example is that apiGetInkPaperColour() can be used as GetInkPaperColour() and GetInkPaperColor().

# Origins and Changes

This project started with "examples/C" and "examples/semi-working-cc65". I replaced the _write() function and added a _read() function by adapting the coresponding functions from from the kim1 library in CC65. I modified crt0 to return to NeoBASIC on exit. To make the neo6502.h file work with multi-source projects, I changed the API access method to use macros instead of defining pointer variables in the header. Finally, I changed the definition of the API access registers from uint16_t to unit8_t.

Neo6502 is not an official target in CC65. This is only a minor issue. When we build CC65 projects for the Neo6502, we have to set a target of "none". This causes the CC65 build tools to look for C runtime functions (e.g. _write, _read, crt0, etc.) in "none.lib". To put the Neo6502 specific versions of these functions into "none.lib", we keep our own custom version of "none.lib" in the "nonelib" directory. This directory has all the components and build scripts (buildlib.bat/buildlib.sh) to update our verion of none.lib. This library is linked into the project during the build process. It does not have to be copied to the project directory.

# Future Development

- I have a substantial portion of a graphics API library completed and will be releasing it soon
- A demo program to go with the graphics API library will also be released

# Contact

I welcome your questions and suggestions. I'm on the Neo6502 Discord as Gollan.


