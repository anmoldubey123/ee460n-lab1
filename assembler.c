/*
	Name 1: Anmol Dubey
	UTEID 1: ad56328
*/


#include <stdio.h> /* standard input/output library */
#include <stdlib.h> /* Standard C Library */
#include <string.h> /* String operations library */
#include <ctype.h> /* Library for useful character operations */
#include <limits.h> /* Library for definitions of common variable type characteristics */



typedef struct {
    char *mnemonic;
    int opcode;
} OpcodeEntry;

OpcodeEntry opcodeTable[] = {
    {"add", 0x1}, 
    {"and", 0x5}, 
    {"br", 0x0},
    {"brp", 0x0},
    {"brz", 0x0},
    {"brzp", 0x0},
    {"brn", 0x0},
    {"brnp", 0x0},
    {"brnz", 0x0},
    {"brnzp", 0x0},
    {"halt", 0xF},
    {"jmp", 0xC},
    {"jsr", 0x4},
    {"jsrr", 0x4},
    {"ldb", 0x2}, 
    {"ldw", 0x6}, 
    {"lea", 0xE},
    {"nop", 0x0},
    {"not", 0x9},
    {"ret", 0xC},
    {"lshf", 0xD},
    {"rshfl", 0xD}, 
    {"rshfa", 0xD},
    {"rti", 0x8}, 
    {"stb", 0x3},
    {"stw", 0x7}, 
    {"trap", 0xF},
    {"xor", 0x9}
};

int isOpcode(char *ptr)
{
   int numOpcodes = sizeof(opcodeTable)/sizeof(opcodeTable[0]);
   for (int i = 0; i<numOpcodes; i++)
   {
      if(strcmp(ptr, opcodeTable[i].mnemonic)==0)
      {
         return i;
      }
   }

   return -1;
}

int main(void)
{
    printf("add    -> %d\n", isOpcode("add"));
    printf("br     -> %d\n", isOpcode("br"));
    printf("brnzp  -> %d\n", isOpcode("brnzp"));
    printf("start  -> %d\n", isOpcode("start"));
    printf(".orig  -> %d\n", isOpcode(".orig"));
    printf("ADD    -> %d\n", isOpcode("ADD"));
    return 0;
}

int toNum( char * pStr )
{
   char * t_ptr;
   char * orig_pStr;
   int t_length,k;
   int lNum, lNeg = 0;
   long int lNumLong;

   orig_pStr = pStr;
   if( *pStr == '#' )				/* decimal */
   { 
     pStr++;
     if( *pStr == '-' )				/* dec is negative */
     {
       lNeg = 1;
       pStr++;
     }
     t_ptr = pStr;
     t_length = strlen(t_ptr);
     for(k=0;k < t_length;k++)
     {
       if (!isdigit(*t_ptr))
       {
	 printf("Error: invalid decimal operand, %s\n",orig_pStr);
	 exit(4);
       }
       t_ptr++;
     }
     lNum = atoi(pStr);
     if (lNeg)
       lNum = -lNum;
 
     return lNum;
   }
   else if( *pStr == 'x' )	/* hex     */
   {
     pStr++;
     if( *pStr == '-' )				/* hex is negative */
     {
       lNeg = 1;
       pStr++;
     }
     t_ptr = pStr;
     t_length = strlen(t_ptr);
     for(k=0;k < t_length;k++)
     {
       if (!isxdigit(*t_ptr))
       {
	 printf("Error: invalid hex operand, %s\n",orig_pStr);
	 exit(4);
       }
       t_ptr++;
     }
     lNumLong = strtol(pStr, NULL, 16);    /* convert hex string into integer */
     lNum = (lNumLong > INT_MAX)? INT_MAX : lNumLong;
     if( lNeg )
       lNum = -lNum;
     return lNum;
   }
   else
   {
	printf( "Error: invalid operand, %s\n", orig_pStr);
	exit(4);  /* This has been changed from error code 3 to error code 4, see clarification 12 */
   }
}

#define MAX_LINE_LENGTH 255
	enum
	{
	   DONE, OK, EMPTY_LINE
	};

	int readAndParse( FILE * pInfile, char * pLine, char ** pLabel, char
	** pOpcode, char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4
	)
	{
	   char * lRet, * lPtr;
	   int i;
	   if( !fgets( pLine, MAX_LINE_LENGTH, pInfile ) )
		return( DONE );
	   for( i = 0; i < strlen( pLine ); i++ )
		pLine[i] = tolower( pLine[i] );
	   
           /* convert entire line to lowercase */
	   *pLabel = *pOpcode = *pArg1 = *pArg2 = *pArg3 = *pArg4 = pLine + strlen(pLine);

	   /* ignore the comments */
	   lPtr = pLine;

	   while( *lPtr != ';' && *lPtr != '\0' &&
	   *lPtr != '\n' ) 
		lPtr++;

	   *lPtr = '\0';
	   if( !(lPtr = strtok( pLine, "\t\n ," ) ) ) 
		return( EMPTY_LINE );

	   if( isOpcode( lPtr ) == -1 && lPtr[0] != '.' ) /* found a label */
	   {
		*pLabel = lPtr;
		if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );
	   }
	   
           *pOpcode = lPtr;

	   if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );
	   
           *pArg1 = lPtr;
	   
           if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

	   *pArg2 = lPtr;
	   if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

	   *pArg3 = lPtr;

	   if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

	   *pArg4 = lPtr;

	   return( OK );
	}

	/* Note: MAX_LINE_LENGTH, OK, EMPTY_LINE, and DONE are defined values */