/*
* BandPassFFTWFilter.cpp
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
* along with WIPP.  If not, see <http://www.gnu.org/licenses/>.
*/
//cppcheck-suppress zerodivcond


#include <dspone/BandPassFFTWFilter.h>
#include <dspone/BandPassFFTWFilterImpl.h>
//#include <dspone/FFTWeightingFilter.h>
//#include <dspone/DspException.h>
//#include <wipp/wipp.h>
//#include <math.h>

namespace dsp {

BandPassFFTWFilter::BandPassFFTWFilter(const int &order, const double &lowFreq, const double &highFreq, filterShape shape)
{
  _impl.reset(new BandPassFFTWFilterImpl(order, lowFreq, highFreq, shape));
}

BandPassFFTWFilter::BandPassFFTWFilter(const int &order, const double &lowFreq, const double &highFreq, const double &centerFreq)
{
  _impl.reset(new BandPassFFTWFilterImpl(order, lowFreq, highFreq, centerFreq));
}


BandPassFFTWFilter::~BandPassFFTWFilter()
{
  _impl.reset();
}


void BandPassFFTWFilter::filterBuffer(const int16_t *inbuffer, int16_t *outbuffer, int length)
{
  _impl->filterBuffer(inbuffer, outbuffer, length);
}

void BandPassFFTWFilter::filterBuffer(const double *inbuffer, double *outbuffer, int length)
{
  _impl->filterBuffer(inbuffer, outbuffer, length);
}

void BandPassFFTWFilter::filterBuffer(const float *inbuffer, float *outbuffer, int length)
{
  _impl->filterBuffer(inbuffer, outbuffer, length);
}


void BandPassFFTWFilter::getCoeficients(double *coefs, int length) const
{
  _impl->getCoeficients(coefs, length);
}


int BandPassFFTWFilter::getFrameLength() const
{
  _impl->getFrameLength();
}

double BandPassFFTWFilter::getLowFreq() const
{
  return _impl->getLowFreq();
}


double BandPassFFTWFilter::getHighFreq() const
{
  return _impl->getHighFreq();
}

double BandPassFFTWFilter::getCenterFreq() const
{
  return _impl->getCenterFreq();
}



}

