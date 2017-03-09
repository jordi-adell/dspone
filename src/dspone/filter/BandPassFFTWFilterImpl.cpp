/*
* BandPassFFTWFilterImpl.cpp
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
//cppcheck-suppress zerodivcond

#include <dspone/filter/BandPassFFTWFilterImpl.h>
#include <dspone/filter/FFTWeightingFilter.h>
#include <dspone/DspException.h>
#include <wipp/wipp.h>
#include <wipp/wipputils.h>
#include <wipp/wippsignal.h>
#include <wipp/wippstats.h>
#include <math.h>

namespace dsp {

  BandPassFFTWFilterImpl::BandPassFFTWFilterImpl(const int &order, const double &lowFreq,
						 const double &highFreq, BandPassFFTWFilter::filterShape shape) :
    _shape(shape)
  {
    initialiseFilter(order, lowFreq, highFreq, (lowFreq+highFreq)/2);
  }

  BandPassFFTWFilterImpl::BandPassFFTWFilterImpl(const int &order, const double &lowFreq, const double &highFreq,
						 const double &centerFreq) :
    _shape(BandPassFFTWFilter::TRIANGULAR)
  {
    initialiseFilter(order, lowFreq, highFreq, centerFreq);
  }


  BandPassFFTWFilterImpl::~BandPassFFTWFilterImpl()
  {

  }

  void BandPassFFTWFilterImpl::initialiseFilter(const int &order, const double &lowFreq, const double &highFreq,
						const double &centerFreq)
  {
    _lowFreq = lowFreq;
    _highFreq = highFreq;
    _centerFreq = centerFreq;
    _order = order;
    _length = 1 << _order;
    _coefsLength = _length/2 + 1; // Because of FFT symetry in real signals.
    _coefs.reset(new BaseType[_coefsLength]);
    wipp::setZeros(_coefs.get(), _coefsLength);
    initialiseFilterCore();
  }


  void BandPassFFTWFilterImpl::setFilterShape(BaseType *coefs, const int &length, const int &startId, const int &endId, const int &centerId) const
  {
    // Currently I am using a triangular window in the frequency domain as a filter
    // This is usefull for mel-scale analysis. The use of assymetry is to
    // ensure the sum of all the filters in a filter bank can be configured to be a constant.
    //    setTriangularFilterShape(coefs, length, startId, endId, centerId);
    switch (_shape)
    {
      case BandPassFFTWFilter::RECTANGULAR:
	setRectangularFilterShape(coefs, length, startId, endId);
      break;
      case BandPassFFTWFilter::TRIANGULAR:
	setTriangularFilterShape(coefs, length, startId, endId, centerId);
      break;
      default:
	throw(DspException("Undefined filter shape in FFT weiting filter bank"));
    }
  }

  void BandPassFFTWFilterImpl::setTriangularFilterShape(BaseType *coefs, const int &length, const int &startId, const int &endId) const
  {
    int centerId =  ((float) (endId + startId))/2 + 0.5;
    setTriangularFilterShape(coefs, length, startId, endId, centerId);
  }

  void BandPassFFTWFilterImpl::setTriangularFilterShape(BaseType *coefs, const int &length, const int &startId, const int &endId , const int &centerId) const
  {
    int triangleLength = endId - startId;
    if (triangleLength <= 0)
      return;

    // cppcheck-suppress zerodivcond
    float triangleFreq = 1.0F/(2*triangleLength);
    int idealCenter = (static_cast<double>(endId + startId))/2 + 0.5;
    // cppcheck-suppress zerodivcond
    double triangleAsym = (centerId - idealCenter)*2*M_PI/triangleLength;
    double trianglePhase = 0;

    if (triangleFreq >= 0.5)
    {
      if (triangleLength == 0)
	WARN_STREAM("Band-pass filter with a band bas of " << triangleLength << " samples in the FFT. In fact, is a nothing-pass filter." <<
		    "This is normal if this filter is part of a mel-scale filter bank, otherwise may indicate serious problems.");
	triangleLength = 2;
    }

    if (trianglePhase < 0 || trianglePhase >= 2*M_PI)
      trianglePhase = 0;

    wipp::set(0, coefs, length);
    wipp::triangle(&coefs[startId], triangleLength, triangleLength, trianglePhase, triangleAsym, 0);

    // We adjust the maximum,
    // to minimise the problems of overlaping triangles not summing 1
    double max=0; size_t idx=0;
    wipp::maxidx(&coefs[startId], triangleLength, &max, &idx);
    if (max != 0)
      (&coefs[startId])[idx] /= max;
  }


  void BandPassFFTWFilterImpl::setRectangularFilterShape(BaseType *coefs, const int &length, const int &startId, const int &endId) const
  {
    wipp::setZeros(coefs, length);
    wipp::set(1, &coefs[startId], endId - startId);
  }

  void BandPassFFTWFilterImpl::initialiseFilterCore()
  {

    if (_highFreq < 0 || _lowFreq < 0 || _lowFreq > 0.5 || _highFreq > 0.5)
    {
	std::string msg("Cut-off frequencies need to be in the Nyquist range [0 - 0.5]:  ");
	msg += std::to_string(_lowFreq) + ", " + std::to_string(_highFreq);
	throw(DspException(msg));
      }

    // I reformulate the cut-off frequencies because of integer indexes in the FFT
    // makes them fall to the nearest possible.
    // +2 here is for CCs IPP compact format since onsided FFT size is 2^(order-1)+1
    int lowFreqId = _lowFreq * (_length+2);
    int highFreqId = _highFreq * (_length+2);
    int centerFreqId = _centerFreq * (_length+2);
    _lowFreq = lowFreqId*1.0F / (_length+2);
    _highFreq = highFreqId*1.0F / (_length+2);
    _centerFreq = centerFreqId*1.0F / (_length+2);

    // Here we set the filter shape
    setFilterShape(_coefs.get(), _coefsLength, lowFreqId, highFreqId, centerFreqId);
    _fftwFilter.reset(new FFTWeightingFilter(_coefs.get(), _coefsLength));
  }

  void BandPassFFTWFilterImpl::filterBuffer(const int16_t *inbuffer, int16_t *outbuffer, int length)
  {
    filterBufferCore(inbuffer, outbuffer, length);
  }

  void BandPassFFTWFilterImpl::filterBuffer(const int32_t *inbuffer, int32_t *outbuffer, int length)
  {
    filterBufferCore(inbuffer, outbuffer, length);
  }

  void BandPassFFTWFilterImpl::filterBuffer(const uint16_t *inbuffer, uint16_t *outbuffer, int length)
  {
    filterBufferCore(inbuffer, outbuffer, length);
  }


  void BandPassFFTWFilterImpl::filterBuffer(const double *inbuffer, double *outbuffer, int length)
  {
    filterBufferCore(inbuffer, outbuffer, length);
  }

  void BandPassFFTWFilterImpl::filterBuffer(const float *inbuffer, float *outbuffer, int length)
  {
    filterBufferCore(inbuffer, outbuffer, length);
  }

  template <class T> void BandPassFFTWFilterImpl::filterBufferCore(const T *inbuffer, T *outbuffer, int length)
  {
    // Remember the length of the buffer must be equal to _length
    _fftwFilter->filterBuffer(inbuffer, outbuffer, length);
  }

  void BandPassFFTWFilterImpl::spectralWeighting(double *spectrum, int length) const
  {
    _fftwFilter->spectralWeighting(spectrum, length);
  }

  void BandPassFFTWFilterImpl::fft(const double *signal, int signallength, double *spectrum, int speclength) const
  {
    _fftwFilter->fft(signal, signallength, spectrum, speclength);
  }

  void BandPassFFTWFilterImpl::ifft(const double *spectrum, int speclength, double *signal, int signallength) const
  {
    _fftwFilter->ifft(spectrum, speclength, signal, signallength);
  }

  void BandPassFFTWFilterImpl::getCoeficients(double *coefs, int length) const
  {
    if (length > _coefsLength)
      length = _coefsLength;

    for (int i=0; i<length; ++i)
    {
      coefs[i] = _coefs[i];
    }
  }


  void BandPassFFTWFilterImpl::checkIPPStatus(int status) const
  {
    if (status)
      {
	throw(DspException(std::to_string(status)));
      }
  }


  int BandPassFFTWFilterImpl::getFrameLength() const
  {
    return _length;
  }



}

