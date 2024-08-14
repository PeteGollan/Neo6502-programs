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
#define API_GROUP_SYSTEM     1
// API functions
#define API_FN_TIMER              1
#define API_FN_KEY_STATUS         2
#define API_FN_BASIC              3
#define API_FN_CREDITS            4
#define API_FN_SERIAL_STATUS      5
#define API_FN_LOCALE             6
#define API_FN_SYSTEM RESET       7
#define API_FN_MOS                8
// 9 omitted
#define API_FN_DEBUG_WRITE_CHAR  10
#define API_FN_API_VERS          11

/*\
|*| Console functions (Group 2)
\*/
// API function group
#define API_GROUP_CONSOLE    2 

// API functions
#define API_FN_READ_CHAR             1
#define API_FN_CONSOLE_STATUS        2
#define API_FN_READ_LINE             3
#define API_FN_DEFINE_HOTKEY         4
#define API_FN_DEFINE_CHAR           5
#define API_FN_WRITE_CHAR            6
#define API_FN_SET_CURSOR_POS        7
#define API_FN_LIST_HOTKEYS          8
#define API_FN_SCREEN_SIZE           9
#define API_FN_INSERT_LINE          10
#define API_FN_DELETE_LINE          11
#define API_FN_CLEAR_SCREEN         12
#define API_FN_GET_CURSOR_POS       13
#define API_FN_CLEAR_REGIION        14
#define API_FN_SET_TEXT_COLOUR      15
#define API_FN_SET_TEXT_COLOR       15
#define API_FN_CURSOR_INVERSE       16
// 17 omitted
#define API_FN_GET_INK_PAPER_COLOUR 18
#define API_FN_GET_INK_PAPER_COLOR  18
#define API_FN_SHOW_HIDE_CURSOR     19

/*\
|*| File I/O functions (Group 3)
\*/

// API function group
#define API_GROUP_FILEIO     3

// API functions
#define API_FN_LIST_DIRECTORY   1
#define API_FN_LOAD_FILENAME    2
#define API_FN_STORE_FILENAME   3
#define API_FN_FILE_OPEN        4
#define API_FN_FILE_CLOSE       5
#define API_FN_FILE_SEEK        6
#define API_FN_FILE_TELL        7
#define API_FN_FILE_READ        8
#define API_FN_FILE_WRITE       9
#define API_FN_FILE_SIZE       10
#define API_FN_FILE_SET_SIZE   11
#define API_FN_FILE_RENAME     12
#define API_FN_FILE_DELETE     13
#define API_FN_DIR_MKDIR       14
#define API_FN_DIR_CHDIR       15
#define API_FN_FILE_STAT       16
#define API_FN_DIR_OPEN        17
#define API_FN_DIR_READ        18
#define API_FN_DIR_CLOSE       19
#define API_FN_FILE_COPY       20
#define API_FN_FILE_SET_ATTRIB 21
#define API_FN_LIST_FILTERED   32

// API_FN_FILE_OPEN Paramaters:
#define API_FILE_OPEN_MODE_READ_ONLY    0
#define API_FILE_OPEN_MODE_WRITE_ONLY   1
#define API_FILE_OPEN_MODE_READ_WRITE   2
#define API_FILE_OPEN_MODE_RW_OVERWRITE 3

// API_FN_FILE_OPEN Paramaters (official):
#define FIOMODE_RDONLY         0
#define FIOMODE_WRONLY         1
#define FIOMODE_RDWR           2
#define FIOMODE_RDWR_CREATE    3 

#define FIO_NUM_FILES          8

// API_FN_FILE Error Codes:
#define FIOERROR_OK                   0
#define FIOERROR_UNKNOWN              1
#define FIOERROR_FILE_NOT_FOUND       2
#define FIOERROR_FILE_EXISTS          3
#define FIOERROR_IS_A_DIRECTORY       4
#define FIOERROR_NOT_A_DIRECTORY      5
#define FIOERROR_OUT_OF_DISK_SPACE    6
#define FIOERROR_DISK_ERROR           7
#define FIOERROR_ACCESS_DENIED        8
#define FIOERROR_INVALID_PARAMETER    9
#define FIOERROR_DIRECTORY_NOT_EMPTY 10
#define FIOERROR_END_OF_DIRECTORY    11
#define FIOERROR_ALREADY_OPEN        12
#define FIOERROR_NOT_OPEN            13


/*\
|*| Mathematics functions (Group 4)
\*/
// API function group 
#define API_GROUP_MATH 4     

// API functions
#define API_FN_RANDOM_INTEGER 28

/*\
|*| Sound functions (Group 8)
\*/
// API function group
#define API_GROUP_SOUND     8

// API functions
#define API_FN_PLAY_SOUND   5  
// API parameter (channel index)
#define API_SOUND_CH_00     0  

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

// For get_morpheus_vers()
// MUST match with neo6502asm.inc
struct morpheus_vers {
  uint8_t major;
  uint8_t minor;
  uint8_t patch;
};

int __fastcall__ get_morpheus_vers(struct morpheus_vers *mv);
