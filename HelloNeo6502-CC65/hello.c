/*-------------------------------------------------------------+
| Hello Neo6502 World! C example for the Neo6502
| Pete Wyspianski (AKA Gollan)
| June 2024
| Ottawa, Canada
|
| Build with CC65. See Readme for details.
|
| Sept. 2025 Updated for changes to build system and API lib:
|  * Use GetInkPaper() and SetInkPaper() to change the text
|    and background colours instead of putchar(). It is faster
|    and clearer as the values being used are the actual colours
|    (0-15).
|  * Switched a few getchar() calls to KeyWait() to avoid issues
|    with characters being echoed.
+--------------------------------------------------------------*/


// CC65 C libs
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>


// Neo6502 Kernel API convenience macros
#include <neo6502.h>
#include "neo6502apilib.h"


#define cc_CLS 12
#define MAXSTRLEN 128
#define MAX_SOUNDS 24
#define ERROR_SOUND_EFFECT API_SFX_LASER_SHORT

const char* helloNeo = "Hello Neo6502 World!";
uint8_t orig_fg=0;
uint8_t orig_bg=0;

void ShowSoundMenuItem(unsigned int n, unsigned int xOffset, unsigned int yOffset, bool select)
{
  // These are padded out to 7 characters to simplify highlighting.
  const char* smenu[] = {
   "Pos    ","Neg    ","Error  ","Confirm","Reject ","Sweep  ",
   "Coin   ","LaserL ","Powerup","Victory","defeat ","fanfare",
   "Alarm1 ","Alarm2 ","Alarm3 ","Ring1  ","Ring2  ","Ring3  ",
   "Danger ","ExplodL","ExplodM","ExplodS","LaserM ","LaserS "
  };
  unsigned int n_smenu = (sizeof(smenu) / sizeof(const char*));
  unsigned int row = 0;
  unsigned int col = 0;

  unsigned char selTextColour = orig_bg;
  unsigned char selBGColour = orig_fg; 

    if (select) {
      // set select colours
      SetInkPaperColour(selTextColour,selBGColour);
    }
    else {
      // set default colours
      SetInkPaperColour(orig_fg,orig_bg);
    }
  col = n%6;
  row = n/6;
  // print the sound number in the top left corner:
  SetCursorPos(col*8+xOffset+1, row*3+yOffset+1);
  printf("%02u     ", n+1); // must print the whole cell for hilighting to work
  // print the sound name one line down in the cell:
  SetCursorPos(col*8+xOffset+1, row*3+yOffset+2);
  printf("%s", smenu[n]); // smenu[] entries fill the entire cell

  // restore default colours for further printing
  SetInkPaperColour(orig_fg,orig_bg);
}

