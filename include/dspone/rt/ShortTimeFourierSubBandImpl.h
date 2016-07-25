/*
* ShortTimeFourierSubBandAnalysis.cpp
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

#ifndef __SHORTTIME_FOURIER_SUBBAND_IMPL_H_
#define __SHORTTIME_FOURIER_SUBBAND_IMPL_H_

#include <dspone/rt/STFTImpl.h>
#include <dspone/rt/ShortTimeFourierSubBand.h>
#include <dspone/filter/FilterBank.h>
#include <dspone/dspdefs.h>

#include <sstream>

namespace dsp {

class SubBandSTFTImpl
{

    private:
	typedef SubBandSTFT::FilterBankType FilterBankType;

    public:

	SubBandSTFTImpl(SubBandSTFT *owner,
		int nbins, int sampleRate, int order,
		int channels, float minFreq, float maxFreq,
		FilterBankType type=SubBandSTFT::MEL);

	SubBandSTFTImpl(SubBandSTFTAnalysis *owner,
		int nbins, int sampleRate, int order,
		int channels, float minFreq, float maxFreq,
		FilterBankType type=SubBandSTFT::MEL);


	~SubBandSTFTImpl();

	virtual void processParametrisation(std::vector<double *> &analysisFrames, int analysisLength,
					    std::vector<double *> &dataChannels, int dataLength);

	int getNumberOfBins() const;

    private:
       const int _numberOfBins;
       const int _sampleRate;
       const int _nchannels;
       const int _order;
       const int _analysisLength;
       std::unique_ptr<FilterBank> _filterBank;
       std::unique_ptr<Complex> _filterCoeficients;
       int _coeficientsLength;
       SignalVector _processedAnalysisFrames;
       SignalVector _subbandAnalysisFrames;
       std::vector<BaseType*> _processedAnalysisFramesPtr;
       std::vector<BaseType*> _subbandAnalysisFramesPtr;

       SubBandSTFT *_owner;
       SubBandSTFTAnalysis *_owner_a;
       void init(FilterBankType type, float minFreq, float maxFreq, float sampleRate);

       void processParametrisation_core(std::vector<double *> &analysisFrames, int analysisLength,
					std::vector<double *> &dataChannels, int dataLength);

       void processParametrisation_core_a(std::vector<double *> &analysisFrames, int analysisLength,
					  std::vector<double *> &dataChannels, int dataLength);


};

}

#endif
