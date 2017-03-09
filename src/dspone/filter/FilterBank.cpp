/*
* FilterBank.cpp
* Copyright 2016 (c) Jordi Adell
* Created on: 2015
* 	Author: Jordi Adell - adellj@gmail.com
*
* This file is part of DSPONE
*
* DSPONE is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DSPONE is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* alogn with DSPONE.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <dspone/filter/FilterBank.h>
#include <dspone/dsplogger.h>
#include <dspone/DspException.h>
#include <wipp/wipputils.h>

#include <math.h>

#include <iostream>

namespace dsp {

FilterBank::FilterBank(int order, const std::vector<double> &centerFreqs) : _centerFreqs(centerFreqs), _order(order)
{

}

FilterBank::FilterBank(const FilterBank &fbank) :
  _centerFreqs(fbank._centerFreqs),
  _order(fbank._order),
  _filterBank(fbank._filterBank)
{

}

FilterBank::FilterBank() : _order(-1)
{

}


int FilterBank::getFiltersCoeficients(double *coefs, int length) const
{
  FilterVector::const_iterator it;
  int offset = 0;
  int delta = _order; //  (1 << (_order - 1)) + 1;
  int ncoefs = 0;
  for (it = _filterBank.begin(); it != _filterBank.end(); ++it, offset += delta)
  {
    if ((offset + delta) > length)
      break;
    it->get()->getCoeficients(&coefs[offset], delta);
    ncoefs += delta;
  }
  return ncoefs;
}

void FilterBank::filterBuffer(int16_t *inbuffer, int16_t *outbuffer, int inlength, int outlength)
{
  filterBufferCore(inbuffer, outbuffer, inlength, outlength);
}

void FilterBank::filterBuffer(double *inbuffer, double *outbuffer, int inlength, int outlength)
{
  filterBufferCore(inbuffer, outbuffer, inlength, outlength);
}

void FilterBank::filterBuffer(float *inbuffer, float *outbuffer, int inlength, int outlength)
{
  filterBufferCore(inbuffer, outbuffer, inlength, outlength);
}





template <class T> void FilterBank::filterBufferCore(T *inbuffer, T *outbuffer, int inlength, int outlength)
{

  if (outlength < inlength*getNBins())
    throw(DspException("output buffer's length needs to be equal or larger thean (inlength x NBins)"));

  int outOffset = 0;
  for (auto it = _filterBank.begin(); it != _filterBank.end(); ++it, outOffset += inlength)
  {
    (*it)->filterBuffer(inbuffer, &outbuffer[outOffset], inlength);
  }

}


}
