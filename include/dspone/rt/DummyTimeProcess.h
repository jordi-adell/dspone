/*
* DummyTimeProcess.h
* Copyright 2017 (c) Jordi Adell
* Created on: 2017
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
* along with DSPONE.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __DUMMY_TIME_PROCESS_H__
#define __DUMMY_TIME_PROCESS_H__

#include <dspone/rt/TimeProcess.h>

namespace dsp
{

class DummyTimeProcess : public dsp::Timeprocess
{
  public:
    DummyTimeProcess(int nchannels, int frameLength) : dsp::Timeprocess(nchannels, frameLength)
    {

    }
    virtual ~DummyTimeProcess() {}

    virtual void processParametrisation(std::vector<double*> &analysisFrames, int analysisLength,
					std::vector<double*> &dataChannels, int dataLength)
    {
	return;
    }

};

}

#endif
