/*---------------------------------------------------+
| Neo6502APILib.h
|
| Neo6502 API and Utility Functions.
+----------------------------------------------------*/

#define NEO_API_RAND16_MAX 0xFFFF

uint16_t NeoAPIRand16(void);
uint16_t GetSysTimer16(void);
unsigned long GetSysTimer32(void);
void PlaySoundEffect(uint8_t sound_ch, uint8_t sound_effect);
void SetCursorPosition(uint8_t x_pos, uint8_t y_pos);
void GetInkPaperColour(uint8_t* fg, uint8_t* bg);
void HideCursor(void);
void ShowCursor(void);

