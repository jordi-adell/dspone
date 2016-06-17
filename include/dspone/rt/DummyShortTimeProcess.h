/*
* DummyShortTimeProcess.h
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
* along with DSPONE.  If not, see <http://www.gnu.org/licenses/>.
*/#ifndef __DUMMY_SHORT_TIME_PROCESS_H_
#define __DUMMY_SHORT_TIME_PROCESS_H_

#include <dspone/rt/ShortTimeProcess.h>
#include <dspone/dspdefs.h>

#include <wipp/wipputils.h>

namespace dsp
{
/**
	 * @brief An implementation of the ShortTimeProcess
	 * where the signal is not modificed, is passed to the analysis buffer
	 * as is, and then copied to the output frame in the reconstruction (synthesis)
	 * step.
	 */
class DummyShortTimeProcess : public ShortTimeProcess
{

    public:
	DummyShortTimeProcess(int windowSize, bool generateDataChannel = false);
	~DummyShortTimeProcess();

    private:

	bool _generateDataChannel;
	static const int _constant = 1000;

	virtual void frameAnalysis (BaseType *inFrame,  BaseType *analysis, int frameLength, int analysisLength, int);
	virtual void processParametrisation(std::vector<double*> &analysisFrames, int analysisLength,
					    std::vector<double*> &dataChannels, int dataLength);
	virtual void frameSynthesis(BaseType *outFrame, BaseType *analysis, int frameLength, int analysisLength, int );
};

}

#endif
