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
#include <dspone/rt/ShortTimeFourierAnalysis.h>
#include <dspone/algorithm/fftImpl.h>
#include <dspone/DspException.h>

#include <wipp/wippdefs.h>
#include <wipp/wipputils.h>

#include <sstream>


namespace dsp {


class SubBandSTFTAnalysis : public STFTAnalysis
{

    public:
       typedef enum {MEL, LINEAR} FilterBankType;
       SubBandSTFTAnalysis(int nbins, int sampleRate, int order, int channels, float minFreq, float maxFreq, FilterBankType type=MEL);

    protected:
       const int _numberOfBins;
       const int _sampleRate;
       boost::scoped_ptr<FilterBank> _filterBank;
       boost::scoped_array<BaseTypeC> _filterCoeficients;
       int _coeficientsLength;
       SignalVector _processedAnalysisFrames;
       SignalVector _subbandAnalysisFrames;


       virtual void processParametrisation();
       virtual void processSetup() = 0;
       virtual void processOneSubband(const SignalVector &analysisFrame, int length, int bin) = 0;
       virtual void processSumamry() = 0;

};

}

