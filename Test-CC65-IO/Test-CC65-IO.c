/*------------------------------------
| Test-CC65-IO.c
| By Pete Wyspianski (aka Gollan)
| July 2024
+--------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h> // for directory functions
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/utsname.h>

#include <neo6502.h>

#define cc_CLS 12
#define MAXSTRLEN 128

int main(void)
{
	char s[MAXSTRLEN];
	int fr; // file return value

	unsigned char ch;
	unsigned int i;
	
	char f1_name[MAXSTRLEN];
	char f2_name[MAXSTRLEN];
	FILE *f1;
	FILE *f2;
	long ftr; // ftell() result - used in Test 2
	char vlfname[]="tabcdefghijklmnopqrstuvwxyz1abcdefghijklmnopqrstuvwxyz2abcdefghijklmnopqrstuvwxyz3abcdefghijklmnopqrstuvwxyz4abcdefghijklmnopqrstuvwxyz5abcdefghijklmnopqrstuvwxyz6abcdefghijklmnopqrstuvwxyz7abcdefghijklmnopqrstuvwxyz8abcdefghijklmnopqrstuvwxyz9abcdefghijklmnopqrstuvwxyz0abcdefghijklmnopqrstuvwxyz";
	struct utsname unameData;
	fpos_t fpos;
	struct morpheus_vers mv;

   putchar(cc_CLS);
   printf("+---------------------------+\n");
   printf("| Test Neo6502 CC65 File IO |\n");
   printf("+---------------------------+\n\n");
   //printf("%cTest Neo6502 CC65 File IO\n\n",cc_CLS);

   if (get_morpheus_vers(&mv) == 0) {

		printf("Morpheus version information:\n");
		printf("Major: %3d\n", mv.major);
    	printf("Minor: %3d\n", mv.minor);
    	printf("Patch: %3d\n", mv.patch);
    	printf("\n");
   } else {
	printf("***ERROR get_morpheus_vers() failed.\n");
	exit (-1);
   }

	printf("\nPress any key to continue...");
	getchar();
	printf("\n\n");
   
   puts("+------------------+");
   puts("| Test 1: mode 'w' |"); 
   puts("+------------------------------------------+");
   puts("| Write to write-only file and try to read |");
   puts("+------------------------------------------+\n");

   strcpy(f1_name,"alpha.txt");
   printf("Opening %s for write...\n",f1_name);
   f1=fopen(f1_name, "w");
   if (f1 == NULL) {
 		printf("***ERROR opening %s:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 	else {
 		printf("Opened.\n");
 	}

   puts("Writing to file using fputs()...");
   // 'Q' is omitted both times so that the two alphabets fit on the screen
   fr=fputs("ABCDEFGHIJKLMNOPRSTUVWXYZabcdefghijklmnoprstuvwxyz",f1);
   if (fr == -1) {
 		printf("***ERROR writing %s:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 	else {
		// Show the default seek position.
 		printf("Wrote to file, ftell()=%ld.\n",ftell(f1));
 	}

	printf("Rewinding file... ");
	rewind(f1);
	printf("ftell()=%ld.\n",ftell(f1));

	printf("Trying to read from write-only file using fgetc()...\n");
	fr=fgetc(f1);
	if (fr == EOF)	{
		printf("AS EXPECTED:\n");
		printf("  ***ERROR reading from write-only file:\n");
		printf("  %s.\n",strerror(errno));
		clearerr(f1);
	} else {
		printf("Reading from write-only file succeeded, ch='%c'.\n",(char) fr);
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
	}

	printf("Writing to file at the new positon...\n");
	printf("BEFORE rewind, ftell()=%ld.\n",ftell(f1));
    rewind(f1);
	if (fputc('X', f1) == -1) {
 		printf("***ERROR writing:%s.\n", strerror(errno));
 		exit (-1);
 	}
 	else {
		// Show the default seek position.
 		printf("Wrote data to file, ftell()=%ld.\n",ftell(f1));
 	}

	printf("Closing file...\n");
	if (fclose(f1) == -1) {
 		printf("***ERROR closing %s:%s.\n", f1_name, strerror(errno));
		exit (-1);
 	} else {
 		printf("Closed file %s.\n", f1_name);
 	}

	puts("+---------------+");
	puts("| End of Test 1 |");
    puts("+---------------+");

	printf("\nPress any key to continue...");
	getchar();
	printf("\n\n");


   puts("+-------------------+");
   puts("| Test 2: mode 'w+' |");
   puts("+--------------------------------+");
   puts("| Write and read write plus file |");
   puts("+--------------------------------+\n");

   strcpy(f1_name,"alpha.txt");
   printf("Opening %s for write plus...\n",f1_name);
   f1=fopen(f1_name, "w+");
   if (f1 == NULL) {
 		printf("***ERROR opening %s:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 	else {
 		printf("Opened.\n");
 	}
	// ***** Reported as issue #571
	// Check size of file (it should be zero)
	fseek(f1, 0, SEEK_END);
	ftr = ftell(f1);
	if (ftr == 0) {
		printf("As expected, w+ file is length zero.\n");
	} else {
		printf("*** ERROR: w+ file is length %ld. Expected 0.\n",ftr);
		printf("*** Fixed in newer Neo6502 firmware.\n");
	}
	rewind(f1);

	sprintf(s,"0123456789ABCDEFGHIJKLMNOPRSTUVW");
	printf("Writing %s to file...\n",s);
	fwrite(s,1,strlen(s),f1);

	printf("Wrote to file, ftell()=%ld.\n",ftell(f1));
	fseek(f1, 0, SEEK_END);
	printf("File length is now %ld.\n",ftell(f1));
	rewind(f1); // *** debugging
	
	printf("Seek to position 5 (location of '5').\n");
	if (fseek(f1,5, SEEK_SET) != 0)
	{
		printf("***ERROR: fseek() %s:%s\n", f1_name,strerror(errno));
		exit (-1);
	}

	printf("Writing \"XXX\" to w+ file...\n");
 	fr=fputs("XXX",f1);
   	if (fr == -1) {
 		printf("***ERROR writing %s:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 	else {
		printf("Wrote to file, ftell()=%ld.\n",ftell(f1));
 	}

	printf("Rewinding file...\n");
	rewind(f1);

	puts("Reading and showing contents of file");
	puts("using fgetc()...");
	while (!feof(f1)) {
		ch=fgetc(f1);
		if (isprint(ch)) putchar(ch);
	}
	printf("\n");
	printf("ftell()=%ld\n",ftell(f1));

	printf("Closing file...\n");
	if (fclose(f1) == -1) {
 		printf("***ERROR closing %s:%s.\n", f1_name, strerror(errno));
 	} else {
 		printf("Closed file %s.\n", f1_name);
 	}
	
	puts("+---------------+");
	puts("| End of Test 2 |");
    puts("+---------------+");


	printf("\nPress any key to continue...");
	getchar();
	printf("\n\n");


   puts("+------------------+");
   puts("| Test 3: mode 'r' |");
   puts("+-------------------------------------------+");
   puts("+ Read from read-only file and try to write |");
   puts("+-------------------------------------------+\n");

   strcpy(f1_name,"alpha.txt");
   printf("Opening %s for read-only...\n",f1_name);
   f1=fopen(f1_name, "r");
   if (f1 == NULL) {
 		printf("***ERROR opening %s:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 	else {
 		printf("Opened.\n");
 	}

	puts("Reading and showing contents of file");
	puts("using fgets()...");

	if (fgets(s,MAXSTRLEN,f1) == NULL) {
		printf("***ERROR reading: %s.\n", strerror(errno));
		exit(-1);
	} else {
		puts(s);
	}
	printf("ftell()=%ld\n",ftell(f1));

	printf("Seek to position 18 (location of 'T').\n");
	if (fseek(f1,18, SEEK_SET) != 0)
	{
		printf("***ERROR: fseek() %s:%s\n", f1_name,strerror(errno));
		exit (-1);
	}

	printf("Writing to read-only file...\n");
 	fr=fputs("Q",f1);
   	if (fr == EOF) {
		printf("AS EXPECTED:\n");
		printf("  Writing to read-only file failed:\n");
		printf("  %s.\n",strerror(errno));
		clearerr(f1);
 	}
 	else {
 		printf("Writing to read-only file succeeded, ftell()=%ld.\n",ftell(f1));
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit(-1);
 	}

	printf("Closing file...\n");
	if (fclose(f1) == -1) {
 		printf("***ERROR closing %s:%s.\n", f1_name, strerror(errno));
		exit(-1);
 	} else {
 		printf("Closed file %s.\n", f1_name);
 	}

	puts("+---------------+");
	puts("| End of Test 3 |");
    puts("+---------------+");

	printf("\nPress any key to continue...");
	getchar();
	printf("\n\n");

   puts("+-------------------+");
   puts("| Test 4: mode 'r+' |");
   puts("+-------------------------------------+");
   puts("| Write and read from read-write file |");
   puts("+-------------------------------------+\n");

   strcpy(f1_name,"alpha.txt");
   printf("Opening %s for read and write...\n",f1_name);
   f1=fopen(f1_name, "r+");
   if (f1 == NULL) {
 		printf("***ERROR opening %s:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 	else {
 		printf("Opened.\n");
 	}
	
	printf("ftell()=%ld\n",ftell(f1));
	puts("Seek to location -34 from End-of-File");
	// Location -34 from EOF is 'R', where 'Q' would normally be.
	if (fseek(f1,-34, SEEK_END) != 0)
	{
		printf("ERROR: fseek() %s:%s\n", f1_name,strerror(errno));
		exit (-1);
	}
	printf("ftell()=%ld\n",ftell(f1));

	puts("Writing \"QQQ\" to file using fputc()...");
	for (i=0;i<3;i++) {
		fr=fputc('Q',f1);
		if (fr == EOF) {
			printf("***ERROR writing %s:%s\n", f1_name,strerror(errno));
 			exit (-1);
		}
	}
	printf("Wrote to file, ftell()=%ld\n",ftell(f1));
	rewind(f1);
 	printf("Rewinding file, ftell()=%ld\n",ftell(f1));

	puts("Reading and displaying contents of file");
	puts("using fgets()...");
	if (fgets(s,MAXSTRLEN,f1) == NULL) {
		printf("***ERROR reading: %s.\n", strerror(errno));
		exit(-1);
	} else {
		puts(s);
	}

	printf("Closing file...\n");
	if (fclose(f1) == -1) {
 		printf("***ERROR closing %s:%s.\n", f1_name, strerror(errno));
		exit(-1);
 	} else {
 		printf("Closed file %s.\n", f1_name);
 	}
	
	puts("+---------------+");
	puts("| End of Test 4 |");
    puts("+---------------+");

	printf("\nPress any key to continue...");
	getchar();
	printf("\n\n");


   puts("+---------------------------+");
   puts("| Test 5: mode 'a' (part 1) |");
   puts("+---------------------------------------------+");
   puts("| Append to a write-only file and try to read |");
   puts("+---------------------------------------------+\n");
   // Append is the most complex mode for CC65/Neo6502 integration.
   // Specificically, for append mode, existing files and new files
   // are handled very differently.

   // Open an existing file for append.
   strcpy(f1_name,"alpha.txt");
   printf("Opening existing file %s for append...\n",f1_name);
   f1=fopen(f1_name, "a");
   if (f1 == NULL) {
 		printf("***ERROR opening file for append:%s\n", strerror(errno));
 		exit (-1);
 	}
 	else {
 		printf("Opened file for append, ftell()=%ld\n",ftell(f1));
 	}

   puts("Appending to file using fputs()...");
   fr=fputs("0123456789",f1);
   if (fr == -1) {
 		printf("***ERROR writing %s:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 	else {
		// Show the default seek position.
 		printf("Wrote to file, ftell()=%ld\n",ftell(f1));
 	}

	printf("Rewinding file... ");
	rewind(f1);
	printf("ftell()=%ld.\n",ftell(f1));

	puts("Trying to read from write-only file");
	puts("using fgets()...");
	if (fgets(s,MAXSTRLEN,f1) == NULL) {
		printf("AS EXPECTED:\n");
		printf("  Reading from write-only file failed,\n");
		printf("  %s.\n",strerror(errno));
		clearerr(f1);
	} else {
		puts(s);
		printf("Reading from write-only file succeeded,\n");
		printf("s=\"%s\".\n",s);
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit(-1);
	}

	puts("Closing the file to reopen in read mode...");
	if (fclose(f1) == -1) {
 		printf("***ERROR closing %s:%s\n", f1_name, strerror(errno));
		exit(-1);
 	} else {
 		printf("Closed file %s\n", f1_name);
 	}

   printf("Reopening file %s for read...\n",f1_name);
   f1=fopen(f1_name, "r");
   if (f1 == NULL) {
 		printf("***ERROR opening file for read:%s\n", strerror(errno));
 		exit (-1);
 	}
 	else {
 		printf("Opened file for read, ftell()=%ld\n",ftell(f1));
 	}

	printf("Reading from %s, now read-only\n",f1_name);
	puts("using fgets()...");
	if (fgets(s,MAXSTRLEN,f1) == NULL) {
		printf("ERROR:\n");
		printf("  Reading from read-only file failed,\n");
		printf("  %s.\n",strerror(errno));
		exit (-1);
	} else {
		puts(s);
	}

	if (fclose(f1) == -1) {
 		printf("***ERROR closing %s:%s\n", f1_name, strerror(errno));
		exit (-1);
 	} else {
 		printf("Closed file %s\n", f1_name);
 	}

	puts("+---------------+");
	puts("| End of Test 5 |");
    puts("+---------------+");

	printf("\nPress any key to continue...");
	getchar();
	printf("\n\n");


   puts("+----------------------------+");
   puts("| Test 6: mode 'a' (part 2)  |");
   puts("+---------------------------------------------+");
   puts("| Append to a write-only file and try to read |");
   puts("+---------------------------------------------+\n");
   // Append is the most complex mode for CC65/Neo6502 integration.
   // Specificically, for append mode, existing files and new files
   // are handled very differently.

 	strcpy(f1_name,"alpha_new.txt");
	printf("Removing %s in case it exists...\n",f1_name);
	
	if (fr=remove(f1_name)) {
		// An error here is OK:
		if (errno == ENOENT) {
			printf("File %s does not exist.\n",f1_name);
		} else {
			 printf("***ERROR removing %s:%s\n", f1_name, strerror(errno));
			 exit(-1);
		}
	} else {
		printf("Removed file %s\n", f1_name);
	}

   // Open a new file for append.
   f1=fopen(f1_name, "a");
   if (f1 == NULL) {
 		printf("***ERROR opening new file for append:%s\n", strerror(errno));
 		exit (-1);
 	}
 	else {
 		printf("Opened new file for append, ftell()=%ld\n",ftell(f1));
 	}

   puts("Appending to file using fputs()...");
   fr=fputs("Neo6502 and CC65.",f1);
   if (fr == -1) {
 		printf("***ERROR writing %s:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 	else {
		// Show the default seek position.
 		printf("Wrote to file, ftell()=%ld\n",ftell(f1));
 	}

	puts("Rewinding file...");
	rewind(f1);
	puts("Trying to read from write-only file");
	puts("using fgets()...");
	if (fgets(s,MAXSTRLEN,f1) == NULL) {
		printf("AS EXPECTED:\n");
		printf("  Reading from write-only file failed,\n");
		printf("  %s.\n",strerror(errno));
		clearerr(f1);
	} else {
		puts(s);
		printf("Reading from write-only file succeeded,\n");
		printf("s=\"%s\".\n",s);
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit (-1);
	}

	puts("Closing the file to reopen in read mode...");
	if (fclose(f1) == -1) {
 		printf("***ERROR closing %s:%s\n", f1_name, strerror(errno));
		exit (-1);
 	} else {
 		printf("Closed file %s\n", f1_name);
 	}

   printf("Reopening file %s for read...\n",f1_name);
   f1=fopen(f1_name, "r");
   if (f1 == NULL) {
 		printf("***ERROR opening file for read:%s\n", strerror(errno));
 		exit (-1);
 	}
 	else {
 		printf("Opened file for read, ftell()=%ld\n",ftell(f1));
 	}

	printf("Reading from %s, now read-only\n",f1_name);
	puts("using fgets()...");
	if (fgets(s,MAXSTRLEN,f1) == NULL) {
		printf("ERROR:\n");
		printf("  Reading from read-only file failed,\n");
		printf("  %s.\n",strerror(errno));
		exit (-1);
	} else {
		puts(s);
	}

	if (fclose(f1) == -1) {
 		printf("***ERROR closing %s:%s\n", f1_name, strerror(errno));
		exit (-1);
 	} else {
 		printf("Closed file %s\n", f1_name);
	}

	puts("+---------------+");
	puts("| End of Test 6 |");
    puts("+---------------+");

	printf("\nPress any key to continue...");
	getchar();
	printf("\n\n");

 puts("+-------------------------------+");
   puts("| Test 7: mode 'a+' (part 1)  |");
   puts("+------------------------------------------------+");
   puts("| Append to an existing r-w file and try to read |");
   puts("+------------------------------------------------+\n");
   // Append is the most complex mode for CC65/Neo6502 integration.
   // Specificically, for append mode, existing files and new files
   // are handled very differently.

// Open an existing file for append read-write.
   strcpy(f1_name,"alpha.txt");
   printf("Opening existing file %s for append r-w...\n",f1_name);
   f1=fopen(f1_name, "a+");
   if (f1 == NULL) {
 		printf("***ERROR opening %s for append r-w:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 	else {
 		printf("Opened %s for append r-w.\n",f1_name);
 	}

   puts("Appending to file using fputs()...");
   fr=fputs(" 0123456789abcdefghijklmnoprstuvwxyz.",f1);
   if (fr == -1) {
 		printf("***ERROR writing %s:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 	else {
		// Show the default seek position.
 		printf("Wrote to file, ftell()=%ld\n",ftell(f1));
 	}

	puts("Rewinding file...");
	rewind(f1);

	// Note: this won't work if the file containes multiple newlines.
	printf("Reading from %s...\n",f1_name);
   	puts("using fgets()...");
	if (fgets(s,MAXSTRLEN,f1) == NULL) {
		printf("ERROR:\n");
		printf("  Reading from r-w file failed,\n");
		printf("  %s.\n",strerror(errno));
		exit (-1);
	} else {
		puts(s);
	}
	printf("After print, ftell()=%ld\n",ftell(f1));
	
	if (fclose(f1) == -1) {
 		printf("***ERROR closing %s:%s\n", f1_name, strerror(errno));
		exit (-1);
 	} else {
 		printf("Closed file %s\n", f1_name);
	}

	puts("+---------------+");
	puts("| End of Test 7 |");
    puts("+---------------+");

	printf("\nPress any key to continue...");
	getchar();
	printf("\n\n");


   puts("+----------------------------+");
   puts("| Test 8: mode 'a+' (part 2) |");
   puts("+------------------------------------------+");
   puts("| Append to a new r-w file and try to read |");
   puts("+------------------------------------------+\n");
   // Append is the most complex mode for CC65/Neo6502 integration.
   // Specificically, for append mode, existing files and new files
   // are handled very differently.

	strcpy(f1_name,"alpha_new.txt");
	printf("Removing %s in case it exists...\n",f1_name);
	
	if (fr=remove(f1_name)) {
		// An error here is OK:
		if (errno == ENOENT) {
			printf("File %s does not exist.\n",f1_name);
		} else {
			 printf("***ERROR removing %s:%s\n", f1_name, strerror(errno));
			 exit(-1);
		}
	} else {
		printf("Removed file %s\n", f1_name);
	}

// Open a new file for append read-write.
   printf("Opening new file %s for append r-w...\n",f1_name);
   f1=fopen(f1_name, "a+");
   if (f1 == NULL) {
 		printf("***ERROR opening %s for append r-w:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 	else {
 		printf("Opened %s for append r-w.\n",f1_name);
 	}

   puts("Appending to file using fputs()...");
   fr=fputs("This is our ongoing test file.\nTHIS is a new line!\n",f1);
   if (fr == -1) {
 		printf("***ERROR writing %s:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 	else {
		// Show the default seek position.
 		printf("Wrote to file, ftell()=%ld\n",ftell(f1));
 	}

	puts("Rewinding file...");
	rewind(f1);

	while (fgets(s,MAXSTRLEN,f1) != NULL)
	{
		printf("%s",s); // The strings will have new lines in them
	}
	printf("After print, ftell()=%ld\n",ftell(f1));
	
	if (fclose(f1) == -1) {
 		printf("***ERROR closing %s:%s\n", f1_name, strerror(errno));
		exit (-1);
 	} else {
 		printf("Closed file %s\n", f1_name);
	}

	puts("+---------------+");
	puts("| End of Test 8 |");
    puts("+---------------+");

	printf("\nPress any key to continue...");
	getchar();
	printf("\n\n");


 	puts("+-----------------------------+");
   	puts("| Test 9: remove() function   |");
   	puts("+----------------------------------------+");
   	puts("| remove existing and non-existing files |");
   	puts("+----------------------------------------+\n");
   	// We've already used remove(), but this is a systematic test.
   
	strcpy(f1_name,"alpha_new.txt");
	printf("Removing existing file %s...\n",f1_name);
	if (fr=remove(f1_name)) {
		printf("***ERROR removing existing file %s:\n%s\n", f1_name, strerror(errno));
		 exit(-1);
	} else {
		printf("Removed existing file %s\n", f1_name);
	}

	// Try to open the removed file:
   	printf("Trying to open removed file %s...\n",f1_name);
   	f1=fopen(f1_name, "r");
   	if (f1 == NULL) {
 		printf("AS EXPECTED:\n");
		printf("  ***ERROR opening %s:\n  %s\n", f1_name,strerror(errno));
 	} else {
		printf("Deleted file %s was opened.\n",f1_name);
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit (-1);
	}

	strcpy(f1_name,"alpha_not.txt");
	printf("Removing non-existing file %s...\n",f1_name);
	if (fr=remove(f1_name)) {
		printf("AS EXPECTED:\n");
		printf("  ***ERROR removing non-existing file %s:\n  %s\n", f1_name, strerror(errno));
	} else {
		printf("Remove of non-existing file succeeded %s\n", f1_name);
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit (-1);
	}

	puts("+---------------+");
	puts("| End of Test 9 |");
    puts("+---------------+");

	printf("\nPress any key to continue...");
	getchar();
	printf("\n\n");


 	puts("+------------------------------+");
   	puts("| Test 10: rename() function   |");
   	puts("+--------------------------------------- +");
   	puts("| remove existing and non-existing files |");
   	puts("+----------------------------------------+\n");

	strcpy(f1_name,"alpha.txt");
	strcpy(f2_name,"alpha_new.txt");

   	f1=fopen(f1_name, "r");
   	if (f1 == NULL) {
 		printf("***ERROR opening %s for read:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 	else {
 		printf("Opened %s for read.\n",f1_name);
 	}

	while (fgets(s,MAXSTRLEN,f1) != NULL)
	{
		printf("%s",s); // The strings will have new lines in them
	}
	printf("\n");

	if (fclose(f1) == -1) {
 		printf("***ERROR closing %s:%s\n", f1_name, strerror(errno));
		exit (-1);
 	} else {
 		printf("Closed file %s\n", f1_name);
	}

	printf("Renaming existing file to non-existing file name,\n");
	printf("%s to %s...\n",f1_name,f2_name);
	if (fr=rename(f1_name,f2_name)) {
		printf("***ERROR renaming %s to %s:\n%s\n", f1_name, f2_name,strerror(errno));
		 exit(-1);
	} else {
		printf("Renamed %s to %s\n", f1_name,f2_name);
	}

	// open new file to show that it is the same
	f2=fopen(f2_name, "r");
   	if (f2 == NULL) {
 		printf("***ERROR opening %s for read:%s\n", f2_name,strerror(errno));
 		exit (-1);
 	}
 	else {
 		printf("Opened %s for read.\n",f2_name);
 	}

	while (fgets(s,MAXSTRLEN,f2) != NULL)
	{
		printf("%s",s); // The strings will have new lines in them
	}
	printf("\n");

	if (fclose(f2) == -1) {
 		printf("***ERROR closing %s:%s\n", f2_name, strerror(errno));
		exit (-1);
 	} else {
 		printf("Closed file %s\n", f2_name);
	}
//------------------------------------------------------------------
	printf("\nPress any key to continue...");getchar();
	printf("\nContinue Test 10...\n");
//------------------------------------------------------------------
	// rename a file to an EXISTING name
	if (fr=rename(f1_name,f2_name)) {
		printf("AS EXPECTED:\n");
		printf("  ***ERROR renaming file %s to\n  existing file %s:\n  %s\n", f1_name, f2_name,strerror(errno));
	} else {
		printf("Rename of file succeeded %s to %s\n", f1_name,f2_name);
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit (-1);
	}

	// Remove f2_name so we can test renaming a non-existing file
	printf("Removing existing file %s...\n",f2_name);
	if (fr=remove(f2_name)) {
		printf("***ERROR removing existing file %s:\n%s\n", f2_name, strerror(errno));
		 exit(-1);
	} else {
		printf("Removed existing file %s\n", f2_name);
	}

	printf("Renaming non-existing file %s\nto %s...\n",f1_name,f2_name);
	if (fr=rename(f1_name,f2_name)) {
		printf("AS EXPECTED:\n");
		printf("  ***ERROR renaming %s to %s:\n  %s\n", f1_name, f2_name,strerror(errno));
	} else {
		printf("Renamed %s to %s\n", f1_name,f2_name);
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit (-1);
	}

	puts("+----------------+");
	puts("| End of Test 10 |");
    puts("+----------------+");

	printf("\nPress any key to continue...");
	getchar();
	printf("\n\n");

 	puts("+------------------------------+");
   	puts("| Test 11: filename conversions|");
   	puts("+------------------------------+");
   	puts("| Overlong file name handling  |");
   	puts("+------------------------------+\n");

	// This file name is over 260 characters, so it should fail (max is 255 chars)
   	//printf("Opening new file with overlong name...\n");
	f1=fopen(vlfname, "r");
   	if (f1 == NULL) {
		if (errno == EINVAL) {
			printf("AS EXPECTED:\n");
			printf("  ***ERROR opening file with overlong name:\n  %s\n",strerror(errno));
		}
		else {
			printf("***ERROR opening file with overlong name:\n%s\n",strerror(errno));
			printf("Unexpected error returned: expected %d, got %d.",EINVAL,errno);
			exit (-1);
		}
 	}
 	else {
		printf("Opened file with overlong name for read.\n");
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit (-1);
 	}

	//printf("Renaming file with overlong old name...\n");
	if (fr=rename(vlfname,f2_name)) {
		if (errno == EINVAL) {
			printf("AS EXPECTED:\n");
			printf("  ***ERROR renaming file with overlong old name:\n  %s\n",strerror(errno));
		} else {
			printf("***ERROR renaming file with overlong name:\n%s\n",strerror(errno));
			printf("Unexpected error returned: expected %d, got %d.",EINVAL,errno);
			exit (-1);
		}
 	} else {
		printf("Renamed file with overlong old name.\n");
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit (-1);
	}

	//printf("Renaming file with overlong new name...\n");
	if (fr=rename(f2_name,vlfname)) {
		if (errno == EINVAL) {
			printf("AS EXPECTED:\n");
			printf("  ***ERROR renaming file with overlong new name:\n  %s\n",strerror(errno));
		} else {
			printf("***ERROR renaming file with overlong new name:\n%s\n",strerror(errno));
			printf("Unexpected error returned: expected %d, got %d.",EINVAL,errno);
			exit (-1);
		}
 	} else {
		printf("Renamed file with overlong new name.\n");
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit (-1);
	}

	//printf("Removing file with overlong name...\n");
	if (fr=remove(vlfname)) {
		if (errno == EINVAL) {
			printf("AS EXPECTED:\n");
			printf("  ***ERROR removing file with overlong name:\n  %s\n",strerror(errno));
		} else {
			printf("***ERROR removing file with overlong name:\n%s\n",strerror(errno));
			printf("Unexpected error returned: expected %d, got %d.",EINVAL,errno);
			exit (-1);
		}
	} else {
		printf("Removed file with overlong name.\n");
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit (-1);
	}

	puts("+----------------+");
	puts("| End of Test 11 |");
    puts("+----------------+");

	printf("\nPress any key to continue...");
	getchar();
	printf("\n\n");

	puts("+-------------------------------+");
   	puts("| Test 12: mkdir() and remove() |");
   	puts("+----------------------------------------------+");
   	puts("| Directory functions + one more remove() test |");
   	puts("+----------------------------------------------+\n");

	// create a file "a2" to be used to test mkdir()
	strcpy(f1_name,"a2");
	f1=fopen(f1_name, "a");
   	if (f1 == NULL) {
 		printf("***ERROR opening %s for read:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 	else {
		printf("Created test file %s.\n",f1_name);
 	}

  	puts("Writing to file using fputs()...");
    fr=fputs("This is test file a2",f1);
   	if (fr == -1) {
 		printf("***ERROR writing %s:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 	else {
		// Show the default seek position.
 		printf("Wrote to file, ftell()=%ld.\n",ftell(f1));
 	}

	//printf("Closing file...\n");
	if (fclose(f1) == -1) {
 		printf("***ERROR closing %s:%s.\n", f1_name, strerror(errno));
		exit (-1);
 	} else {
 		printf("Closed file %s.\n", f1_name);
 	}

	strcpy(f1_name,"a1"); // new directory name
	printf("Making new directory %s\n",f1_name);
	if (fr=mkdir(f1_name) == -1) {
		printf("***ERROR making directory %s:\n%s\n",f1_name,strerror(errno));
		exit (-1);
	} else {
		printf("Made directory %s\n",f1_name);
	}

	// Attempt to make directory with the same name (should fail):
	strcpy(f1_name,"a1"); // new directory name
	printf("Trying to make directory with existing name %s\n",f1_name);
	if (fr=mkdir(f1_name) == -1) {
		printf("AS EXPECTED:\n");
		printf("  ***ERROR making directory %s:\n  %s\n",f1_name,strerror(errno));
	} else {
		printf("Made directory %s\n",f1_name);
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit (-1);
	}

	// Attempt to make directory with the name of the file previously created (should fail):
	strcpy(f1_name,"a2"); // new directory name
	printf("Trying to make directory with existing\nfile name %s\n",f1_name);
	if (fr=mkdir(f1_name) == -1) {
		printf("AS EXPECTED:\n");
		printf("  ***ERROR making directory %s:\n  %s\n",f1_name,strerror(errno));
	} else {
		printf("Made directory %s\n",f1_name);
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit (-1);
	}

	// Try to remove() a directory (should fail)
	strcpy(f1_name,"a1");
	printf("Removing directory %s...\n",f1_name);
	if (fr=remove(f1_name)) {
		printf("AS EXPECTED:\n");
		printf("  ***ERROR removing directory %s:\n  %s\n", f1_name, strerror(errno));
	} else {
		printf("Removed existing directory %s\n", f1_name);
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit(-1);
	}

	puts("+----------------+");
	puts("| End of Test 12 |");
    puts("+----------------+");

	printf("\nPress any key to continue...");
	getchar();
	printf("\n\n");

	puts("+-------------------------------+");
   	puts("| Test 13: rmdir() and chdir()  |");
   	puts("+-------------------------------+");
   	puts("| Remove and change directories |");
   	puts("+-------------------------------+\n");

// *** We expect directory "a1" and file "a2" to be present from the previous test	

	// try to remove existing, empty directory using remove()
	strcpy(f1_name,"a1"); // directory name
	if (remove(f1_name) == -1) {
		printf("AS EXPECTED:\n");
		printf("  ***ERROR remove() directory %s:\n  %s\n", f1_name, strerror(errno));
	} else {
		printf("remove() existing directory %s\n", f1_name);
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit(-1);
	}

	// rmdir() existing, empty directory
	strcpy(f1_name,"a1"); // directory name
	if (rmdir(f1_name) == -1) {
		printf("***ERROR rmdir() dir %s:\n%s\n",f1_name,strerror(errno));
		exit (-1);
	} else {
		printf("rmdir() dir %s\n",f1_name);
	}


	// Try to rmdir() a directory that doesn't exist (should fail)
	strcpy(f1_name,"a1");
	// printf("rmdir() non-existing directory %s...\n",f1_name);
	if (rmdir(f1_name)) {
		printf("AS EXPECTED:\n");
		printf("  ***ERROR rmdir() non-existing directory %s:\n  %s\n", f1_name, strerror(errno));
	} else {
		printf("rmdir() non-existing directory %s\n", f1_name);
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit(-1);
	}

	// Try to rmdir() a file that exists (should fail)
	strcpy(f2_name,"a2");
	//printf("rmdir() file %s...\n",f2_name);
	if (rmdir(f2_name)) {
		printf("AS EXPECTED:\n");
		printf("  ***ERROR rmdir() file %s:%s\n", f2_name, strerror(errno));
	} else {
		printf("rmdir() file %s\n", f2_name);
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit(-1);
	}

	// Create a new directory for the next step
	strcpy(f1_name,"a1"); // new directory name
	// printf("Making new directory %s\n",f1_name);
	if (mkdir(f1_name) == -1) {
		printf("***ERROR making new directory %s:\n%s\n",f1_name,strerror(errno));
		exit (-1);
	} else {
		printf("Made new directory %s\n",f1_name);
	}


	// chdir()
	if (chdir(f1_name) == -1) {
		printf("***ERROR changing directory %s:\n%s\n",f1_name,strerror(errno));
		exit (-1);
	} else {
		printf("Changed directory to %s\n",f1_name);
	}

	// create a file to be used to test mkdir()
	strcpy(f1_name,"a1_file.txt");
	//printf("Creating file %s for testing.\n");
	f1=fopen(f1_name, "a");
   	if (f1 == NULL) {
 		printf("***ERROR creating %s:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 	else {
		printf("Created file %s for testing.\n",f1_name);
 	}


    fr=fputs("This file should be in directory a1",f1);
   	if (fr == -1) {
 		printf("***ERROR writing %s:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	} else {
		puts("Wrote to file using fputs()...");
	}

	//printf("Closing file...\n");
	if (fclose(f1) == -1) {
 		printf("***ERROR closing %s:%s.\n", f1_name, strerror(errno));
		exit (-1);
 	} else {
		printf("Closed file %s.\n",f1_name);
	}

	// chdir()
	strcpy(f1_name,"..");
	// printf("Changing directory back up one level to %s\n",f1_name);
	if (chdir(f1_name) == -1) {
		printf("***ERROR changing directory to %s:\n%s\n",f1_name,strerror(errno));
		exit (-1);
	} else {
		printf("Changed directory back up one level to %s\n",f1_name);
	}


	// create another file in the top-level directory
	strcpy(f1_name,"a3");
	f1=fopen(f1_name, "a");
   	if (f1 == NULL) {
 		printf("***ERROR opening %s for read:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 	else {
		printf("Created test file %s.\n",f1_name);
 	}

  	puts("Writing to file using fputs()...");
    fr=fputs("This file should be in \"storage\"",f1);
   	if (fr == -1) {
 		printf("***ERROR writing %s:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}

		if (fclose(f1) == -1) {
 		printf("***ERROR closing %s:%s.\n", f1_name, strerror(errno));
		exit (-1);
 	} else {
		printf("Closed file %s\n",f1_name);
	}

//------------------------------------------------------------------
	printf("\nPress any key to continue...");getchar();
	printf("\nContinue Test 13...\n");
//------------------------------------------------------------------
	
	// chdir() to non-existing directory:
	strcpy(f1_name,"a5");
	//printf("chdir() to non-existing directory %s\n",f1_name);
	if (chdir(f1_name) == -1) {
		printf("AS EXPECTED:\n");
		printf("  ***ERROR chdir() to non-existing dir %s:\n  %s\n",f1_name,strerror(errno));
	} else {
		printf("chdir() to non-existing dir %s succeeded\n", f1_name);
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit(-1);
	}

	// rmdir() non-empty directory:
	strcpy(f1_name,"a1");
	// printf("rmdir() to non-empty directory %s\n",f1_name);
	if (rmdir(f1_name) == -1) {
		printf("AS EXPECTED:\n");
		printf("  ***ERROR rmdir() non-empty dir %s:\n  %s\n",f1_name,strerror(errno));
	} else {
		printf("rmdir() non-empty dir %s succeeded\n", f1_name);
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit(-1);
	}

	// Start cleanup and more checks...
	// printf("Changing directory to %s\n",f1_name);
	if (chdir(f1_name) == -1) {
		printf("***ERROR changing directory %s:\n%s\n",f1_name,strerror(errno));
		exit (-1);
	} else {
		printf("Changed directory to %s\n",f1_name);
	}

	// remove the file that was written to this directory
	strcpy(f1_name,"a1_file.txt");
	if (remove(f1_name)) {
			 printf("***ERROR removing %s:%s\n", f1_name, strerror(errno));
			 exit(-1);
	} else {
		printf("Removed file %s\n", f1_name);
	}

	// chdir()
	strcpy(f1_name,"..");
	//printf("Changing directory back up one level to %s\n",f1_name);
	if (chdir(f1_name) == -1) {
		printf("***ERROR changing directory to %s:\n%s\n",f1_name,strerror(errno));
		exit (-1);
	} else {
		printf("Changed directory to %s\n",f1_name);
	}

	// rmdir() now empty directory:
	strcpy(f1_name,"a1");
	// printf("rmdir() to empty directory %s\n",f1_name);
	if (rmdir(f1_name) == -1) {
		printf("***ERROR rmdir() empty dir %s:\n  %s\n",f1_name,strerror(errno));
		exit (-1);
	} else {
		printf("rmdir() empty dir %s succeeded\n", f1_name);
	}

	// remove the remaining test files:
	strcpy(f1_name,"a2");
	if (remove(f1_name)) {
			 printf("***ERROR removing %s:%s\n", f1_name, strerror(errno));
			 exit(-1);
	} else {
		printf("Removed file %s\n", f1_name);
	}

	strcpy(f1_name,"a3");
	if (remove(f1_name)) {
			 printf("***ERROR removing %s:%s\n", f1_name, strerror(errno));
			 exit(-1);
	} else {
		printf("Removed file %s\n", f1_name);
	}

	puts("+----------------+");
	puts("| End of Test 13 |");
    puts("+----------------+");

	printf("\nPress any key to continue...");
	getchar();
	printf("\n\n");

	puts("+-------------------------------+");
   	puts("| Test 14: fseek()              |");
   	puts("+-------------------------------+");
   	
	// create a file "alpha.txt" to be used to test fseek()
	strcpy(f1_name,"alpha.txt");
	f1=fopen(f1_name, "a+");
   	if (f1 == NULL) {
 		printf("***ERROR opening %s for append+:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 	else {
		printf("Created test file %s.\n",f1_name);
 	}

	puts("Writing to file using fputs()...");
   // 'Q' is omitted both times so that the two alphabets fit on the screen
   fr=fputs("ABCDEFGHIJKLMNOPRSTUVWXYZabcdefghijklmnoprstuvwxyz",f1);
   if (fr == -1) {
 		printf("***ERROR writing %s:%s\n", f1_name,strerror(errno));
 		exit (-1);
 	}
 
	puts("Rewind, reading, and showing contents of file");
	puts("using fgets()...");
	rewind(f1);
	
	if (fgets(s,MAXSTRLEN,f1) == NULL) {
		printf("***ERROR reading: %s.\n", strerror(errno));
		exit(-1);
	} else {
		puts(s);
	}
	printf("ftell()=%ld\n",ftell(f1));

	printf("Testing fseek(), showing ftell() AFTER fgetc()...\n");
	if (fseek(f1,10, SEEK_SET) != 0)
	{
		printf("ERROR: fseek() %s:%s\n", f1_name,strerror(errno));
		exit (-1);
	}
   	ch = fgetc(f1);
   	if (ch == 'K') {
		printf("ch='%c', ftell()=%ld\n", ch,ftell(f1));
	} else {
		printf("***ERROR fseek() %s\n, expected '%c', read '%c'\n",f1_name,'K',ch);
 		exit (-1);
	}

   	if (fseek(f1,5, SEEK_CUR) != 0) 
	{
		printf("ERROR: fseek() %s:%s\n", f1_name,strerror(errno));
		exit (-1);
	}
   ch = fgetc(f1);
   printf("ch='%c', ftell()=%ld\n", ch,ftell(f1));

	if (fseek(f1,-10, SEEK_CUR) != 0) 
	{
		printf("ERROR: fseek() %s:%s\n", f1_name,strerror(errno));
		exit (-1);
	}
	ch = fgetc(f1);
	printf("ch='%c', ftell()=%ld\n", ch,ftell(f1));

	if (fseek(f1,-10, SEEK_END) != 0) 
	{
		printf("ERROR: fseek() %s:%s\n", f1_name,strerror(errno));
		exit (-1);
	}
	ch = fgetc(f1);
	printf("ch='%c', ftell()=%ld\n", ch,ftell(f1));

	// Bad whence, this should fail.
	if (fseek(f1,-10, 0xa5) != 0) 
	{
		printf("  AS EXPECTED:\n");
		printf("  ***ERROR:fseek() with bad whence:\n  %s\n", strerror(errno));
		// Errors put the file stream into an error state that must be cleared:
		clearerr(f1);
	} else {
		printf("ERROR: fseek() with bad WHENCE succeeded.\n");
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		exit (-1);
	}
	
	printf("ftell()=%ld\n", ftell(f1));

	// Bad offset, this should fail.
	if   (fseek(f1,-2, SEEK_SET) != 0) 
	{
		printf("  AS EXPECTED:\n");
		printf("  ***ERROR:fseek() with bad offset:%s\n",strerror(errno));
		// Errors put the file stream into an error state that must be cleared:
		clearerr(f1);
	} else {
		printf("ERROR: fseek() with BAD offset succeeded.\n");
		printf("*** INCORRECT BEHAVIOUR FOR FILE IO***\n");
		printf("ch='%c', ftell()=%ld\n", ch,ftell(f1));
		exit (-1);
	}
	
//------------------------------------------------------------------
	printf("\nPress any key to continue...");getchar();
	printf("\nContinue Test 14...\n");
//------------------------------------------------------------------

	printf("ftell()=%ld\n", ftell(f1));

	printf("Testing fgetpos() and fsetpos()...\n");

	if (fseek(f1,-11, SEEK_END) != 0) 
	{
		printf("ERROR: fseek() %s:%s\n", f1_name,strerror(errno));
		exit (-1);
	}
	printf("After fseek(\"...\",-11,SEEK_END),ftell()=%ld\n",ftell(f1));
	ch = fgetc(f1);
	printf("ch='%c', ftell()=%ld\n", ch,ftell(f1));

	if (fgetpos(f1,&fpos)) {
		printf("  ***ERROR: fgetpos() %s:%s\n", f1_name,strerror(errno));
		exit (-1);
	} else {
		printf("fgetpos() succeeded on %s.\n",f1_name);
	}

	printf("Rewinding %s\n",f1_name);
	rewind(f1);
	ch = fgetc(f1);
	printf("ch='%c', ftell()=%ld\n", ch,ftell(f1));

	if (fsetpos(f1,&fpos)) {
	printf("  ***ERROR: fsetpos() %s:%s\n", f1_name,strerror(errno));
	exit (-1);
	} else {
		printf("fsetpos() succeeded on %s.\n",f1_name);
	}
	ch = fgetc(f1);
	printf("ch='%c', ftell()=%ld\n", ch,ftell(f1));

	if (fclose(f1) == -1) {
		printf("***ERROR closing %s:%s\n", f1_name, strerror(errno));
	} else {
		printf("Closed file %s\n", f1_name);
	}

	if (remove(f1_name)) {
		printf("***ERROR removing %s:%s\n", f1_name, strerror(errno));
		exit(-1);
	} else {
		printf("Removed file %s\n", f1_name);
	}

	puts("+----------------+");
	puts("| End of Test 14 |");
    puts("+----------------+");

	printf("\nPress any key to continue...");
	getchar();
	printf("\n\n");

	puts("+------------------+");
   	puts("| Test 15: uname() |");
   	puts("+------------------+\n");
   	
	puts("Calling uname()...\n");

    if (uname(&unameData)) {
		printf("***ERROR uname() failed.\n");
		exit (-1);
	}

	printf("System:  %s\n", unameData.sysname);
    printf("Node:    %s\n", unameData.nodename);
    printf("Release: %s\n", unameData.release);
    printf("Version: %s\n", unameData.version);
    printf("Machine: %s\n", unameData.machine);
	printf("\n");

	puts("+----------------+");
	puts("| End of Test 15 |");
    puts("+----------------+");

	printf("\nPress any key to continue...");
	getchar();
	printf("\n\n");

	puts("+------------------------------------+");
   	puts("| Test 16: freopen() and std streams |");
   	puts("+------------------------------------+");

	printf("Printing alphabet to STDOUT...\n");
	fprintf(stdout,"ABCDEFGHIJKLMNOPRSTUVWXYZ\n");
	
	printf("Printing digits to STDERR...\n");
	fprintf(stderr,"0123456789\n");
	
	strcpy(f1_name,"test16.txt");
	printf("Redirecting STDERR to %s\n",f1_name);
	printf("using freopen()...\n");

	f1=freopen(f1_name, "a+", stderr);
	if (f1 == NULL) {
 		printf("***ERROR freopen() %s for write+:\n%s\n", f1_name,strerror(errno));
 		exit (-1);
	} else {
       fprintf(stderr,"This stderr output should be in file %s.\n",f1_name);
    }

	puts("Rewind, reading, and showing contents of file");
	puts("using fgets()...");
	rewind(f1);
	
	if (fgets(s,MAXSTRLEN,f1) == NULL) {
		printf("***ERROR reading: %s.\n", strerror(errno));
		exit(-1);
	} else {
		puts(s);
	}

	if (fclose(f1) == -1) {
		printf("***ERROR closing %s:%s\n", f1_name, strerror(errno));
	} else {
		printf("Closed file %s\n", f1_name);
	}

	if (remove(f1_name)) {
		printf("***ERROR removing %s:%s\n", f1_name, strerror(errno));
		exit(-1);
	} else {
		printf("Removed file %s\n", f1_name);
	}

	puts("+----------------+");
	puts("| End of Test 16 |");
    puts("+----------------+");

	printf("\nPress any key to continue...");
	getchar();
	printf("\n\n");

	return 0;
}