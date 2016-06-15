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
#include <dspone/rt/STFTImpl.h>
#include <dspone/dspdefs.h>


namespace dsp {

STFTAnalysis::STFTAnalysis(int channels, int order) :
  ShortTimeAnalysis(STFTImpl::getAnalysisWindowLength(order), STFTImpl::getFFTLength(order), channels)
{

}

STFTAnalysis::STFTAnalysis(int order) :
ShortTimeAnalysis(STFTImpl::getAnalysisWindowLength(order), STFTImpl::getFFTLength(order))
{

}

STFTAnalysis::~STFTAnalysis()
{

}

//STFTAnalysis::frameSynthesis(double *outFrame, double *analysis, int frameLength, int analysisLength, int channel)
//{

//}


void STFTAnalysis::frameAnalysis(double *inFrame, double *analysis, int frameLength, int analysisLength, int channel)
{
    _impl->frameAnalysis(inFrame, analysis, frameLength, analysisLength, channel);
}



}

