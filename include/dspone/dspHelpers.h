/*
* dspHelpers.h
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
#ifndef __DSPHELPERS_H_
#define __DSPHELPERS_H_

#include <dspone/dspdefs.h>

namespace dsp {

BaseType calculateLinearPowerFFT(const SignalVector &analysisFrames, int analysisLength, unsigned int nchannels);
BaseType calculateLogPowerFFT(const SignalVector &analysisFrames, int analysisLength, unsigned int nchannels);
BaseType calculateLinearPowerTemporal(const SignalVector &analysisFrames, int analysisLength, unsigned int nchannels);
BaseType calculateLinearPowerTemporal(const std::vector<BaseType*> &analysisFrames, int analysisLength, unsigned int nchannels);
BaseType calculateLogPowerTemporal(const SignalVector &analysisFrames, int analysisLength, unsigned int nchannels);

BaseType calculateLinearPowerTemporal(const BaseType* signal, int length);
BaseType calculateLinearPowerTemporal(const BaseType16s *signal, int length);
BaseType calculateLinearPowerTemporal(const SignalPtr &signal, int length);
BaseType calculateLogPowerTemporal(const BaseType *signal, int length);
BaseType calculateLogPowerTemporal(const BaseType16s *signal, int length);
BaseType calculateLogPowerTemporal(const SignalPtr &signal, int length);
BaseType calculateLinearPowerFFT(const SignalPtr &signal, int length);
BaseType calculateLogPowerFFT(const SignalPtr &signal, int length);

}



#endif // __DSPHELPERS_H_
