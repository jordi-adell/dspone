/*
* TimeProcess.cpp
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
* along with WIPP.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <dspone/TimeProcess.h>
#include <wipp/wipputils.h>

namespace dsp{

Timeprocess::Timeprocess(int nchannels, int frameLength) :
  ShortTimeProcess(frameLength, frameLength, nchannels)
{

}
void Timeprocess::frameAnalysis(BaseType *inFrame,
				BaseType *analysis,
				int frameLength,
				int analysisLength, int)
{
  int length = std::min(frameLength, analysisLength); // fameLength and analysisLength shoudl be equal
  wipp::copyBuffer(inFrame, analysis, length);
}

void Timeprocess::frameSynthesis(BaseType *outFrame,
				 BaseType *analysis,
				 int frameLength,
				 int analysisLength, int)

{
  int length = std::min(frameLength, analysisLength); // fameLength and analysisLength shoudl be equal
  wipp::copyBuffer(analysis, outFrame, length);
}

Timeprocess::~Timeprocess()
{

}

}
