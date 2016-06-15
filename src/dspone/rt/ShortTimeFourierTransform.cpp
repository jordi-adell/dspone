/*
* ShortTimeFourierTransform.cpp
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

#include <dspone/rt/ShortTimeFourierTransform.h>
#include <dspone/rt/STFTImpl.h>

namespace dsp {

// ------------ STFT -------------------------------------------------
// (+2) This is to allocate enough memory for CCs IPP compact complex data
STFT::STFT(int order) :
  ShortTimeProcess(STFTImpl::getAnalysisWindowLength(order), STFTImpl::getFFTLength(order), 1)
{
  _impl.reset(new STFTImpl(order));
}

STFT::STFT(int nchannels, int order) :
  ShortTimeProcess(STFTImpl::getAnalysisWindowLength(order), STFTImpl::getFFTLength(order), nchannels)
{
  _impl.reset(new STFTImpl(nchannels, order));
}

STFT::~STFT()
{

}

void STFT::frameAnalysis(double *inFrame, double *analysis, int frameLength, int analysisLength, int channel)
{
  _impl->frameAnalysis(inFrame, analysis, frameLength, analysisLength, channel);
}

void STFT::frameSynthesis(double *outFrame, double *analysis, int frameLength, int analysisLength, int channel)
{
  _impl->frameSynthesis(outFrame, analysis, frameLength, analysisLength, channel);
}


int STFT::getAnalysisLength() const
{
  _impl->getAnalysisLength();
}

}

