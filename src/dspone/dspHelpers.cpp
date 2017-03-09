/*
* dspHelpers.cpp
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

#include <dspone/dspHelpers.h>
#include <wipp/wipputils.h>
#include <wipp/wippstats.h>
#include <wipp/wippsignal.h>
#include <wipp/wippdefs.h>

#include <boost/shared_array.hpp>

namespace dsp {

template <typename U, typename T>
U calculateLinearPowerTemporal_core(const T *signal, int length)
{
  BaseType64 aux[length];
  wipp::copyBuffer(signal, aux, length);
  BaseType64 power;
  wipp::sqr(aux, length);
  wipp::mean(aux, length, &power);
  return power;
}

template <typename T, typename U>
U calculateLinearPowerTemporal_core(const std::vector<const T*> &analysisFrames, int analysisLength, unsigned int nchannels)
{
  double meanPower = 0;

  boost::shared_array<double> signalPower;
  boost::shared_array<double> channelPower;

  signalPower.reset(new BaseType[analysisLength]);
  channelPower.reset(new BaseType[nchannels]);

  wipp::setZeros(signalPower.get(), analysisLength);
  wipp::setZeros(channelPower.get(), nchannels);

  for (unsigned int c = 0; c < nchannels; ++c)
  {
    wipp::sqr(analysisFrames[c], signalPower.get(), analysisLength);
    wipp::mean(signalPower.get(), analysisLength, &channelPower[c]);
  }

  wipp::mean(channelPower.get(), nchannels, &meanPower);
  return meanPower;
}

template <typename T, typename U>
U calculateLinearPowerFFT_core(const std::vector<const T*> &analysisFrames, int analysisLength, unsigned int nchannels)
{
  boost::shared_array<double> signalPower;
  boost::shared_array<double> channelPower;
  double meanPower;
  int complexFFTCCSLength = analysisLength/2;

  signalPower.reset(new BaseType[complexFFTCCSLength]);
  channelPower.reset(new BaseType[nchannels]);

  // Calculate power for all channels.
  for (unsigned int c = 0; c < nchannels; ++c)
  {
    // Since we are using the normalization factor 1/N in the forward FFT,
    // the sum of squared samples includes the factor 1/(N^2).
    // This implies that the sum is directly the mean power (see Parseval's theorem).
    // We multiply x2 the result because we are working with one-sided FFT.

    wipp::power(reinterpret_cast<const wipp::wipp_complex_t*>(analysisFrames[c]), signalPower.get(), complexFFTCCSLength);
    wipp::sum(signalPower.get(), complexFFTCCSLength, &(channelPower[c]));
    channelPower.get()[c] = 2*channelPower[c] - signalPower[0] - signalPower[complexFFTCCSLength-1];
  }

  wipp::mean(channelPower.get(), nchannels, &meanPower);
  return meanPower;
}

BaseType calculateLinearPowerTemporal(const std::vector<BaseType *> signal, int length)
{
  std::vector<const BaseType*> vsignal;
  for (auto it = signal.begin(); it != signal.end(); ++it)
    vsignal.push_back(*it);
  return calculateLinearPowerTemporal_core<BaseType, BaseType>(vsignal, length, signal.size());
}

BaseType calculateLinearPowerTemporal(const SignalVector &analysisFrames, int analysisLength, unsigned int nchannels)
{
  std::vector<const BaseType*> vsignal;
  for (auto it = analysisFrames.begin(); it != analysisFrames.end(); ++it)
    vsignal.push_back(it->get());

  if (vsignal.size()  < nchannels)
    nchannels = vsignal.size();
  return calculateLinearPowerTemporal_core<BaseType, BaseType>(vsignal, analysisLength, nchannels);
}

BaseType calculateLinearPowerTemporal(const std::vector<BaseType*> &analysisFrames, int analysisLength, unsigned int nchannels)
{
  std::vector<const BaseType*> vsignal;
  for (auto it = analysisFrames.begin(); it != analysisFrames.end(); ++it)
    vsignal.push_back(*it);

  return calculateLinearPowerTemporal_core<BaseType, BaseType>(vsignal, analysisLength, nchannels);
}

BaseType calculateLogPowerTemporal(const SignalVector &analysisFrames, int analysisLength, unsigned int nchannels)
{
  return 10*log10(calculateLinearPowerTemporal(analysisFrames, analysisLength, nchannels)+1e-10);
}

BaseType calculateLogPowerTemporal(const std::vector<BaseType*> signal, int length)
{
  return 10*log10(calculateLinearPowerTemporal(signal, length));
}

BaseType calculateLogPowerFFT(const SignalVector &analysisFrames, int analysisLength, unsigned int nchannels)
{
  return 10*log10(calculateLinearPowerFFT(analysisFrames, analysisLength, nchannels)+1e-10);
}


BaseType calculateLinearPowerTemporal(const SignalPtr &signal, int length)
{
  return calculateLinearPowerTemporal(signal.get(), length);
}

BaseType calculateLinearPowerTemporal(const BaseType* signal, int length)
{
  BaseType power;
  BaseType signalSqr[length];
  wipp::sqr(signal, signalSqr, length);
  wipp::mean(signalSqr, length, &power);
  return power;
}

BaseType calculateLinearPowerTemporal(const BaseType32 *signal, int length)
{
  return calculateLinearPowerTemporal_core<BaseType>(signal, length);
}

BaseType calculateLinearPowerTemporal(const BaseType16s *signal, int length)
{
  return calculateLinearPowerTemporal_core<BaseType>(signal, length);
}


BaseType calculateLogPowerTemporal(const BaseType32 *signal, int length)
{
  return 10*log10(calculateLinearPowerTemporal(signal, length)+1e-100);
}

BaseType calculateLogPowerTemporal(const BaseType64 *signal, int length)
{
  return 10*log10(calculateLinearPowerTemporal(signal, length)+1e-100);
}

BaseType calculateLogPowerTemporal(const BaseType16s *signal, int length)
{
  return 10*log10(calculateLinearPowerTemporal(signal, length)+1e-100);
}

BaseType calculateLogPowerTemporal(const SignalPtr &signal, int length)
{
  return 10*log10(calculateLinearPowerTemporal(signal, length)+1e-100);
}

BaseType calculateLinearPowerFFT(const SignalPtr &signal, int length)
{
  BaseType power;
  int complexFFTCCSLength = length/2;
  BaseType signalPower[complexFFTCCSLength];
  // Since we are using the normalization factor 1/N in the forward FFT,
  // the sum of squared samples includes the factor 1/(N^2).
  // This implies that the sum is directly the mean power (see Parseval's theorem).
  // We multiply x2 the result because we are working with one-sided FFT.

  wipp::power(reinterpret_cast<wipp::wipp_complex_t*>(signal.get()), signalPower, complexFFTCCSLength);
  wipp::sum(signalPower, complexFFTCCSLength, &power);
  power = 2*power - signalPower[0] - signalPower[complexFFTCCSLength-1];

  return power;
}


BaseType calculateLinearPowerFFT(const std::vector<BaseType*> &signal, int length)
{
  std::vector<const BaseType*> vsignal;
  for (auto it = signal.begin(); it != signal.end(); ++it)
    vsignal.push_back(*it);
  return calculateLinearPowerFFT_core<BaseType, const BaseType>(vsignal, length, signal.size());
}

BaseType calculateLinearPowerFFT(const SignalVector &analysisFrames, int analysisLength, unsigned int nchannels)
{
  std::vector<const BaseType*> vsignal;
  for (auto it = analysisFrames.begin(); it != analysisFrames.end(); ++it)
    vsignal.push_back(it->get());

  if (vsignal.size()  < nchannels)
    nchannels = vsignal.size();

  return calculateLinearPowerFFT_core<BaseType, BaseType>(vsignal, analysisLength, vsignal.size());
}

BaseType calculateLinearPowerFFT(const BaseType *signal, int length)
{
  std::vector<const BaseType*> vsignal;
  vsignal.push_back(signal);
  return calculateLinearPowerFFT_core<BaseType, BaseType>(vsignal, length, 1);
}

BaseType calculateLogPowerFFT(const SignalPtr &signal, int length)
{
  return 10*log10((signal, length));
}

BaseType calculateLogPowerFFT(const BaseType* signal, int length)
{
  return 10*log10(calculateLinearPowerFFT(signal, length) + 1e-100);
}

BaseType calculateLogPowerFFT(const std::vector<BaseType*> &signal, int length)
{
  return 10*log10(calculateLinearPowerFFT(signal, length) + 1e-100);
}


}
