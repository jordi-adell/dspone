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
#include <dspone/rt/ShortTimeProcessImpl.h>
#include <dspone/rt/ShortTimeProcess.h>


namespace dsp {

ShortTimeProcessImpl::ShortTimeProcessImpl(ShortTimeProcess *frameProcessor,
					   int windowSize,
					   int analysisLength,
					   int nchannels,
					   Mode mode) :
  _frameProcessor(frameProcessor),
  _windowSize (windowSize),
  _windowShift (_windowSize/2),
  _halfWindowSize (_windowSize/2),
  _nchannels(nchannels),
  _nDataChannels(10),
  _analysisLength((analysisLength == 0) ? _windowSize + 2 : analysisLength),
  _window(new BaseType[_windowSize+1]), // Needs to be of odd length in order to preserve the original energy.
  _iwindow(new BaseType[_windowSize]),
  _mode(mode),
  _doSynthesis(_mode == ShortTimeProcess::ANALYSIS_SYNTHESIS)
{
  initVariableMembers();
  initWindowBuffers();
  initBuffers();
}



double ShortTimeProcessImpl::getRate() const
{
  return _windowShift;
}

int ShortTimeProcessImpl::getNumberOfChannels() const
{
  return _nchannels;
}

int ShortTimeProcessImpl::getNumberOfDataChannels() const
{
  return _nDataChannels;
}

ShortTimeProcessImpl::~ShortTimeProcessImpl()
{
}


void ShortTimeProcessImpl::initVariableMembers()
{
  _firstCall = true;
}

void ShortTimeProcessImpl::initWindowBuffers()
{

  // Create auxiliar buffers
  _frame.reset(new BaseType[_windowSize]);

  // Setting window vector
  wipp::set(1.0,_window.get(),_windowSize+1);
  wipp::window(_window.get(), _windowSize+1, wipp::wippHANN);
  wipp::sqrt(&_window[1],_windowSize-1);
  _window[0]=0;

  // Setting inverse window vector
  BaseType ones[_windowSize];
  wipp::set(1.0, ones, _windowSize);
  wipp::copyBuffer(_window.get(),_iwindow.get(),_windowSize);
  wipp::div(&_window[1], ones, &_iwindow[1], _windowSize-1);
}

void ShortTimeProcessImpl::initBuffers()
{
  wipp::setZeros(_frame.get(),_windowSize);

  for (unsigned int i = 0; i<_nchannels; ++i)
  {
    _latencyBufferProcessed.push_back(SignalPtr(new BaseType[_windowSize]));
    wipp::wipp_circular_buffer_t *cb;
    wipp::init_cirular_buffer(&cb, _maximumLatencyBufferSize, _frame.get(), _windowSize);
    _latencyBufferSignal.push_back(cb);
    wipp::setZeros(_latencyBufferProcessed.back().get(), _windowSize);

    _analysisFramesPtr.push_back(new BaseType[_analysisLength]);
    _analysisFrames.push_back(SignalPtr(_analysisFramesPtr.back()));
    wipp::setZeros(_analysisFrames.back().get(), _analysisLength);
  }
  allocateNDataChannels(_nDataChannels);
}

void ShortTimeProcessImpl::allocateNDataChannels(int nDataChannels)
{
  for (unsigned int i = _dataFrames.size(); i < nDataChannels; ++i)
  {
    _dataFramesPtr.push_back(new BaseType[_windowSize]);
    _dataFrames.push_back(SignalPtr(_dataFramesPtr.back()));
    wipp::setZeros(_dataFrames.back().get(), _windowSize);
  }

  for (unsigned int i = _latencyBufferSignal.size(); i < nDataChannels + _nchannels; ++i)
  {
    BaseType zeros[_windowSize];
    wipp::setZeros(zeros, _windowSize);
    //    _latencyBufferSignal.push_back(container::CircularBuffer<BaseType, _maximumLatencyBufferSize>());
    _latencyBufferProcessed.push_back(SignalPtr(new BaseType[_windowSize]));
    wipp::setZeros(_latencyBufferProcessed.back().get(), _windowSize);
    wipp::wipp_circular_buffer_t *cb;
    wipp::init_cirular_buffer(&cb, _maximumLatencyBufferSize, zeros, _windowSize);
    _latencyBufferSignal.push_back(cb);
  }
}


int ShortTimeProcessImpl::getAmountOfRemainingSamples()
{
  if (_firstCall)
    return 0;
  else
    return _windowShift;
}



void ShortTimeProcessImpl::clear()
{
  _latencyBufferSignal.clear();
  _analysisFrames.clear();
  _analysisFramesPtr.clear();
  for (size_t i = 0; i < _latencyBufferSignal.size(); ++i)
  {
    wipp::wipp_circular_buffer_t *f = _latencyBufferSignal.at(i);
    wipp::delete_circular_buffer(&f);
  }
  _dataFrames.clear();
  _dataFramesPtr.clear();
  _latencyBufferProcessed.clear();

  initBuffers();
}

void ShortTimeProcessImpl::ShortTimeProcessing(const SignalVector &signal, const SignalVector &output, int length)
{
  std::string msg = "ShortTimeProcess process frame of ";
  msg +=  std::to_string(_windowSize) + " samples";
  int lastFrame = length - _windowSize;
  int sample = 0;

  SignalVector::const_iterator it;

  for (it = output.begin(); it != output.end(); ++it)
    wipp::setZeros(it->get(), length);



  for (sample=0; sample <= lastFrame; sample = sample + _windowShift)
  {
    unsigned int channel;
    for (channel = 0, it = signal.begin(); // Foreach signal channel
	 it != signal.end() && channel < _nchannels;
	 ++it, ++channel)
    {
      BaseType *ptr = it->get();
      wipp::copyBuffer(&ptr[sample],_frame.get(),_windowSize);
      wipp::mult(_window.get(),_frame.get(),_windowSize);
      frameAnalysis(_frame.get(), _analysisFrames[channel].get(), _windowSize, _analysisLength, channel);
    }

    for(channel = 0;  // Foreach data channel
	it != signal.end() && channel < _nDataChannels;
	++it, ++channel)
    {
      BaseType *ptr = it->get();
      wipp::copyBuffer(&ptr[sample], _dataFrames[channel].get(), _windowSize);
    }


    /// Set data channels point to the actual signal channels.

    processParametrisation();

    if (!_doSynthesis)
      continue;

    for (channel = 0, it = output.begin(); // Foreach signal channel
	 it != output.end() && channel < _nchannels;
	 ++it, ++channel)
    {
      BaseType *ptr =it->get();
      frameSynthesis(_frame.get(), _analysisFrames[channel].get(), _windowSize, _analysisLength, channel);
      wipp::mult(_window.get(),_frame.get(),_windowSize);
      wipp::add(_frame.get(),&ptr[sample],_windowSize);
    }

    for(channel = 0; // Foreach data channel
	it != output.end() && channel < _nDataChannels;
	++it, ++channel)
    {
      BaseType *ptr = it->get();
      wipp::copyBuffer(_dataFrames[channel].get(), &ptr[sample], _windowSize);
    }
  }
}



int ShortTimeProcessImpl::calculateOrderFromSampleRate(int sampleRate, double frameRate)
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


void ShortTimeProcessImpl::unwindowFrame(double *frame, size_t length) const
{
  wipp::mult(_iwindow.get(), frame, ((length < _windowSize) ? length : _windowSize));
}

void ShortTimeProcessImpl::unwindowFrame(double *frame, double *unwindowed, size_t length) const
{
  wipp::setZeros(unwindowed, length);
  wipp::copyBuffer(frame, unwindowed, length);
  wipp::mult(_iwindow.get(), frame, ((length < _windowSize) ? length : _windowSize));
}



int ShortTimeProcessImpl::getLatency() const
{
  size_t occupancy;
  //  return _latencyBufferSignal[0].getCountUsed();
  wipp::cf_occupancy(_latencyBufferSignal[0], &occupancy);
  return occupancy;
}

int ShortTimeProcessImpl::getMaxLatency() const
{
  return getWindowSize() + _windowShift;
}

int ShortTimeProcessImpl::getMinLatency() const
{
  return _windowShift;
}

int ShortTimeProcessImpl::getAnalysisLength() const
{
  return _analysisLength;
}

int ShortTimeProcessImpl::getWindowSize() const
{
  return _windowSize;
}

int ShortTimeProcessImpl::getFrameSize() const
{
  return getWindowSize();
}

int ShortTimeProcessImpl::getFrameRate() const
{
  return _windowShift;
}


void ShortTimeProcessImpl::frameAnalysis(BaseType *inFrame,
					 BaseType *analysis,
					 int frameLength,
					 int analysisLength,
					 int channel)
{
  _qtdebug.plot(inFrame, frameLength, QtDebug::IN_FRAME, channel);
  _frameProcessor->frameAnalysis(inFrame, analysis, frameLength, analysisLength, channel);
}

void ShortTimeProcessImpl::processParametrisation()
{

  _qtdebug.plot(_analysisFrames, _analysisLength, QtDebug::IN_ANALYSIS);

  _frameProcessor->processParametrisation(_analysisFramesPtr, _analysisLength, _dataFramesPtr, _windowSize);

  _qtdebug.plot(_analysisFrames, _analysisLength, QtDebug::OUT_ANALYSIS);
}


void ShortTimeProcessImpl::frameSynthesis(BaseType *outFrame,
					  BaseType *analysis,
					  int frameLength,
					  int analysisLength,
					  int channel)
{
  _frameProcessor->frameSynthesis(outFrame, analysis, frameLength, analysisLength, channel);
  _qtdebug.plot(outFrame, frameLength, QtDebug::OUT_FRAME, channel);
}




}



