/*---------------------------------------------------+
| Neo6502APILib.c
| 
| Neo6502 Library Functions.
|
| Compilation of existing functions and new functions.
| These were updated from existing, unattributed examples:
|   void PlaySoundEffect(uint8_t sound_ch, uint8_t sound_effect);
|   void SetCursorPosition(uint8_t x_pos, uint8_t y_pos);
|
| Remaining functions added by:
| Pete Wyspianski (AKA Gollan)
| June 2024, Ottawa Canada
+----------------------------------------------------*/


// CC65 C libs
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

#include "neo6502.h"


/*---------------------------------------------------------
 | NeoAPIRandRand16
 |
 |
 | Returns a 16-bit random integer in the range 0-65535,
 | inclusive, using the Neo6502 API Random Integer function.
 +---------------------------------------------------------*/
uint16_t NeoAPIRand16(void)
{
  uint8_t Reg1[20]; // Register1 and Register2
  uint16_t Reg1Addr = (uint16_t)Reg1;

  // set max range of requested random integer... 5 bytes, every second address
  Reg1[0] = 0x00;   // type: 32-bit integer, little endian
  Reg1[2] = 0x00;   // byte0 (lsb) of max range+2 $ffff+1/65535+1 (only asking for 16-bits)
  Reg1[4] = 0x00;   // byte1 of max range
  Reg1[6] = 0x01;   // byte2 of max range
  Reg1[8] = 0x00;   // byte3 (msb) of max range

  WRITE_API_PARAMETERS_ADDR(0, (uint8_t)(Reg1Addr & 0xFF)); // LSB
  WRITE_API_PARAMETERS_ADDR(1, (uint8_t)(Reg1Addr >> 8)); // MSB
  WRITE_API_PARAMETERS_ADDR(2, 0x02); // Step between bytes of each register

  WRITE_API_FUNCTION_ADDR(API_FN_RANDOM_INTEGER);
  WRITE_API_COMMAND_ADDR(API_GROUP_MATH);

  return Reg1[2] + ((unsigned int)Reg1[4])<<8;
}


/*-----------------------------------------------+
| GetSysTimer16()
|
| Returns the lowest 16-bits of the
| 100hz system timer
+-----------------------------------------------*/
uint16_t GetSysTimer16(void)
{
  WRITE_API_FUNCTION_ADDR(API_FN_TIMER);
  WRITE_API_COMMAND_ADDR(API_GROUP_SYSTEM);
   
  return (uint16_t)READ_API_PARAMETERS_ADDR(0) + (uint16_t)READ_API_PARAMETERS_ADDR(1) << 8;
 }


/*-----------------------------------------------+
| GetSysTimer32()
|
| Returns the entire 32-bits of the
| 100hz system timer
+-----------------------------------------------*/
unsigned long GetSysTimer32(void)
{
  WRITE_API_FUNCTION_ADDR(API_FN_TIMER);
  WRITE_API_COMMAND_ADDR(API_GROUP_SYSTEM);

  return (unsigned long)READ_API_PARAMETERS_ADDR(0) + ((unsigned long)READ_API_PARAMETERS_ADDR(1) << 8);
  +((unsigned long)READ_API_PARAMETERS_ADDR(2) << 16) + ((unsigned long)READ_API_PARAMETERS_ADDR(3) << 24);
}


/*-----------------------------------------------+
| PlaySoundEffect(sound_ch, sound_effect)
+-----------------------------------------------*/
void PlaySoundEffect(uint8_t sound_ch, uint8_t sound_effect)
{
  WRITE_API_FUNCTION_ADDR(API_FN_PLAY_SOUND);
  WRITE_API_PARAMETERS_ADDR(0, sound_ch);
  WRITE_API_PARAMETERS_ADDR(1, sound_effect);
  WRITE_API_COMMAND_ADDR(API_GROUP_SOUND);
}


/*-----------------------------------------------+
| SetCursorPosition(x,y)
+-----------------------------------------------*/
void SetCursorPosition(uint8_t x_pos, uint8_t y_pos)
{
  WRITE_API_FUNCTION_ADDR(API_FN_SET_CURSOR_POS);
  WRITE_API_PARAMETERS_ADDR(0, x_pos);
  WRITE_API_PARAMETERS_ADDR(1, y_pos);
  WRITE_API_COMMAND_ADDR(API_GROUP_CONSOLE);
}

/*-----------------------------------------------+
| GetInkPaperColour()
|
| Returns the ink (foreground) colour and
| the paper (background) colours.
| Note: these are the actual character values
| to print to get the colours.
+-----------------------------------------------*/
void GetInkPaperColour(uint8_t* fg, uint8_t* bg)
{
  WRITE_API_FUNCTION_ADDR(API_FN_GET_INK_PAPER_COLOUR);
  WRITE_API_COMMAND_ADDR(API_GROUP_CONSOLE);
  *fg = READ_API_PARAMETERS_ADDR(0) + 0x80;
  *bg = READ_API_PARAMETERS_ADDR(1) + 0x90;
}

/*-----------------------------------------------+
| HideCursor()
|
| Note:
|  Cursor will be restored after a CLEAR SCREEN
+-----------------------------------------------*/
void HideCursor(void)
{
  WRITE_API_FUNCTION_ADDR(API_FN_SHOW_HIDE_CURSOR);
  WRITE_API_PARAMETERS_ADDR(0, 0); // hide
  WRITE_API_COMMAND_ADDR(API_GROUP_CONSOLE);
}

/*-----------------------------------------------+
| ShowCursor()
+---------_--------------------------------------*/
void ShowCursor(void)
{
  WRITE_API_FUNCTION_ADDR(API_FN_SHOW_HIDE_CURSOR);
  WRITE_API_PARAMETERS_ADDR(0, 1); // show
  WRITE_API_COMMAND_ADDR(API_GROUP_CONSOLE);
}

