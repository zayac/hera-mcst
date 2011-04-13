/* 
 * File:   MIF_Output.h
 * Author: zayac
 *
 * Created on April 14, 2011, 4:27 AM
 */

#ifndef _MIF_OUTPUT_H
#define	_MIF_OUTPUT_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class MIF_Output {
public:
    enum radix { BIN, DEC, HEX, OCT, UNS };
    MIF_Output(unsigned depth = 16, unsigned width = 16, enum radix address = HEX, enum radix data = HEX);
    
    template <class T>
    void generateObjectFile(string fileName, vector<T> instructions);
    
    virtual ~MIF_Output();
private:
    unsigned depth;
    unsigned width;
    radix address;
    radix data;
};

template <class T>
void MIF_Output::generateObjectFile(string fileName, vector<T> instructions)
{
    ofstream ofile;
    ofile.open(fileName.c_str());

    string saddr;
    if (address == HEX)
        saddr = "HEX";
    else if (address == DEC )
        saddr = "DEC";
    else if (address == OCT )
        saddr = "OCT";
    else if (address == BIN )
        saddr = "BIN";
    else
        saddr = "UNS";
    ofile << "DEPTH = " << depth << ";";
    //cout << "\t% Memory depth and width are required %";
    ofile << endl;
    //ofile << "\t\t % DEPTH is the number of addresses %" << endl;
    ofile << "WIDTH = " << width << ";";
    //cout << "\t% WIDTH is the number of bits of data per word %" << endl;
    ofile << endl;
    //ofile << "% DEPTH and WIDTH should be entered as decimal numbers %" << endl << endl;
    ofile << "ADDRESS_RADIX = " << saddr << ";";
    //cout << "\t% Address and value radixes are required %" << endl;
    ofile << endl;
    if (data == HEX)
        saddr = "HEX";
    else if (data == DEC )
        saddr = "DEC";
    else if (data == OCT )
        saddr = "OCT";
    else if (data == BIN )
        saddr = "BIN";
    else
        saddr = "UNS";
    ofile << "DATA_RADIX = " << data << ";" << endl;
    //cout << "\t% Enter BIN, DEC, HEX, OCT, or UNS; unless %" << endl;
    //ofile << "\t\t% otherwise specified, radixes = HEX %" << endl;
    //ofile << "-- Specify value for addresses, which can be single address or range" << endl;
    ofile << "CONTENT" << endl;
    ofile << "BEGIN" << endl;
    for(unsigned i = 0; i < instructions.size(); i ++)
    {
        ofile << hex << i * width << "\t:\t";
        if (data == HEX)
            ofile << hex << instructions[i];
        else if (data == DEC)
            ofile << dec << instructions[i];
        else if (data == OCT)
            ofile << oct << instructions[i];
        else if (data == BIN)
        {
            T n = instructions[i];
            const size_t size = sizeof(n) * 8;
            char result[size];

            unsigned index = size;
            do {
                result[--index] = '0' + (n & 1);
            } while (n >>= 1);

            ofile << std::string(result + index, result + size);
        }
        ofile << ";" << endl;
    }
    //ofile << "--" << endl;
    ofile << "END;" << endl;
}


#endif	/* _MIF_OUTPUT_H */

