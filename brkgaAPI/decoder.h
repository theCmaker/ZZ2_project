#pragma once

#include <vector>
#include "data.h"
#include "front.h"

// the decoder to be used in BRKGA
class Decoder
{
  protected:
    Data & data;
	Front front;

  public:
    Decoder (Data & d): data(d) {}
    double decode (std::vector<double> & chromosome);
	const Front & getFront() const {return front;}
};
