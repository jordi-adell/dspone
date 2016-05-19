/*
* PreEmphasisFilter.cpp
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
#include <dspone/PreEmphasisFilter.h>
#include <dspone/IIRFilter.h>
#include <wipp/wipputils.h>
#include <boost/scoped_array.hpp>

namespace dsp {


PreEmphasisFilter::PreEmphasisFilter(double factor)
{
  static const int order = 1;
  static const int length = 2*(order+1);
  double coefs[length];
  coefs[0] = 1;
  coefs[1] = 0;
  coefs[2] = 1;
  coefs[3] = factor;

  //                  1
  //  H(z) = ----------------------
  //            1 - factor/z

  _filterImpl.reset(new IIRFilter(coefs, length));

}

PreEmphasisFilter::~PreEmphasisFilter()
{
  _filterImpl.reset();
}

void PreEmphasisFilter::filter(int16_t *signal, int length)
{
  _filterImpl->filter(signal, length);
}

void PreEmphasisFilter::filterBuffer(const int16_t *insignal, int16_t *outsignal, int length)
{
  _filterImpl->filterBuffer(insignal, outsignal, length);
}

void PreEmphasisFilter::filterBuffer(const double *insignal, double *outsignal, int length)
{
  _filterImpl->filterBuffer(insignal, outsignal, length);
}

void PreEmphasisFilter::filterBuffer(const float *insignal, float *outsignal, int length)
{
  _filterImpl->filterBuffer(insignal, outsignal, length);
}

int PreEmphasisFilter::getFrameLength() const
{
  return _filterImpl->getFrameLength();
}

}

