/*
* stft.cpp
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
#include <dspone/rt/STFTImpl.h>


namespace dsp {


// ------------ STFT -------------------------------------------------
STFTImpl::STFTImpl(int order)
{
  _fft.push_back(FFTImpl(order));
}

STFTImpl::STFTImpl(int nchannels, int order)
{
  for (size_t i = 0; i < nchannels; ++i)
    _fft.push_back(FFTImpl(order));
}

STFTImpl::~STFTImpl()
{

}

void STFTImpl::frameAnalysis(BaseType *inFrame, BaseType *analysis, int frameLength, int , int channel)
{
  _fft[channel].fwdTransform(inFrame, analysis, frameLength);
}

void STFTImpl::frameSynthesis(BaseType *outFrame, BaseType *analysis, int frameLength, int, int channel)
{
  _fft[channel].invTransfrom(outFrame, analysis, frameLength);
}


int STFTImpl::getFFTLength(int order)
{
  FFTImpl::getFFTLength(order);
}


int STFTImpl::getAnalysisWindowLength(int order)
{
  FFTImpl::getAnalysisWindowLength(order);
}

int STFTImpl::getAnalysisLength() const
{
  if (!_fft.empty())
    return _fft.at(0).getAnalysisWindowLength();
  else
    return 0;
}

int STFTImpl::getOrder() const
{
  if (!_fft.empty())
    return _fft.at(0).getOrder();
  else
    return 0;
}

int STFTImpl::getNumChannels() const
{
  return _fft.size();
}

int STFTImpl::getOneSidedFFTLength() const
{
  if (!_fft.empty())
    return _fft.at(0).getOneSidedFFTLength();
  else
    return 0;
}

}

