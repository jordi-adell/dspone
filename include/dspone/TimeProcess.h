/*
* TimeProcess.h
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
*/
#ifndef __TIMEPROCESS_H_
#define __TIMEPROCESS_H_

#include "ShortTimeProcess.h"

namespace dsp{

class Timeprocess : public ShortTimeProcess
{
    public:

	Timeprocess(int channels, int frameLength);
	virtual ~Timeprocess();

    private:
	/**
	* @brief frameAnalysis
	* Implementation of the "none" frame analysis.
	* In fact it copies the frame from time domain directly to the analysis
	* frame. So, no analysis is made.
	* @param inFrame
	* @param analysis
	* @param frameLength
	* @param analysisLength
	* @param channel
	*/
	virtual void frameAnalysis(BaseType *inFrame, BaseType *analysis, int frameLength, int analysisLength, int channel);
	/**
	* @brief frameSynthesis
	*  This function just takes the analysis frame, interprets it as a time
	* frame and copies it into inthe output time frame, whithout any modification.
	* @param outFrame
	* @param analysis
	* @param frameLength
	* @param analysisLength
	* @param channel
	*/
	virtual void frameSynthesis(BaseType *outFrame, BaseType *analysis, int frameLength, int analysisLength, int channel);

	/**
	* @brief processParametrisation
	* You can implement this function and access _analysisFrames as
	* if they were time-frames for each input channel.
	* No analysis frames in done here.
	*/
	virtual void processParametrisation() = 0;

};

}

#endif
