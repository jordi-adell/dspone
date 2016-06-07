/*
* fftImpl.cpp
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
#include "fftImpl.h"
#include <dspone/dsplogger.h>
#include <wipp/wipputils.h>

namespace dsp {

FFTImpl::FFTImpl(int order) :
  _fftOrder(order),
  _fftLength(FFTImpl::getFFTLength(order)), // (+2) because of the CCS format in IPP library.
  _oneSidedFFTLength(_fftLength/2)
{
  init();
}

FFTImpl::~FFTImpl()
{
  wipp::delete_fft(&_fftspec);
}

void FFTImpl::init()
{
  wipp::init_fft(&_fftspec, 1 <<_fftOrder);
  _frame.reset(new BaseType[_fftLength]);
}


void FFTImpl::fwdTransform(const BaseType *inFrame, BaseType *fft, int frameLength)
{
  if (frameLength > _fftLength)
  {
    ERROR_STREAM("Frame length larger than FFT length (2^order +2 )");
    wipp::setZeros(reinterpret_cast<wipp::wipp_complex_t*>(fft), _fftLength);
  }
  else if (frameLength == _fftLength)
  {
    fwdTransform(inFrame, fft);
  }
  else
  {
    wipp::setZeros(_frame.get(), _fftLength);
    wipp::copyBuffer(inFrame, _frame.get(), frameLength);
    fwdTransform(_frame.get(), fft);
  }
}


void FFTImpl::invTransfrom(BaseType *outFrame, const BaseType *fft, int frameLength)
{

  if (frameLength == _fftLength)
  {
    invTransfrom(outFrame, fft);
  }
  else
  {
    if (frameLength > _fftLength)
    {
      ERROR_STREAM("You cannot get a signal longer than its FFT transofrm. You'll get only the first "
		       << _fftLength << " samples rather than the " << frameLength << " you requested.");
    }
    invTransfrom(_frame.get(), fft);
    wipp::copyBuffer(_frame.get(), outFrame, frameLength);
  }
}

void FFTImpl::fwdTransform(const BaseType *inFrame, BaseType *fft)
{
  wipp::fft(inFrame, fft, _fftspec);
}

void FFTImpl::invTransfrom(BaseType *outFrame, const BaseType *fft)
{
  wipp::ifft(fft, outFrame, _fftspec);
}

}


