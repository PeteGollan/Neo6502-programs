/*\
|*| Convenience macros for Neo6502 applications programming
|*| SPDX-License-Identifier: CC0-1.0
\*/


/* Neo6502 Kernel API control addresses */

#define ControlPort 0xFF00
#define API_COMMAND_ADDR 0xFF00
#define API_FUNCTION_ADDR 0xFF01
#define API_PARAMETERS_ADDR 0xFF04

/* Neo6502 Kernel API control addresse macros */

#define WRITE_API_COMMAND_ADDR(b) ((*(volatile uint8_t *)API_COMMAND_ADDR) = (b))
#define WRITE_API_FUNCTION_ADDR(b) ((*(volatile uint8_t *)API_FUNCTION_ADDR) = (b))

// Parameter macros: first argument is which paramter: 0, 1, etc.
#define READ_API_PARAMETERS_ADDR(offset) (*(volatile uint8_t *)(API_PARAMETERS_ADDR+offset))
#define WRITE_API_PARAMETERS_ADDR(offset,b) ((*(volatile uint8_t *)(API_PARAMETERS_ADDR+offset)) = (b))


/* Neo6502 Kernel API control codes (see api.pdf) */

/*\
|*| System functions (Group 1)
\*/
// API function group
#define API_GROUP_SYSTEM    0x01
// API functions
#define API_FN_TIMER        0x01 
#define API_FN_KEY_STATUS   0x02
#define API_FN_SYSTEM_RESET 0x07

/*\
|*| Console functions (Group 2)
\*/
// API function group
#define API_GROUP_CONSOLE      0x02 
// API functions
#define API_FN_READ_CHAR            0x01
#define API_FN_CONSOLE_STATUS       0x02
#define API_FN_READ_LINE            0x03
#define API_FN_DEFINE_HOTKEY        0x04
#define API_FN_DEFINE_CHAR          0x05
#define API_FN_WRITE_CHAR           0x06
#define API_FN_SET_CURSOR_POS       0x07
#define API_FN_LIST_HOTKEYS         0x08
#define API_FN_SCREEN_SIZE          0x09
#define API_FN_INSERT_LINE          0x0A
#define API_FN_DELETE_LINE          0x0B
#define API_FN_CLEAR_SCREEN         0x0C
#define API_FN_GET_CURSOR_POS       0x0D
#define API_FN_CLEAR_REGIION        0x0E
#define API_FN_SET_TEXT_COLOR       0x0F
#define API_FN_CURSOR_INVERSE       0x10
#define API_FN_GET_INK_PAPER_COLOUR 0x12
#define API_FN_SHOW_HIDE_CURSOR     0x13


/*\
|*| Mathematics functions (Group 4)
\*/
#define API_GROUP_MATH 0x04         // API function group  
#define API_FN_RANDOM_INTEGER 0x1C  // API function 28

/*\
|*| Sound functions (Group 8)
\*/
// API function group
#define API_GROUP_SOUND   0x08
// API function
#define API_FN_PLAY_SOUND 0x05  
// API parameter (channel index)
#define API_SOUND_CH_00   0x00  

// Sound parameters(Group 8, Function 5)
// API parameter (sound effect)
#define API_SFX_POSITIVE     0x00
#define API_SFX_NEGATIVE     0x01
#define API_SFX_ERROR        0x02
#define API_SFX_CONFIRM      0x03
#define API_SFX_REJECT       0x04
#define API_SFX_SWEEP        0x05
#define API_SFX_COIN         0x06
#define API_SFX_LASER_LONG   0x07
#define API_SFX_POWERUP      0x08
#define API_SFX_VICTORY      0x09
#define API_SFX_DEFEAT       0x0A
#define API_SFX_FANFARE      0x0B
#define API_SFX_ALARM1       0x0C
#define API_SFX_ALARM2       0x0D
#define API_SFX_ALARM3       0x0E
#define API_SFX_RING1        0x0F
#define API_SFX_RING2        0x10
#define API_SFX_RING3        0x11
#define API_SFX_DANGER       0x12
#define API_SFX_EXPL_LONG    0x13
#define API_SFX_EXPL_MEDIUM  0x14
#define API_SFX_EXPL_SHORT   0x15
#define API_SFX_LASER_MEDIUM 0x16
#define API_SFX_LASER_SHORT  0x17


/*-----------------------------------------
| Key values returned from getchar()
| Note: these are in hexidecimal
+-----------------------------------------*/
#define LEFT_ARROW_KEY  0x01
#define RIGHT_ARROW_KEY 0x04
#define DOWN_ARROW_KEY  0x13
#define UP_ARROW_KEY    0x17
#define ENTER_KEY       0x0A

/*-----------------------------------------
| Foreground (ink) colours 0x80-0x8F 
| Note: these are in hexidecimal
+-----------------------------------------*/

#define INK_BLACK_TRANSP 0x80
#define INK_RED          0x81
#define INK_GREEN        0x82
#define INK_YELLOW       0x83
#define INK_BLUE         0x84
#define INK_MAGENTA      0x85
#define INK_CYAN         0x86
#define INK_WHITE        0x87
#define INK_BLACK        0x88
#define INK_DARK_GREY    0x89
#define INK_DARK_GREEN   0x8A
#define INK_ORANGE       0x8B
#define INK_DARK_ORANGE  0x8C
#define INK_BROWN        0x8D
#define INK_PINK         0x8E
#define INK_LIGHT_GREY   0x8F

/*-----------------------------------------
| Background (paper) colours 0x90-0x9F
| Note: these are in hexidecimal
+-----------------------------------------*/

#define PAPER_BLACK_TRANSP 0x90
#define PAPER_RED          0x91
#define PAPER_GREEN        0x92
#define PAPER_YELLOW       0x93
#define PAPER_BLUE         0x94
#define PAPER_MAGENTA      0x95
#define PAPER_CYAN         0x96
#define PAPER_WHITE        0x97
#define PAPER_BLACK        0x98
#define PAPER_DARK_GREY    0x99
#define PAPER_DARK_GREEN   0x9A
#define PAPER_ORANGE       0x9B
#define PAPER_DARK_ORANGE  0x9C
#define PAPER_BROWN        0x9D
#define PAPER_PINK         0x9E
#define PAPER_LIGHT_GREY   0x9F
