/*
* SignalProcessor.cpp
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
#include <dspone/rt/SignalProcessor.h>
#include <dspone/rt/ShortTimeProcess.h>
#include <dspone/rt/ShortTimeAnalysis.h>

#include <dspone/rt/DummyShortTimeProcess.h>

namespace dsp {


SignalProcessor::SignalProcessor()
{
  _impl.reset(new DummyShortTimeProcess(512));
}

SignalProcessor::~SignalProcessor()
{
  _impl.reset();
}

int SignalProcessor::process(const std::vector<int16_t *> &signal,
			     unsigned int inbuffersize,
			     const std::vector<int16_t *> &output,
			     unsigned int outbuffersize)
{
  return _impl->process(signal, inbuffersize, output, outbuffersize);
}

int SignalProcessor::process(const std::vector<float *> &signal,
			     unsigned int inbuffersize,
			     const std::vector<float *> &output,
			     unsigned int outbuffersize)
{
  return  _impl->process(signal, inbuffersize, output, outbuffersize);
}


int SignalProcessor::process(const std::vector<double *> &signal,
			     unsigned int inbuffersize,
			     const std::vector<double *> &output,
			     unsigned int outbuffersize)
{
  return  _impl->process(signal, inbuffersize, output, outbuffersize);
}



int SignalProcessor::process(const std::vector<std::shared_ptr<int16_t> > &signal,
			     unsigned int inbuffersize,
			     const std::vector<std::shared_ptr<int16_t> > &output,
			     unsigned int outbuffersize)
{
  return _impl->process(signal, inbuffersize, output, outbuffersize);
}

int SignalProcessor::process(const std::vector<std::shared_ptr<float> > &signal,
			     unsigned int inbuffersize,
			     const std::vector<std::shared_ptr<float> > &output,
			     unsigned int outbuffersize)
{
  return _impl->process(signal, inbuffersize, output, outbuffersize);
}

int SignalProcessor::process(const std::vector<std::shared_ptr<double> > &signal,
			     unsigned int inbuffersize,
			     const std::vector<std::shared_ptr<double> > &output,
			     unsigned int outbuffersize)
{
  return _impl->process(signal, inbuffersize, output, outbuffersize);
}

int SignalProcessor::getLatency() const
{
  return _impl->getLatency();
}

int SignalProcessor::getMaxLatency() const
{
  return _impl->getMaxLatency();
}

int SignalProcessor::getBufferSize() const
{
  return _impl->getFrameSize();
}

int SignalProcessor::getNumberOfChannels() const
{
  return _impl->getNumberOfChannels();
}




SignalAnalyser::SignalAnalyser()
{

}

SignalAnalyser::~SignalAnalyser()
{
  _impl.reset();
}

int SignalAnalyser::process(const std::vector<std::shared_ptr<int16_t> > &signal, unsigned int buffersize)
{
  return _impl->process(signal, buffersize);
}

int SignalAnalyser::process(const std::vector<std::shared_ptr<float> > &signal, unsigned int buffersize)
{
  return _impl->process(signal, buffersize);
}

int SignalAnalyser::process(const std::vector<std::shared_ptr<double> > &signal, unsigned int buffersize)
{
  return _impl->process(signal, buffersize);
}

int SignalAnalyser::process(const std::vector<int16_t *> &signal, unsigned int buffersize)
{
  return _impl->process(signal, buffersize);
}

int SignalAnalyser::process(const std::vector<float *> &signal, unsigned int buffersize)
{
  return _impl->process(signal, buffersize);
}

int SignalAnalyser::process(const std::vector<double *> &signal, unsigned int buffersize)
{
  return _impl->process(signal, buffersize);
}



int SignalAnalyser::getLatency() const
{
  return _impl->getLatency();
}

int SignalAnalyser::getMaxLatency() const
{
  return _impl->getMaxLatency();
}

int SignalAnalyser::getBufferSize() const
{
  return -1;
}

int SignalAnalyser::getNumberOfChannels() const
{
  return _impl->getNumberOfChannels();
}

}

