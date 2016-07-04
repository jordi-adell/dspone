/*
* dsp.h
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
#ifndef __DSP_H_
#define __DSP_H_

#include <dspone/rt/SignalProcessor.h>
#include <dspone/rt/SignalAnalyser.h>
#include <dspone/rt/Filters.h>

#define _DSP_DECL  dsp

typedef _DSP_DECL::SignalProcessor ProcessorModule;
typedef _DSP_DECL::SignalAnalyser AnalyserModule;
typedef _DSP_DECL::FilterProcess FilterModule;

typedef std::shared_ptr<ProcessorModule> ProcessorModulePtr;
typedef std::shared_ptr<AnalyserModule> AnalyserModulePtr;
typedef std::shared_ptr<FilterModule> FilterModulePtr;

#endif // __DSP_H_
