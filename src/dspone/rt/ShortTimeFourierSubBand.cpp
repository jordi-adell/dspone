/*
* stft.cpp
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
#include <dspone/rt/ShortTimeFourierSubBand.h>
#include <dspone/rt/ShortTimeFourierSubBandImpl.h>

namespace dsp {

SubBandSTFTAnalysis::SubBandSTFTAnalysis(int nbins, int sampleRate,
			 int order, int channels, float minFreq, float maxFreq, SubBandSTFT::FilterBankType type) :
  STFTAnalysis(channels, order)
{
  _impl.reset(new SubBandSTFTImpl(this, nbins, sampleRate, order, channels, minFreq, maxFreq, type));
}

SubBandSTFTAnalysis::~SubBandSTFTAnalysis()
{
  _impl.reset();
}

void SubBandSTFTAnalysis::processParametrisation(std::vector<double *> &analysisFrames, int analysisLength,
					 std::vector<double *> &dataChannels, int dataLength)
{
  _impl->processParametrisation(analysisFrames, analysisLength, dataChannels, dataLength);
}




SubBandSTFT::SubBandSTFT(int nbins, int sampleRate,
			 int order, int channels, float minFreq, float maxFreq, FilterBankType type) :
  STFT(channels, order)
{
  _impl.reset(new SubBandSTFTImpl(this, nbins, sampleRate, order, channels, minFreq, maxFreq, type));
}

SubBandSTFT::~SubBandSTFT()
{
  _impl.reset();
}

void SubBandSTFT::processParametrisation(std::vector<double *> &analysisFrames, int analysisLength,
					 std::vector<double *> &dataChannels, int dataLength)
{
  _impl->processParametrisation(analysisFrames, analysisLength, dataChannels, dataLength);
}



}

