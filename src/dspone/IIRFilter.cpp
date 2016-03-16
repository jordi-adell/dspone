/*
* IIRFilter.cpp
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
#include <dspone/IIRFilter.h>
#include <dspone/DspException.h>
#include <dspone/dsplogger.h>
#include <wipp/wipputils.h>

namespace dsp {


IIRFilter::IIRFilter(const double *coefs, int length) :
  _order(0),
  _iir_filter(NULL)
{
  int bufferSize;
  _order = length/2 - 1;
  if (length%2 != 0)
    ERROR_STREAM("length should be an even number");
  else
    wipp::init_iir(_iir_filter, coefs, length/2, &coefs[length/2], length/2);
}

IIRFilter::~IIRFilter()
{
  wipp::delete_iir(_iir_filter);
}

void IIRFilter::filter(int16_t *signal, int length)
{
  double hires[length];
  filter(hires, length);
}

void IIRFilter::filter(float *signal, int length)
{
  double hires[length];
  filter(hires, length);
}

void IIRFilter::filter(double *signal, int length)
{
  if (_iir_filter != NULL)
    wipp::iir_filter(_iir_filter, signal, length);
}


void IIRFilter::filterBuffer(const int16_t *insignal, int16_t *outsignal, int length)
{
  double hiresin[length];
  double hiresout[length];
  wipp::copyBuffer(insignal, hiresin, length);
  filterBuffer(hiresin, hiresout, length);
  wipp::copyBuffer(hiresout, outsignal, length);
}

void IIRFilter::filterBuffer(const double *insignal, double *outsignal, int length)
{
  if (_iir_filter == NULL)
    wipp::copyBuffer(insignal, outsignal, length);
  else
    wipp::iir_filter(_iir_filter, insignal, outsignal, length);
}


void IIRFilter::filterBuffer(const float *insignal, float *outsignal, int length)
{
  double hiresin[length];
  double hiresout[length];
  wipp::copyBuffer(insignal, hiresin, length);
  filterBuffer(hiresin, hiresout, length);
  wipp::copyBuffer(hiresout, outsignal, length);
}


//      void IIRFilter::drawFilter()
//      {
//        int order=10;
//        FFT fft(order);

//        Ipp16s delta[1 << order];
//        Ipp16s impulseResponse[1 << order];
//        BaseType baseImpulseResponse[1 << order];
//        BaseType transferFunction[(1 << order) +1];
//        BaseType transferFunctionMod[1 << (order - 1)];
//        ippsZero_16s(delta, 1 << order);
//        delta[0] = 1;

//        filter(delta, impulseResponse, 1 << order);
//        util::ipp::copyBuffer(impulseResponse, baseImpulseResponse, 1 << order);
//        fft.fwdTransform(baseImpulseResponse, transferFunction, (1 << order));
//        ippsMagnitude_64fc(reinterpret_cast<BaseTypeC*>(transferFunction), transferFunctionMod, 1 << (order -1));


//        _plot->reset_all();
//        _plot->plot_x(&transferFunctionMod[0], (1 << (order -1)), "IIR Filter");


//      }

}

