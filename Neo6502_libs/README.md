# API Library for Neo6502

**An API Library for Neo6502**

This library provides Neo6502 API support functions for programs using the CC65 system. There is a separate graphics library under development.


# To Build
 The API library can be built using the "buildapilib.bat" file in the Neo6502_libs directory. This produces a file "api.lib" in the library directory. It is only necessary to build the library if something changes in the files in the "api" subdirectory.

# API Library Functions

Functions are listed with their defined name and any aliases.

**apiGetSysTimer16()** 
* Returns the two least significant bytes of the 100Hz system timer. One tick of the system timer is 10ms.

**apiDelay10ms(t)**
* Waits for the number of 10ms intervals specified in t
* Uses the 100Hz system timer function
* t=0 returns immediately
* t=100 is 1 sec, t=1000 is 10 sec
* The maximum t=65535 is 655 sec (10m 55sec)

**apiHideCursor()** / **HideCursor()**
* Hides (turns off) the system cursor. The cursor is always turned back on when the screen is cleared.

**apiShowCursor()** / **ShowCursor()**

* Shows the system cursor.

**apiKeyWait()** / **KeyWait()**

* Waits for a key to be pressed on the keyboard and returns the ASCII value of the key as an unsigned char. Does not show cursor or echo type characters (use getchar() for instead). Does not return function keys, numpad keys, or other special keys. The ENTER key is returned as $0A.


**apiRand16()**

 * Returns a 16-bit random integer in the range 0-65535 inclusive, using the Neo6502 API Random Integer function.


**apiGetInkPaperColour(\*fg,\*bg)** / **GetInkPaperColour()** / **GetInkPaperColor()**

* Takes pointers to unsigned 8-bit variables for the foreground (ink) and background (paper) colours and returns text colour and background colour values in the range 0-15. 

**apiSetInkPaperColour(fg,bg)** / **SetInkPaperColour()** / **SetInkPaperColor()**

* Sets the text and background colours. Input values are in the range 0-15. Does NOT range check the parameters.
*Actually calls the Neo6502 API SetTextColour(), but this name is used as it is more descriptive and corresponds with the "GetInkPaperColour()" function.


**apiSetCursorPos(x,y)** / **SetCursorPos()**

* Sets the text cursor position to the given x and y values. Does not range check the parameters.

**apiPlaySoundEffect(sound_ch,sound_effect)** \ **PlaySoundEffect()**

* Plays the specifed sound effect on the specified channel


# Contact

I welcome your questions and suggestions. I'm on the Neo6502 Discord as Gollan.
