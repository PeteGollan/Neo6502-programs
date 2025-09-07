/*---------------------------------------------------+
| Neo6502APILib.h
|
| Neo6502 API and Utility Functions.
+----------------------------------------------------*/

#define NEO_API_RAND16_MAX 0xFFFF

void __fastcall__ apiHideCursor(void);
void __fastcall__ apiShowCursor(void);
#define HideCursor apiHideCursor
#define ShowCursor apiShowCursor

void __fastcall__ apiDelay10ms(uint16_t t);
unsigned char __fastcall__ apiKeyWait(void); // Does NOT show cursor or echo. For cursor and echo use getchar().
#define KeyWait apiKeyWait
uint16_t __fastcall__ apiRand16(void);

void __fastcall__ apiGetInkPaperColour(uint8_t* fg, uint8_t* bg);
#define GetInkPaperColour apiGetInkPaperColour
#define GetInkPaperColor apiGetInkPaperColour

void __fastcall__ apiSetInkPaperColour(uint8_t fg, uint8_t bg);
#define SetInkPaperColour apiSetInkPaperColour
#define SetInkPaperColor apiSetInkPaperColour
#define SetTextColour apiSetInkPaperColour
#define SetTextColor apiSetInkPaperColour

void __fastcall__ apiSetCursorPos(uint8_t x_pos, uint8_t y_pos);
#define SetCursorPos apiSetCursorPos
void __fastcall__ apiPlaySoundEffect(uint8_t sound_ch, uint8_t sound_effect);
#define PlaySoundEffect apiPlaySoundEffect
uint16_t __fastcall__ apiGetSysTimer16(void);
