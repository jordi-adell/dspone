/*
* ShortTimeProcess.cpp
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
#include <dspone/rt/ShortTimeProcess.h>
#include <dspone/rt/ShortTimeProcessImpl.h>
#include <dspone/rt/ShortTimeAnalysisImpl.hpp>
#include <dspone/DspException.h>

//#include <wipp/wipputils.h>
//#include <wipp/wippfilter.h>
#include <fstream>

namespace dsp {

ShortTimeProcess::ShortTimeProcess(int windowSize, int analysisLength, int nchannels, Mode mode)
{
  switch(mode)
  {
    case ANALYSIS:
      _impl.reset(new ShortTimeAnalysisImpl(this, windowSize, analysisLength, nchannels));
    break;
    case ANALYSIS_SYNTHESIS:
    default:
      _impl.reset(new ShortTimeProcessImpl(this, windowSize, analysisLength, nchannels, mode));
    break;
  }
}

double ShortTimeProcess::getRate() const
{
  _impl->getRate();
}

int ShortTimeProcess::getNumberOfChannels() const
{
  return _impl->getNumberOfChannels();;
}

ShortTimeProcess::~ShortTimeProcess()
{
  _impl.reset();
}




int ShortTimeProcess::getAmountOfRemainingSamples()
{
  return _impl->getAmountOfRemainingSamples();
}





int ShortTimeProcess::calculateOrderFromSampleRate(int sampleRate, double frameRate)
{
  int order = static_cast<int>(log(2*frameRate*sampleRate)/log(2.0F) + 0.5);
  if (0 < order && order < 4)
  {
    ERROR_STREAM("Too low order for FFT: " << order << " this might cose undefined errors.");
  }
  else if(order <= 0)
  {
    std::stringstream oss;
    oss << "Negative or zero FFT order value: " << order << ". "
	<< "Order is optained from frame rate: " << frameRate
	<< " and sample rate: " << sampleRate;
    throw(DspException(oss.str()));
  }
  return order;
}

int ShortTimeProcess::getLatency() const
{
  return _impl->getLatency();
}

int ShortTimeProcess::getMaxLatency() const
{
  return _impl->getMaxLatency();
}

int ShortTimeProcess::getMinLatency() const
{
  return _impl->getMinLatency();
}

int ShortTimeProcess::getBufferSize() const
{
  _impl->getFrameSize();
}

int ShortTimeProcess::getAnalysisLength() const
{
  return _impl->getAnalysisLength();
}

int ShortTimeProcess::getWindowSize() const
{
  return _impl->getWindowSize();
}

int ShortTimeProcess::getFrameSize() const
{
  return _impl->getFrameSize();
}

int ShortTimeProcess::getFrameRate() const
{
  return getFrameRate();
}


int ShortTimeProcess::process(const std::vector<double *> &signal, unsigned int inbuffersize,
			      const std::vector<double *> &output, unsigned int outbuffersize)
{
  return _impl->process(signal, inbuffersize, output, outbuffersize);
}

int ShortTimeProcess::process(const std::vector<float *> &signal, unsigned int inbuffersize,
			      const std::vector<float *> &output, unsigned int outbuffersize)
{
  return _impl->process(signal, inbuffersize, output, outbuffersize);
}

int ShortTimeProcess::process(const std::vector<int16_t *> &signal, unsigned int inbuffersize,
			      const std::vector<int16_t *> &output, unsigned int outbuffersize)
{
  return _impl->process(signal, inbuffersize, output, outbuffersize);
}

int ShortTimeProcess::process(const std::vector<std::shared_ptr<double> > &signal, unsigned int inbuffersize,
			      const std::vector<std::shared_ptr<double> > &output, unsigned int outbuffersize)
{
  return _impl->process(signal, inbuffersize, output, outbuffersize);
}

int ShortTimeProcess::process(const std::vector<std::shared_ptr<float> > &signal, unsigned int inbuffersize,
			      const std::vector<std::shared_ptr<float> > &output, unsigned int outbuffersize)
{
  return _impl->process(signal, inbuffersize, output, outbuffersize);
}

int ShortTimeProcess::process(const std::vector<std::shared_ptr<int16_t> > &signal, unsigned int inbuffersize,
			      const std::vector<std::shared_ptr<int16_t> > &output, unsigned int outbuffersize)
{
  return _impl->process(signal, inbuffersize, output, outbuffersize);
}

}



