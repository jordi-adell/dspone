/*
* fft.cpp
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
#include <dspone/algorithm/fft.h>
#include <dspone/algorithm/fftImpl.h>
#include <wipp/wipputils.h>


namespace dsp {

FFT::FFT(int order)
{
  _impl.reset(new FFTImpl(order));
}

FFT::~FFT()
{

}

int FFT::getAnalysisWindowLength() const
{
  return _impl->getAnalysisWindowLength();
}

int FFT::getAnalysisWindowLength(int order)
{
  return  FFTImpl::getAnalysisWindowLength(order);
}

int FFT::getFFTLength(int order)
{
  return FFTImpl::getFFTLength(order);
}

int FFT::getFFTLength() const
{
  return _impl->getFFTLength();
}

int FFT::getOneSidedFFTLength() const
{
  return _impl->getOneSidedFFTLength();
}

int FFT::getOrder() const
{
  return _impl->getOrder();
}

void FFT::fwdTransform(const BaseType *inFrame, BaseType *fft, int frameLength)
{
  _impl->fwdTransform(inFrame, fft, frameLength);
}


void FFT::invTransfrom(BaseType *outFrame, const BaseType *fft, int frameLength)
{
  _impl->invTransfrom(outFrame, fft, frameLength);
}

void FFT::fwdTransform(const BaseType *inFrame, BaseType *fft)
{
  _impl->fwdTransform(inFrame, fft);
}

void FFT::invTransfrom(BaseType *outFrame, const BaseType *fft)
{
  _impl->invTransfrom(outFrame, fft);
}

}


