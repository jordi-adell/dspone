/*
* BasicFilterModules.h
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
* along with DSPONE.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __BASICFILTERSMODULES_H_
#define __BASICFILTERSMODULES_H_

#include <dspone/FilterProcess.h>

class IIRFilterModule : public dsp::FilterProcess
{
public:
  IIRFilterModule(int nchannels, const double *coefs, int length);
};


class FIRFilterModule : public dsp::FilterProcess
{
public:
  FIRFilterModule(int nchannels, const double *coefs, int length);
};

class BandPassFIRFilterModule : public dsp::FilterProcess
{
public:
  BandPassFIRFilterModule(int nchannels, int order, double lowFreq, double highFreq);
};

class BandPassFFTWFilterModule : public dsp::FilterProcess
{
public:
  BandPassFFTWFilterModule(int nchannels, int order, double lowFreq, double highFreq);
};

class PreEmphasisFilterModule : public dsp::FilterProcess
{
public:
  PreEmphasisFilterModule(int nchannels, double factor);
};

#endif // __BASICFILTERSMODULES_H_