void DoSoundMenu(void)
{
  const char* border   = "+-------+-------+-------+-------+-------+-------+";
  const char* bodygrid = "|       |       |       |       |       |       |";
  
  unsigned int i=0;

  unsigned int xOffset = 2; // Grid is 2 from left
  unsigned int yOffset = 5; // Grid is 5 from top line
  char ch;
  unsigned int iSelect = 0;
  unsigned int iSelectRow = 0;
  unsigned int iSelectCol = 0;
  
  putchar(cc_CLS);
  HideCursor(); 
  SetCursorPos(16, 0);
  printf("%s",helloNeo);
  SetCursorPos(17, 2);
  puts("Neo6502 Sound Menu");

  // print the grid
  
  for (i = 0; i < 4; i++) {
    SetCursorPos(xOffset, i*3+yOffset);
    printf("%s\n", border); // top border
    SetCursorPos(xOffset, i*3 + 1 + yOffset);
    printf("%s\n", bodygrid); // grid
    SetCursorPos(xOffset, i * 3 + 2 + yOffset);
    printf("%s\n", bodygrid); // grid
  }
  SetCursorPos(xOffset,i*3 + yOffset);
  printf("%s\n", border); // bottom border
  
  for (i = 0; i < MAX_SOUNDS; i++) {
    ShowSoundMenuItem(i, xOffset, yOffset,false);
  }
    
  // Show the top left item selected:
  iSelect=0;
  iSelectRow=0;
  iSelectCol=0;

  ShowSoundMenuItem(iSelect, xOffset, yOffset, true);

  SetCursorPos(0,20); // just a guess.. below the screen
  
  printf("  Use the arrow keys to move the selection.\n");
  printf("  Press ENTER to play the selected sound.\n");
  printf("  Press 'X' to return to the main menu.\n");
  HideCursor();

  do {
    // Our getchar() echos, so just blank out any visible character:
    SetCursorPos(0,23);
    ch = getchar();
    SetCursorPos(0,23);
    printf(" \n"); // Clears any visible char
    
    // process user input:
    switch (ch) {
      case RIGHT_ARROW_KEY:
        iSelectCol = iSelectCol+1;
        if (iSelectCol > 5) {
          iSelectCol = 0; // wrap around
        }
        ShowSoundMenuItem(iSelect, xOffset, yOffset, false); // deselect current item
        iSelect = iSelectRow*6+iSelectCol;
        ShowSoundMenuItem(iSelect, xOffset, yOffset, true); // select new item
      break;
    case LEFT_ARROW_KEY: 
      if (iSelectCol == 0) {
        iSelectCol = 5; // wrap around
      }
      else {
        iSelectCol = iSelectCol-1;
      }
      ShowSoundMenuItem(iSelect, xOffset, yOffset, false); // deselect current item
      iSelect = iSelectRow*6+iSelectCol;
      ShowSoundMenuItem(iSelect, xOffset, yOffset, true); // select new item
      break;
    case UP_ARROW_KEY:
      if (iSelectRow == 0) {
        iSelectRow=3; // wrap around
      }
      else {
        iSelectRow=iSelectRow-1;
      }
      ShowSoundMenuItem(iSelect, xOffset, yOffset, false); // deselect current item
      iSelect = iSelectRow*6+iSelectCol;
      ShowSoundMenuItem(iSelect, xOffset, yOffset, true); // select new item
      break;
    case DOWN_ARROW_KEY:
      iSelectRow = iSelectRow+1;
      if (iSelectRow > 3) {
        iSelectRow = 0; // wrap around
      }
      ShowSoundMenuItem(iSelect, xOffset, yOffset, false); // deselect current item
      iSelect = iSelectRow*6+iSelectCol;
      ShowSoundMenuItem(iSelect, xOffset, yOffset, true); // select new item
      break;
    case ENTER_KEY: 
      PlaySoundEffect(API_SOUND_CH_00, iSelect);  // Play selected sound!
      break;
    }
  } while (ch != 'x' && ch != 'X');
}

