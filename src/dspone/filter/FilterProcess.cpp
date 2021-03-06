/*
* FilterProcess.cpp
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
#include <dspone/rt/FilterProcess.hpp>
#include <dspone/DspException.h>

namespace dsp {

FilterProcess::FilterProcess(int nchannels) :
  _nchannels(nchannels)
{

}

FilterProcess::~FilterProcess()
{

}

int FilterProcess::getLatency() const
{
  return 0;
}

int FilterProcess::getMaxLatency() const
{
  return 0;
}

int FilterProcess::getBufferSize() const
{
  return _filters.at(0)->getFrameLength();
}

int FilterProcess::getNumberOfChannels() const
{
  return _filters.size();
}

int FilterProcess::process(const std::vector<double*> &signal,
			   unsigned int inbuffersize,
			   const std::vector<double*> &output,
			   unsigned int outbuffersize)
{
    process_core(signal, inbuffersize, output, outbuffersize);
}

int FilterProcess::process(const std::vector<float*> &signal,
			   unsigned int inbuffersize,
			   const std::vector<float*> &output,
			   unsigned int outbuffersize)
{
    process_core(signal, inbuffersize, output, outbuffersize);

}

int FilterProcess::process(const std::vector<int16_t*> &signal,
			   unsigned int inbuffersize,
			   const std::vector<int16_t*> &output,
			   unsigned int outbuffersize)
{
    process_core(signal, inbuffersize, output, outbuffersize);
}

int FilterProcess::process(const std::vector<uint16_t*> &signal,
			   unsigned int inbuffersize,
			   const std::vector<uint16_t*> &output,
			   unsigned int outbuffersize)
{
    process_core(signal, inbuffersize, output, outbuffersize);
}





}
