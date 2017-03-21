/*
* BandPassFIRFilter.cpp
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
/*
 *  BandPassFIRFilter.cpp
 *  Copyright (c) 2016
 *  Created on: 2016
 *      Author:  Jordi Adell (adellj@gmail.com)
 */
#include <dspone/filter/BandPassFIRFilter.h>
#include <dspone/DspException.h>
#include <dspone/dspdefs.h>

#include <wipp/wipputils.h>
#include <wipp/wippfilter.h>
#include <wipp/wippfft.h>
#include <wipp/wippsignal.h>

#include <sstream>
#include <math.h>


namespace dsp {

class BandPassFIRFilterImpl
{
  public:
    BandPassFIRFilterImpl(const int order, BandPassFIRFilter::FrequencyShape shape):
      _order(order),
      _shape(shape),
      _winType(wipp::wippHAMMING)
    {
    }

    ~BandPassFIRFilterImpl()
    {
      _coefs.reset();
      _firFilter.reset();
    }

    const int _order;
    const BandPassFIRFilter::FrequencyShape _shape;

    /** Actual implementation of the filter, using the FIR technique **/
    std::unique_ptr<FIRFilter> _firFilter;
    boost::shared_array<BandPassFIRFilter::BaseType> _coefs;

    const wipp::wipp_window_t _winType;
};

BandPassFIRFilter::BandPassFIRFilter(const int order, const double lowFreq, const double highFreq, FrequencyShape shape):
  _impl(new BandPassFIRFilterImpl(order, shape)),
  _lowFreq(lowFreq), _highFreq(highFreq)
{
  checkParameters();
  initialiseFilter();
}


BandPassFIRFilter::~BandPassFIRFilter()
{
  _impl.reset();
}


void BandPassFIRFilter::initialiseFilter()
{
  _impl->_coefs.reset(new BaseType[_impl->_order]);
  wipp::setZeros(_impl->_coefs.get(), _impl->_order);

  switch(_impl->_shape)
  {
    case RECTANGULAR:
      initialiseRectangularCoefs();
    break;
    case TRIANGULAR:
      initialiseTriangularCoefs();
    break;
  }
  _impl->_firFilter.reset(new FIRFilter(static_cast<const double*>(_impl->_coefs.get()), _impl->_order));
}

void BandPassFIRFilter::initialiseRectangularCoefs()
{
  int status = wipp::fir_coefs( _lowFreq,_highFreq, _impl->_coefs.get(), _impl->_order, _impl->_winType);
  if (status)
  {
    std::string msg = "I had problems while initialising a band pass FIR filter.";
    throw(DspException(msg));
  }
}

void BandPassFIRFilter::initialiseTriangularCoefs()
{
  initialiseTriangularCoefs_sinc2();
}


void BandPassFIRFilter::initialiseTriangularCoefs_invfilt()
{

  BaseType freqShape[_impl->_order];
  BaseType spectrum[_impl->_order+2]; // CCS format (see IPP doc for help)
  wipp::setZeros(freqShape, _impl->_order);
  wipp::setZeros(freqShape, _impl->_order);
  wipp::setZeros(spectrum, _impl->_order+2);

  int lowfreq = _lowFreq*_impl->_order;
  int highfreq = _highFreq*_impl->_order;
  if (lowfreq == highfreq)
  {
    std::ostringstream oss;
    oss << "Filter order " << _impl->_order << " is too low for the cut-off frequencies you are requesting: "
	<< "(" << _lowFreq << ", " << _highFreq << ")";
    throw(DspException(oss.str()));
  }

  double logOrder = (int) log2(_impl->_order);
  double roundedOrder = pow(2,logOrder);
  if (_impl->_order != roundedOrder)
    throw(DspException("The order of the filter needs to be power of 2 for efficient FFT processing if TRIANGULAR shape has been chosen"));

  // Create a triangular shape
  int length = highfreq - lowfreq;
  wipp::triangle(&freqShape[lowfreq],  highfreq - lowfreq, length/2, 0 ,0);

  // Create specrtum in CCS format.
  int i,j;
  for (i=0, j=0;  i < _impl->_order/2+1; i++, j=j+2)
  {
    spectrum[j] = freqShape[i];
    spectrum[j] =   freqShape[i]*cos(static_cast<double>(i)*M_PI);
    spectrum[j+1] = freqShape[i]*sin(static_cast<double>(i)*M_PI);
  }

  wipp::wipp_fft_t *fft;
  wipp::init_fft(&fft, _impl->_order);
  wipp::ifft(spectrum,
	     _impl->_coefs.get(),
	     fft);
  wipp::delete_fft(&fft);

  _impl->_firFilter.reset(new FIRFilter(static_cast<const double*>(_impl->_coefs.get()), _impl->_order));
}


void BandPassFIRFilter::initialiseTriangularCoefs_sinc2()
{

  wipp::setZeros(_impl->_coefs.get(), _impl->_order);
  int status = wipp::fir_coefs(_lowFreq, _highFreq, _impl->_coefs.get(), _impl->_order, _impl->_winType, wipp::wippfTRIANGULAR);
  if (status)
  {
    std::string msg ="I had problems while initialised a band pass FIR filter.";
    throw(DspException(msg));
  }

  wipp::wipp_fft_t *fft;
  wipp::init_fft(&fft, _impl->_order);
  wipp::wipp_complex_t cfft[_impl->_order/2+1];
  double magn[_impl->_order/2 + 1];
  wipp::fft(_impl->_coefs.get(), reinterpret_cast<double*>(cfft), fft);
  wipp::magnitude(cfft, magn, _impl->_order/2 + 1);

}


void BandPassFIRFilter::checkParameters() const
{
  if (_lowFreq <= 0)
  {
    std::ostringstream oss;
    oss << "The BandPassFilter does not admint a negative lower cut-off frequency: " << _lowFreq;
    throw(DspException(oss.str()));
  }
  if (_highFreq > 0.5)
  {
    std::ostringstream oss;
    oss << "The BandPassFilter does not admint a high cut-off frequency above 0.5: " << _highFreq;
    throw(DspException(oss.str()));
  }

  if(_lowFreq >= _highFreq)
  {
    std::ostringstream msg;
    msg << "For a band pass filter parameters must strictly accomplish: lowFreq < highFreq. ";
    msg << "(" << _lowFreq << " < " << _highFreq << ")";
    throw(DspException(msg.str()));
  }
}


void BandPassFIRFilter::filterBuffer(const double *inbuffer, double *outbuffer, int length)
{
  filterBufferCore(inbuffer, outbuffer, length);
}

void BandPassFIRFilter::filterBuffer(const float *inbuffer, float *outbuffer, int length)
{
  filterBufferCore(inbuffer, outbuffer, length);
}

void BandPassFIRFilter::filterBuffer(const int16_t *inbuffer, int16_t *outbuffer, int length)
{
  filterBufferCore(inbuffer, outbuffer, length);
}

void BandPassFIRFilter::filterBuffer(const int32_t *inbuffer, int32_t *outbuffer, int length)
{
  filterBufferCore(inbuffer, outbuffer, length);
}

void BandPassFIRFilter::filterBuffer(const uint16_t *inbuffer, uint16_t *outbuffer, int length)
{
  filterBufferCore(inbuffer, outbuffer, length);
}

template <class T> void BandPassFIRFilter::filterBufferCore(const T *inbuffer, T *outbuffer, int length)
{
  _impl->_firFilter->filterBuffer(inbuffer, outbuffer, length);
}

void BandPassFIRFilter::getCoeficients(double *coefs, int length) const
{
  if (length > _impl->_order)
    length = _impl->_order;
  for (int i=0; i<length; ++i)
    coefs[i] = _impl->_coefs[i];
}

}

