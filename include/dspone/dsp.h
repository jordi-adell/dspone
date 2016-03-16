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

#include "SignalProcessor.h"
#include "BasicFilterModules.h"

#include <boost/shared_ptr.hpp>
#include <wipp/wipp.h>

using namespace wipp;

#define __INTERACTION_DSP_DECL  dsp

typedef __INTERACTION_DSP_DECL::SignalProcessingDispatcher<__INTERACTION_DSP_DECL::SignalProcessor> SignalProcessor_t;
typedef __INTERACTION_DSP_DECL::SignalProcessingDispatcher<__INTERACTION_DSP_DECL::FilterProcess  > FilterProcessor_t;
typedef __INTERACTION_DSP_DECL::SignalAnalysisDispatcher<__INTERACTION_DSP_DECL::SignalAnalyser> SignalAnalyser_t;


typedef SignalProcessor_t ProcessorModule;
typedef SignalAnalyser_t AnalyserModule;
typedef FilterProcessor_t FilterModule;

typedef boost::shared_ptr<ProcessorModule> SignalProcessorModulePtr;
typedef boost::shared_ptr<AnalyserModule> SignalAnalyserModulePtr;
typedef boost::shared_ptr<FilterModule> FilterModulePtr;



#endif // __DSP_H_
