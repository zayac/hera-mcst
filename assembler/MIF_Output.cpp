/* 
 * File:   MIF_Output.cpp
 * Author: zayac
 * 
 * Created on April 14, 2011, 4:27 AM
 */

#include "MIF_Output.h"


MIF_Output::MIF_Output(unsigned depth, unsigned width, enum radix address, enum radix data)
{
    this->width = width;
    this->depth = depth;
    this->address = address;
    this->data = data;
}


MIF_Output::~MIF_Output() {

}


