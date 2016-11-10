/*
* dspHelpers.cpp
* Copyright 2016 (c) Jordi Adell
* Created on: 2016
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

#include <dspone/algorithm/signalPower.h>
#include <dspone/dspHelpers.h>

namespace dsp
{

SignalPower::SignalPower()
{

}


SignalPower::~SignalPower()
{

}


double SignalPower::power(const double * signal, int length)
{
    return calculateLinearPowerTemporal(signal, length);
}

double SignalPower::logPower(const double *signal, int length)
{
    return calculateLogPowerTemporal(signal, length);
}

double SignalPower::power(const std::vector<boost::shared_array<double> > &signal, int length)
{
    return calculateLinearPowerTemporal(signal, length, signal.size());
}


double SignalPower::logPower(const std::vector<boost::shared_array<double> > &signal, int length)
{
    return calculateLogPowerTemporal(signal, length, signal.size());
}


double SignalPower::power(const boost::shared_array<double> &signal, int length)
{
    return calculateLinearPowerTemporal(signal, length);
}

double SignalPower::logPower(const boost::shared_array<double> &signal, int length)
{
    return calculateLogPowerTemporal(signal, length);
}

double SignalPower::power(const int16_t * signal, int length)
{
    return calculateLinearPowerTemporal(signal, length);
}

double SignalPower::logPower(const int16_t *signal, int length)
{
    return calculateLogPowerTemporal(signal, length);
}

double SignalPower::logPower(const std::vector<double *> &signal, int length)
{
  return calculateLogPowerTemporal(signal, length);
}

double SignalPower::power(const std::vector<double *> &signal, int length)
{
  return calculateLinearPowerTemporal(signal, length);
}

double SignalPower::FFTPower(const double *fft, int fft_length)
{
  return calculateLinearPowerFFT(fft, fft_length);
}

double SignalPower::FFTPower(const boost::shared_array<double> &fft, int fft_length)
{
  return calculateLinearPowerFFT(fft, fft_length);
}

double SignalPower::FFTPower(const std::vector<double*> &fft, int fft_length)
{
  return calculateLinearPowerFFT(fft, fft_length);
}

double SignalPower::FFTLogPower(const std::vector<double*> &fft, int fft_length)
{
  return calculateLogPowerFFT(fft, fft_length);
}

double SignalPower::FFTPower(const std::vector<boost::shared_array<double> > &fft, int fft_length)
{
  return calculateLinearPowerFFT(fft, fft_length, fft.size());
}

double SignalPower::FFTLogPower(const double *fft, int fft_length)
{
  return calculateLogPowerFFT(fft, fft_length);
}

double SignalPower::FFTLogPower(const boost::shared_array<double>  &fft, int fft_length)
{
  return calculateLogPowerFFT(fft, fft_length);
}

double SignalPower::FFTLogPower(const std::vector<boost::shared_array<double> > &fft, int fft_length)
{
  return calculateLogPowerFFT(fft, fft_length, fft.size());
}




}
