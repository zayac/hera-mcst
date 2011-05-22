/***********************************
* Prorgam converting .mif file to
* "com format" for sending through
* com port
*
* written by Lavrov Alexey
* last modification: 19.05.11
************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define NUMBER_PARAM 2  //during staring the program
#define COMM_LENGTH 4   //length of command in com format
enum {ENABLE, DISABLE};

void print_usage(void);

int main(int argc, void *argv[])
{
  int count, cl;  //cl - command length
  char *infile, *outfile;
  char ch, wren; // ch - current charater, wren - write enable for output
  char end_tr[] = "0000"; //end of the transmission for com module
  FILE *fpin, *fpout;

  if (argc != NUMBER_PARAM + 1) {
    print_usage();
    exit(1);
  }
  infile = argv[1];
  outfile = argv[2];

  fpin = fopen(infile, "r");
  fpout = fopen(outfile, "w");
  if (fpin == NULL) {
    fprintf(stderr, "Error during opening %s\n", infile);
    exit(1);
  }
  if (fpout == NULL) {
    fprintf(stderr, "Error during opening %s\n", outfile);
    exit(1);
  }

  wren = DISABLE;
  count = 0;
  cl = COMM_LENGTH;
  while((ch = fgetc(fpin)) != EOF) {
    if (ch == ':') //we write command after ':' in the line
      wren = ENABLE;
    if (wren == ENABLE) {
      if (((ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F') || \
          (ch >= '0' && ch <= '9')) && count < cl) {
        fprintf(fpout, "%c", ch);
        count++;
        //printf(" count= %d ", count);
      }
      else if (ch == '\n') {
        wren = DISABLE;
        count = 0;
        fprintf(fpout, "\n");
      }
    }
  }
  fprintf(fpout, "%s\n", end_tr); //end of transmission for com module

  fclose(fpin);
  fclose(fpout);
  return 0;
}

/* prints usage of program */
void print_usage(void)
{
  fprintf(stdout, "USAGE: set input and output files\n");
  fprintf(stdout, "example: mif_to_com.c input.mif output.txt\n");
}
