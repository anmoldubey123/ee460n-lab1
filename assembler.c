/*
	Name 1: Anmol Dubey
	UTEID 1: ad56328
*/


#include <stdio.h> /* standard input/output library */
#include <stdlib.h> /* Standard C Library */
#include <string.h> /* String operations library */
#include <ctype.h> /* Library for useful character operations */
#include <limits.h> /* Library for definitions of common variable type characteristics */

#define MAX_LINE_LENGTH 255

enum {DONE, OK, EMPTY_LINE};

#define MAX_LABEL_LEN 20
#define MAX_SYMBOLS 255

typedef struct {
    int address;
    char label[MAX_LABEL_LEN + 1];
} TableEntry;

TableEntry symbolTable[MAX_SYMBOLS];
int numSymbols = 0;



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

int toRegister(char *ptr)
{
   int registerValue;
   if(ptr[0]=='r')
   {
      registerValue = ptr[1] - '0';
   }
   else 
   {
      exit(4);
   }
   return registerValue;
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

   int main(int argc, char *argv[])
   {
      int lc = 0;
      int startAddress = 0;

      char lLine[MAX_LINE_LENGTH + 1];
      char *lLabel, *lOpcode, *lArg1, *lArg2, *lArg3, *lArg4;
      int lRet;

      FILE *infile = NULL;
      FILE *outfile = NULL;

      infile = fopen(argv[1], "r");
      if (infile == NULL)
      {
         exit(4);
      }

      outfile = fopen(argv[2], "w");
      if (outfile == NULL)
      {
         exit(4);
      }

      /* PASS 1 */
      do
      {
         lRet = readAndParse(infile, lLine, &lLabel, &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4);

         if (lRet != DONE && lRet != EMPTY_LINE)
         {
            if (strcmp(lOpcode, ".orig") == 0)
            {
               lc = toNum(lArg1);
               startAddress = lc;
            }
            else if (strcmp(lOpcode, ".end") == 0)
            {
               break;
            }
            else
            {
               if (lLabel[0] != '\0')
               {
                  symbolTable[numSymbols].address = lc;
                  strcpy(symbolTable[numSymbols].label, lLabel);
                  numSymbols++;
               }
               lc += 2;
            }
         }
      } while (lRet != DONE);

      rewind(infile);

      lc = startAddress;
      fprintf(outfile, "0x%.4X\n", startAddress);

      /* PASS 2 */
      do
      {
         lRet = readAndParse(infile, lLine, &lLabel, &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4);

         if (lRet != DONE && lRet != EMPTY_LINE)
         {
            if (strcmp(lOpcode, ".orig") == 0)
            {
               /* already emitted */
            }
            else if (strcmp(lOpcode, ".end") == 0)
            {
               break;
            }
            else if (strcmp(lOpcode, ".fill") == 0)
            {
               int value = toNum(lArg1) & 0xFFFF;
               fprintf(outfile, "0x%.4X\n", value);
               lc += 2;
            }
            else
            {
               int opIndex = isOpcode(lOpcode);
               int instr = 0;

               if (strcmp(lOpcode, "add") == 0 || strcmp(lOpcode, "and") == 0 || strcmp(lOpcode, "xor") == 0)
               {
                  int dr = toRegister(lArg1);
                  int sr1 = toRegister(lArg2);

                  instr = (opcodeTable[opIndex].opcode << 12) | (dr << 9) | (sr1 << 6);

                  if (lArg3[0] == 'r')
                  {
                     instr |= toRegister(lArg3);
                  }
                  else
                  {
                     instr |= 1 << 5;
                     instr |= toNum(lArg3) & 0x1F;
                  }
               }
               else if (strcmp(lOpcode, "ldb") == 0 || strcmp(lOpcode, "ldw") == 0 || strcmp(lOpcode, "stb") == 0 || strcmp(lOpcode, "stw") == 0)
               {
                  int reg = toRegister(lArg1);
                  int baseR = toRegister(lArg2);
                  int offset = toNum(lArg3) & 0x3F;

                  instr = (opcodeTable[opIndex].opcode << 12) | (reg << 9) | (baseR << 6) | offset;
               }
               else if (strcmp(lOpcode, "lshf") == 0 || strcmp(lOpcode, "rshfl") == 0 || strcmp(lOpcode, "rshfa") == 0)
               {
                  int dr = toRegister(lArg1);
                  int sr = toRegister(lArg2);
                  int amount = toNum(lArg3) & 0xF;

                  instr = (opcodeTable[opIndex].opcode << 12) | (dr << 9) | (sr << 6) | amount;

                  if (strcmp(lOpcode, "rshfl") == 0)
                  {
                     instr |= 1 << 4;
                  }
                  else if (strcmp(lOpcode, "rshfa") == 0)
                  {
                     instr |= 1 << 4;
                     instr |= 1 << 5;
                  }
               }
               else if (strcmp(lOpcode, "nop") == 0)
               {
                  instr = 0x0000;
               }
               else if (strcmp(lOpcode, "rti") == 0)
               {
                  instr = 0x8000;
               }
               else if (strcmp(lOpcode, "ret") == 0)
               {
                  instr = 0xC1C0;
               }
               else if (strcmp(lOpcode, "halt") == 0)
               {
                  instr = 0xF025;
               }
               else if (strcmp(lOpcode, "trap") == 0)
               {
                  instr = (0xF << 12) | (toNum(lArg1) & 0xFF);
               }
               else if (strcmp(lOpcode, "jmp") == 0)
               {
                  instr = (opcodeTable[opIndex].opcode << 12) | (toRegister(lArg1) << 6);
               }
               else if (strcmp(lOpcode, "not") == 0)
               {
                  instr = (opcodeTable[opIndex].opcode << 12) | (toRegister(lArg1) << 9) | (toRegister(lArg2) << 6) | 0x3F;
               }
               else if (strcmp(lOpcode, "jsrr") == 0)
               {
                  instr = (opcodeTable[opIndex].opcode << 12) | (toRegister(lArg1) << 6);
               }

               fprintf(outfile, "0x%.4X\n", instr & 0xFFFF);
               lc += 2;
            }
         }
      } while (lRet != DONE);

      fclose(infile);
      fclose(outfile);
      return 0;
   }