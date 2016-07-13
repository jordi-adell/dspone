/*
* ShortTimeFourierSubBand.cpp
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

#ifndef __SHORTTIME_FOURIER_SUBBAND_H_
#define __SHORTTIME_FOURIER_SUBBAND_H_

#include <dspone/rt/ShortTimeFourierTransform.h>
#include <dspone/rt/ShortTimeFourierAnalysis.h>

namespace dsp {

class SubBandSTFTImpl;



class SubBandSTFT : public STFT
{

    public:
       typedef enum {MEL, LINEAR} FilterBankType;
	SubBandSTFT(int nbins, int sampleRate, int order, int channels,
		    float minFreq, float maxFreq, FilterBankType type=MEL);

	virtual ~SubBandSTFT();

    private:
       friend class SubBandSTFTImpl;
       virtual void processParametrisation(std::vector<double *> &analysisFrames, int analysisLength,
					   std::vector<double *> &dataChannels, int dataLength);
       std::unique_ptr<SubBandSTFTImpl> _impl;

    protected:

       virtual void processSetup(std::vector<double *> &analysisFrames, int analysisLength,
				 std::vector<double *> &dataChannels, int dataLength) = 0;

       virtual void processOneSubband(std::vector<double*> &analysisFrame, int length, int bin) = 0;

       virtual void processSumamry(std::vector<double *> &analysisFrames, int analysisLength,
				   std::vector<double *> &dataChannels, int dataLength) = 0;

};


class SubBandSTFTAnalysis : public STFTAnalysis
{
    public:
	SubBandSTFTAnalysis(int nbins, int sampleRate, int order, int channels,
		    float minFreq, float maxFreq, SubBandSTFT::FilterBankType type= SubBandSTFT::MEL);

	virtual ~SubBandSTFTAnalysis();

    private:
       friend class SubBandSTFTImpl;
       virtual void processParametrisation(std::vector<double *> &analysisFrames, int analysisLength,
					   std::vector<double *> &dataChannels, int dataLength);
       std::unique_ptr<SubBandSTFTImpl> _impl;

    protected:

       virtual void processSetup(std::vector<double *> &analysisFrames, int analysisLength,
				 std::vector<double *> &dataChannels, int dataLength) = 0;

       virtual void processOneSubband(std::vector<double*> &analysisFrame, int length, int bin) = 0;

       virtual void processSumamry(std::vector<double *> &analysisFrames, int analysisLength,
				   std::vector<double *> &dataChannels, int dataLength) = 0;

};


}

#endif
