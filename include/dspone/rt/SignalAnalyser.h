/*
* SignalProcessor.h
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
#ifndef __SIGNAL_ANALYSER_H_
#define __SIGNAL_ANALYSER_H_

#include <vector>
#include <stdint.h>

#include <boost/shared_array.hpp>

namespace dsp {


class SignalAnalyser
{
    public:
	SignalAnalyser() {}
	virtual ~SignalAnalyser() {}

	virtual int process(const std::vector<double*> &signal,
		    unsigned int inbuffersize) = 0;

	virtual int process(const std::vector<float*> &signal,
		    unsigned int inbuffersize) = 0;

	virtual int process(const std::vector<int16_t*> &signal,
		    unsigned int inbuffersize) = 0;

	virtual int process(const std::vector<uint16_t*> &signal,
		    unsigned int inbuffersize) = 0;

	virtual int getLatency() const = 0;
	virtual int getMaxLatency() const = 0;
	virtual int getBufferSize() const = 0;
	virtual int getNumberOfChannels() const = 0;

};


}


#endif // __ISHORTTIMEPROCESS_H_
