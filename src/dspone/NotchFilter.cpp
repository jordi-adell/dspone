/*
* NotchFilter.cpp
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
#include <dspone/NotchFilter.h>
#include <dspone/IIRFilter.h>
#include <math.h>


namespace dsp {


NotchFilter::NotchFilter(double freq, double quality) : _order (4)
{

  int length = 2*(_order+1);
  _coefs.reset(new double[length]);
  //@bug review
  double mod = std::min(quality, 1.0);
  mod = std::max(mod, 0.0);

  double angle = std::min(2*M_PI*freq,  2*M_PI);
  angle = std::max(2*M_PI*angle, -2*M_PI);

  _coefs[0] = mod*mod;
  _coefs[1] = -4*mod*mod*mod*cos(angle);
  _coefs[2] = (2+4*cos(angle)*cos(angle))*mod*mod;
  _coefs[3] = -4*mod*cos(angle);
  _coefs[4] = 1;

  _coefs[5] = 1;
  _coefs[6] = -4*cos(angle);
  _coefs[7] = 2+4*cos(angle)*cos(angle);
  _coefs[8] = -4*cos(angle);
  _coefs[9] = 1;

  _filterImpl.reset(new IIRFilter(_coefs.get(), length));
}


NotchFilter::~NotchFilter()
{
  _filterImpl.reset();
}

void NotchFilter::filter(int16_t *signal, int length)
{
  _filterImpl->filter(signal, length);
}

void NotchFilter::filterBuffer(const int16_t *insignal, int16_t *outsignal, int length)
{
  _filterImpl->filterBuffer(insignal, outsignal, length);
}

void NotchFilter::filterBuffer(const double *insignal, double *outsignal, int length)
{
  _filterImpl->filterBuffer(insignal, outsignal, length);
}

void NotchFilter::filterBuffer(const float *insignal, float *outsignal, int length)
{
  _filterImpl->filterBuffer(insignal, outsignal, length);
}

}