void DoLuckyNumber(void)
{
  char ch;
  char s[MAXSTRLEN];
  int t=0;      // The temperature in some measure of degrees
  uint16_t r=0; // The LUCKY NUMBER!!!

  putchar(cc_CLS);
  SetCursorPos(16, 0);
  printf("%s",helloNeo);

  SetCursorPos(12, 4);
  printf("Your Lucky Number for Today!");
  SetCursorPos(0, 6);
  printf("  To personalize your LUCKY NUMBER, please enter\n");
  printf("  the approximate temperature near your Neo6502\n");
  printf("  or enter 'X' to return to the main menu.\n\n");
  printf("  Temperature?"); // This kind of parks the cursor
  //HideCursor();

  // Old school input here.
  s[0] = (char) NULL;
  do {
    ch=KeyWait(); // Does not echo
    if (isdigit(ch)) {
      strncat(s, &ch, 1);
      putchar(ch); // echo digits
    }
    else {
      switch (ch) {
      case '-':
        if (strlen(s)==0) {
          strncat(s, &ch, 1);
          putchar(ch); // echo the negative sign
        }
        else {
          // negative sign, but not at the beginning:
          PlaySoundEffect(API_SOUND_CH_00,ERROR_SOUND_EFFECT);
        }
        break;
      case '\n':
      case 'x':
      case 'X':
        break;
      default:
        PlaySoundEffect(API_SOUND_CH_00, ERROR_SOUND_EFFECT);
        break;
      }
    }

  } while (ch != 'x' && ch != 'X' && ch != '\n');

  if (ch == 'x' || ch == 'X') return;

  t=atoi(s);
  printf("  You temperature is %d degrees.\n\n",t);
  
  printf("  Temperature Units\n");
  printf("  K)elvin\n");
  printf("  F)ahrenheit\n");
  printf("  C)elsius\n");
  printf("  C)entigrade\n");
  printf("  X) to return to the main menu\n");
  printf("  -----------------------------\n");
  printf("  Enter K F C or X?");
  do {
    ch=toupper(getchar());
    switch (ch) {
      case 'X':
        break;
      case 'K':
        break;
      case 'F':
        break;
      case 'C':
        break;
      default:
      
        PlaySoundEffect(API_SOUND_CH_00, ERROR_SOUND_EFFECT);
        break;
    }
  } while (ch != 'K' && ch != 'F' && ch != 'C' && ch != 'X');
  
  if(ch == 'X') return;
  
  printf("\n\n");

  if (ch != 'F') {
    printf("  Great choice of temperature units!\n\n");
  }

  if (ch == 'K') {
    if (t > 300) {
      printf("  Hopefully, you can get cool.\n");
    }
    if (t < 270) {
      printf("  Maybe Neo6502 can keep you warm!\n");
    }
  }

  if (ch == 'F') {
    if (t>-40 && t<32) {
      printf("  Seems chilly!\n");
    }
    if (t>100) {
      printf("  Wow! That is hot.\n");
    }
    if (t<=-40) {
      printf("  Maybe playing Neo6502 games will\n");
      printf("  help you stay warm.\n");
    }
  }

  if (ch == 'C') {
    if (t>32) {
      printf("  Very hot!\n");
    }
    if (t >= -10 && t <= 5) {
      printf("  T-shirt weather in Canada!\n");
    }
  }
  // Generate the random number and personalize it.
  r=apiRand16();
  if (r%10 >= 5) {
    r=r-t;
  }
  else {
    r=r+t;
  }

  printf("\n  Your personalized LUCKY NUMBER is...%u\n\n",r);
  
  printf("  Enter 'X' to return to the main menu?");
  ch=getchar();

}

