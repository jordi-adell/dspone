/*
* FFTWeightingFilter.cpp
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
#include <dspone/filter/FFTWeightingFilter.h>
#include <dspone/DspException.h>
#include <dspone/dsplogger.h>

#include <dspone/algorithm/fft.h>

#include <dspone/dspdefs.h>

#include <wipp/wipputils.h>

#include <math.h>

namespace dsp {

FFTWeightingFilter::FFTWeightingFilter(const double *coefs, int length) :
  _order(0),  _length(0),  _specLength(0),   _coefsLength(0)
{
 // @TODO add a type check before the cast.
  initialiseFilter(reinterpret_cast<const BaseType*>(coefs), length);
}


FFTWeightingFilter::~FFTWeightingFilter()
{
  _fft.reset();
}

void FFTWeightingFilter::initialiseFilter(const BaseType *coefs, int length)
{
  if (length <= 0)
    return;
  _order = (int) log2(length-1) + 1;
  _coefsLength = pow(2,_order-1) + 1;
  if (length != _coefsLength)
    throw(DspException("The length of the spectral weight coeficients needs to be power of 2 plus ones for eficient FFT processing"));

  _length = 1 << _order;
  _specLength = _length + 2; // This is beacuse the CCs compact form in IPP nees 2 more samples to store the whole spectrum.
  _spectrum.reset(new BaseType[_specLength]);
  _coefs.reset(new BaseType[2*_coefsLength]);
  wipp::real2complex(coefs, NULL, reinterpret_cast<wipp::wipp_complex_t*>(_coefs.get()), _coefsLength);

  _fft.reset(new FFT(_order));

}

void FFTWeightingFilter::filterBuffer(const double *inbuffer, double *outbuffer, int length)
{
  filterBufferCore(inbuffer, outbuffer, length);
}

void FFTWeightingFilter::filterBuffer(const float *inbuffer, float *outbuffer, int length)
{
  BaseType in64buffer[_length];
  BaseType out64buffer[_length];

  wipp::copyBuffer(inbuffer, in64buffer, length);
  filterBufferCore(in64buffer, out64buffer, length);
  wipp::copyBuffer(out64buffer, outbuffer, length);
}

void FFTWeightingFilter::filterBuffer(const int16_t *inbuffer, int16_t *outbuffer, int length)
{
  BaseType in64buffer[_length];
  BaseType out64buffer[_length];

  wipp::copyBuffer(inbuffer, in64buffer, length);
  filterBufferCore(in64buffer, out64buffer, length);
  wipp::copyBuffer(out64buffer, outbuffer, length);
}

void FFTWeightingFilter::filterBuffer(const uint16_t *inbuffer, uint16_t *outbuffer, int length)
{
  BaseType in64buffer[_length];
  BaseType out64buffer[_length];

  wipp::copyBuffer(inbuffer, in64buffer, length);
  filterBufferCore(in64buffer, out64buffer, length);
  wipp::copyBuffer(out64buffer, outbuffer, length);
}

inline void FFTWeightingFilter::filterBufferCore(const BaseType *inbuffer, BaseType *outbuffer, int length) const
{
  if (length != _length)
    throw(DspException("The buffer length has to be " + std::to_string(_length)));
  fft(inbuffer, _length, _spectrum.get(), _specLength);

  spectralWeighting(_spectrum.get(), _specLength);
  ifft(_spectrum.get(), _specLength, outbuffer, _length);
}

void FFTWeightingFilter::spectralWeighting(BaseType *spectrum, int length) const
{
  if (length != _specLength)
    throw(DspException("The buffer spectrum has to be " + std::to_string(_coefsLength)));

  wipp::mult(reinterpret_cast<wipp::wipp_complex_t*>(_coefs.get()), reinterpret_cast<wipp::wipp_complex_t*>(spectrum), _coefsLength);

}

void FFTWeightingFilter::fft(const double *signal, int signallength, BaseType *spectrum, int speclength) const
{
  if (speclength != _specLength ||  signallength != _length)
    throw DspException("Either the signal or the spectrum buffer lengths are wrong");

  _fft->fwdTransform(signal, spectrum, signallength);
}

void FFTWeightingFilter::ifft(const BaseType *spectrum, int speclength, double *signal, int signallength) const
{
  if (speclength != _specLength ||  signallength != _length)
    throw DspException("Either the signal or the spectrum buffer legths are wrong");
  _fft->invTransfrom(signal, spectrum, signallength);
}


}

