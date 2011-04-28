#include "MIFReader.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>
#define SIZE_STRING 1024
#define ERROR_IN_LINE { cout << "Unable to read mif headers: line " << numberline << " col " << i+1 << endl; return 1; }
using namespace std;

MIFReader::MIFReader(unsigned short int *memory, unsigned int *Size_Program)
:_memory (memory), _Size_Program (Size_Program){};

bool MIFReader::read (ELF_Input* file){

    int DEPTH, WIDTH, ADDRESS_RADIX, DATA_RADIX;
    char line[SIZE_STRING];
    char *end;
    int flag = 0;
    int i;
    unsigned int temporary;
    int len;
    int numberline = 0;
    while( flag < 6 ){
        if(!file->getline(line, 1024, '\n')) ERROR_IN_LINE
        numberline++;
        len = strlen(line);
        if(len == 0) continue;

        for(i = 0; (flag > 3) && (i < len) && (line[i] != '\0') && ( line[i]!='-' || line[i+1]!='-') ; i++){
            if( (flag == 5) && !memcmp(&line[i], "BEGIN", 5)){
                //cout << "BEGIN" << endl;
                if(i+5 <= len) i+=4; else ERROR_IN_LINE
                flag++;
                continue;
            }
            else if( (flag == 4) && !memcmp(&line[i], "CONTENT", 7)){
                //cout << "CONTENT" << endl;
                if(i+7 <= len) i+=6; else ERROR_IN_LINE
                flag++;
                continue;
            }
            else if(!isspace(line[i])) ERROR_IN_LINE
        }

        for(i = 0; (flag < 4) && (i < len) && (line[i] != '\0') && ( line[i]!='-' || line[i+1]!='-') ; i++){
            if(!memcmp(&line[i], "DEPTH", 5)){
                if(i+5 <= len) i+=5; else ERROR_IN_LINE
                while(isspace(line[i])) i++;
                if(line[i] == '=') DEPTH = strtol(&line[i+1], &end, 10); else ERROR_IN_LINE
                i += (end-&line[i])/sizeof(char);
                //cout << "DEPTH = " << DEPTH << endl;
                while(isspace(line[i])) i++;
                if(line[i] != ';') ERROR_IN_LINE
                flag++;
                continue;
            }
            else if(!memcmp(&line[i], "WIDTH", 5)){
                if(i+5 <= len) i+=5; else ERROR_IN_LINE
                while(isspace(line[i])) i++;
                if(line[i] == '=') WIDTH = strtol(&line[i+1], &end, 10); else ERROR_IN_LINE
                i += (end-&line[i])/sizeof(char);
                //cout << "WIDTH = " << WIDTH << endl;
                while(isspace(line[i])) i++;
                if(line[i] != ';') ERROR_IN_LINE
                flag++;
                continue;
            }
            else if(!memcmp(&line[i], "ADDRESS_RADIX", 13)){
                if(i+13 <= len) i+=13; else ERROR_IN_LINE
                while(isspace(line[i])) i++;
                if(line[i] == '=') i++; else ERROR_IN_LINE
                while(isspace(line[i])) i++;
                if(!memcmp(&line[i], "BIN", 3)) ADDRESS_RADIX = 2;
                else if(!memcmp(&line[i], "OCT", 3)) ADDRESS_RADIX = 8;
                else if(!memcmp(&line[i], "DEC", 3)) ADDRESS_RADIX = 10;
                else if(!memcmp(&line[i], "HEX", 3)) ADDRESS_RADIX = 16;
                else ERROR_IN_LINE
                i+=3;
                //cout << "ADDRESS_RADIX = " << ADDRESS_RADIX << endl;
                while(isspace(line[i])) i++;
                if(line[i] != ';') ERROR_IN_LINE
                flag++;
                continue;
            }
            else if(!memcmp(&line[i], "DATA_RADIX", 10)){
                if(i+10 <= len) i+=10; else ERROR_IN_LINE
                while(isspace(line[i])) i++;
                if(line[i] == '=') i++; else ERROR_IN_LINE
                while(isspace(line[i])) i++;
                if(!memcmp(&line[i], "BIN", 3)) DATA_RADIX = 2;
                else if(!memcmp(&line[i], "OCT", 3)) DATA_RADIX = 8;
                else if(!memcmp(&line[i], "DEC", 3)) DATA_RADIX = 10;
                else if(!memcmp(&line[i], "HEX", 3)) DATA_RADIX = 16;
                else ERROR_IN_LINE
                i+=3;
                //cout << "DATA_RADIX = " << DATA_RADIX << endl;
                while(isspace(line[i])) i++;
                if(line[i] != ';') ERROR_IN_LINE
                flag++;
                continue;
            }
            else if(!isspace(line[i])) ERROR_IN_LINE
        }

    }
    if(flag < 6){ cout << "Unable to read mif headers: not enough parameters or incorrect sequence" << endl; return 1; }

    while( flag == 6 ){
        if(!file->getline(line, 1024, '\n')) ERROR_IN_LINE
        numberline++;
        len = strlen(line);
        if(len == 0) continue;

        for(i = 0; (i < len) && (line[i] != '\0') && ( line[i]!='-' || line[i+1]!='-') ; i++){
            if(!memcmp(line, "END", 3)){*_Size_Program = temporary+1; /*cout << "END" << endl;*/ flag = 0; break; }
            else if(isxdigit(line[i])){
                temporary = strtol(&line[i], &end, ADDRESS_RADIX);
                i += (end-&line[i])/sizeof(char);
                while(isspace(line[i])) i++;
                if(line[i] != ':') ERROR_IN_LINE
                do i++; while(isspace(line[i]));
                if(isxdigit(line[i])) _memory[temporary] = strtol(&line[i], &end, DATA_RADIX);
                i += (end-&line[i])/sizeof(char);
                while(isspace(line[i])) i++;
                if(line[i] != ';') ERROR_IN_LINE
//cout << hex << temporary << '\t' << cmd << endl;
                continue;
            }
            else if(!isspace(line[i])) ERROR_IN_LINE
        }
    }
    flag = 0;
	return 0;
}