void DoCharMenu(void)
{
  // These are for both ink/foreground (0x80-0x8F)
  // and paper/background (0x90-0x9F).
  const char* scolour[] = {
    "Black-trsp",
    "Red",
    "Green",
    "Yellow",
    "Blue",
    "Magenta",
    "Cyan",
    "White",
    "Black",
    "Dark grey",
    "Dark green",
    "Orange",
    "Dark orange",
    "Brown",
    "Pink",
    "Light grey"
  };

  char ch;
  unsigned int i=0;
  uint8_t fg=0;
  uint8_t bg=0;

  putchar(cc_CLS);
  SetCursorPos(16, 0);
  printf("%s", helloNeo);
    
  SetCursorPos(16,2);
  printf("%cN%ce%co%c6%c5%c0%c2%c T%ce%cx%ct%c C%co%cl%co%cu%cr%cs%c",INK_RED,INK_ORANGE,INK_YELLOW,INK_GREEN,INK_CYAN,INK_MAGENTA,
    INK_RED, INK_ORANGE, INK_YELLOW, INK_GREEN, INK_CYAN, INK_MAGENTA, INK_RED, INK_ORANGE, INK_YELLOW, INK_GREEN, INK_CYAN, INK_MAGENTA,
    INK_RED);
  
 // show off the text colours
  SetCursorPos(0, 5);
  printf("%c%c                        ",INK_BLACK, PAPER_RED);
  SetCursorPos(0, 6);
  printf("%c%c Foreground/Ink Colours ",INK_BLACK,PAPER_RED);
  SetCursorPos(0, 7);
  printf("%c%c                        ", INK_BLACK, PAPER_RED);

  for (i=0; i<=0x0F; i++) {
    SetCursorPos(0, 9+i);
    if (i+0x80 == INK_BLACK_TRANSP || i+0x80 == INK_BLACK || i+0x80 == INK_DARK_GREY || i+0x80 == INK_BLUE) {
      // special handling for dark colours: set bg to white
      printf("%c%c %s ink colour ", i+0x80, PAPER_WHITE, scolour[i]);
    }
    else {
      // Use a black background for most colours
      printf("%c%c %s ink colour ", i+0x80,PAPER_BLACK, scolour[i]);
    }
  }

  // show off the background colours
  SetCursorPos(26, 5);
  printf("%c%c                          ", INK_RED, PAPER_WHITE);
  SetCursorPos(26, 6);
  printf("%c%c Background/Paper Colours ", INK_RED, PAPER_WHITE);
  SetCursorPos(26, 7);
  printf("%c%c                          ", INK_RED, PAPER_WHITE);
  
  for (i=0; i<=0x0F; i++) {
    SetCursorPos(26, 9+i);
    if (i+0x90 == PAPER_BLACK_TRANSP || i+0x90 == PAPER_BLACK || i+0x90 == PAPER_DARK_GREY || i+0x90 == PAPER_BLUE) {
      // special handling for dark background colours: set fg to white
      printf("%c%c %s paper colour ",INK_WHITE,i+0x90,scolour[i]);
    }
    else {
      // Use a black foreground for most colours
      printf("%c%c %s paper colour ", INK_BLACK, i+0x90,  scolour[i]);
    }
  }

  printf("\n\n");
   
  // Restore original colours:
  SetInkPaperColour(orig_fg, orig_bg);
  SetCursorPos(0, 27);
  // We don't check what key they press... we just don't want to say "any key"
  // because on the emulator, pressing ESC is bad.
  printf("Enter 'X' to return to the main menu?");
  ch=KeyWait();
}

void ShowMainMenu()
{
  const char* menu[] = {
    "S)ee your lucky number for today",
    "L)isten to Neo6502 sound effects",
    "V)iew Neo6502 text colours      ",
    "Q)uit                           "
    };
  
  const char* border = "+---------------------------------+";

  unsigned int n_menu = (sizeof(menu)/sizeof(const char*));
  unsigned int i = 0;

  putchar(cc_CLS);
  SetCursorPos(16, 0);
  printf("%s", helloNeo);

   // Centre the menu vertically a bit.
  SetCursorPos(0,3);

  for (i = 0; i < n_menu; i++) {
    // 9 spaces before
    printf("        %s\n", border);
    printf("        |%s |\n",menu[i]);
    }
    printf("        %s\n\n", border);
 
 // Park the cursor at the bottom left edge of the menu.
    SetCursorPos(8,12);
    HideCursor();
}

int main()
{
  unsigned int i = 0;
  unsigned char ch;

  // save these so we can put them back at the end:
  GetInkPaperColour(&orig_fg, &orig_bg);

  ShowMainMenu();

  do {
      ch=toupper(getchar());
    // switch statement goes here
    switch (ch) {
      case 'S':
        DoLuckyNumber();
        break;
      case 'L':
        DoSoundMenu();
        break;
      case 'V':
        DoCharMenu();
        break;
    }
    ShowMainMenu(); // redisplay this menu
  } while (ch != 'Q');
  SetCursorPos(0,14); // just a guess.. somewhere below the menu
  puts("The end.");
  ShowCursor();
  return 0 ;
}
