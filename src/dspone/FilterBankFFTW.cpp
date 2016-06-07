/*
* FilterBankFFTW.cpp
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
#include <dspone/filter/FilterBankFFTW.h>
#include <dspone/filter/BandPassFFTWFilterImpl.h>
#include <dspone/DspException.h>
#include <dspone/dspdefs.h>

#include <wipp/wipputils.h>

#include <math.h>

namespace dsp {

FilterBankFFTW::FilterBankFFTW() : _fftOrder(8){} // This initialisation is to avoid a warning.
FilterBankFFTW::FilterBankFFTW(int order, std::vector<double> centerFreqs) :
  FilterBank(((1 << (order-1))+1), centerFreqs), // For FFT of real signals is simetric ans has N/2+1 coeficients.
  _fftOrder(order)
{
  initialiseFilters();
}

FilterBankFFTW::FilterBankFFTW(const FilterBankFFTW &fbank) : FilterBank(fbank)
{

}


void FilterBankFFTW::initialiseFilters()
{
  if (_centerFreqs.empty())
    return;

  std::vector<double>::iterator it = _centerFreqs.begin();
  double initialFreq = 0;
  double centerFreq = *it;
  double endFreq = centerFreq;

  for (++it; it != _centerFreqs.end(); ++it)
  {
    endFreq = *it;
    if (endFreq <= centerFreq)
    {
      std::string msg("Center frequencies need to be monotonically ascending: ");
      msg +=  std::to_string(centerFreq) + " --> " + std::to_string(endFreq);
      throw(DspException(msg));
    }
    _filterBank.push_back(PtrFilter());
    _filterBank.back().reset(new BandPassFFTWFilterImpl(_fftOrder, initialFreq, endFreq, centerFreq));
    initialFreq = centerFreq;
    centerFreq = endFreq;
  }

  endFreq = 2*centerFreq - initialFreq;
  if (endFreq > 0.5)
    endFreq = 0.5;
  _filterBank.push_back(PtrFilter());
  _filterBank.back().reset(new BandPassFFTWFilterImpl(_fftOrder, initialFreq, endFreq, centerFreq));

}

void FilterBankFFTW::filterBuffer(double *inbuffer, double *residual, double *outbuffer, int inlength, int outlength)
{
  filterBufferCoreEfficient(inbuffer, residual, outbuffer, inlength, outlength);
}

void FilterBankFFTW::filterBuffer(float *inbuffer, float *residual, float *outbuffer, int inlength, int outlength)
{
  double bufferHR[inlength];
  double outbufferHR[outlength];
  double residualHR[inlength];
  wipp::copyBuffer(inbuffer, bufferHR, inlength);
  filterBuffer(bufferHR, residualHR, outbufferHR, inlength, outlength);
  wipp::copyBuffer(outbufferHR, outbuffer, outlength);
  wipp::copyBuffer(residualHR, residual, inlength);
}

void FilterBankFFTW::filterBuffer(int16_t *inbuffer, int16_t *residual, int16_t *outbuffer, int inlength, int outlength)
{
  double bufferHR[inlength];
  double outbufferHR[outlength];
  double residualHR[inlength];
  wipp::copyBuffer(inbuffer, bufferHR, inlength);
  filterBuffer(bufferHR, residualHR, outbufferHR, inlength, outlength);
  wipp::copyBuffer(outbufferHR, outbuffer, outlength);
  wipp::copyBuffer(residualHR, residual, inlength);
}


template <class T> void FilterBankFFTW::filterBufferCoreEfficient(T *inbuffer, T *residual, T *outbuffer, int inlength, int outlength)
{
  if (inlength != pow(2,_fftOrder))
    throw(DspException("Input buffer must be 2^order samples long (a frame)"));
  if (outlength < inlength*getNBins())
    throw(DspException("output buffer's length needs to be equal or larger thean (inlength x NBins)"));

  int outOffset = 0;
  FilterVector::iterator it;

  int speclength = inlength + 2;
  BaseType64 spectrum[speclength];
  BaseType64 weightedSpectrum[speclength];

  BandPassFFTWFilterImpl *filter = dynamic_cast<BandPassFFTWFilterImpl*>(_filterBank.begin()->get());

  filter->fft(inbuffer, inlength, spectrum, speclength);
  memcpy(weightedSpectrum, spectrum, speclength*sizeof(BaseType64));


  for (it = _filterBank.begin(); it != _filterBank.end(); ++it, outOffset += inlength)
  {
    filter = dynamic_cast<BandPassFFTWFilterImpl*>(it->get());
    filter->spectralWeighting(weightedSpectrum, speclength);
    filter->ifft(weightedSpectrum, speclength, &outbuffer[outOffset], inlength);
    memcpy(weightedSpectrum, spectrum, speclength*sizeof(BaseType64));

  }
  filter->ifft(weightedSpectrum, speclength, residual, inlength);
}


}

