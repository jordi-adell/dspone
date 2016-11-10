/*
* ShortTimeProcessAnalysis.h
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

#ifndef __SHORT_TIME_ANALYSIS_H_
#define __SHORT_TIME_ANALYSIS_H_

#include <dspone/rt/SignalAnalyser.h>
#include <dspone/rt/ShortTimeProcess.h>

namespace dsp
{

class ShortTimeAnalysisImpl;

/**
	 * @brief The ShortTimeAnalysis class
	 * This class has to be used when only analysis is needed.
	 * If you use this class, the reconstruction of the signal is not done.
	 */
class ShortTimeAnalysis : public SignalAnalyser, public ShortTimeProcess
{
    public:
	ShortTimeAnalysis(int windowSize , int analysisLength = 0, int nchannels = 1);
	virtual ~ShortTimeAnalysis();

	virtual int process(const std::vector<double *> &signal, unsigned int buffersize);
	int process(const std::vector<boost::shared_array<double> > &signal, unsigned int buffersize);

	virtual int process(const std::vector<float*> &signal, unsigned int buffersize);
	int process(const std::vector<boost::shared_array<float> > &signal, unsigned int buffersize);

	virtual int process(const std::vector<int16_t*> &signal, unsigned int buffersize);
	int process(const std::vector<boost::shared_array<int16_t> > &signal, unsigned int buffersize);

	virtual int process(const std::vector<uint16_t*> &signal, unsigned int buffersize);
	int process(const std::vector<boost::shared_array<uint16_t> > &signal, unsigned int buffersize);

    protected:

	virtual int getLatency() const;
	virtual int getMaxLatency() const;
	virtual int getBufferSize() const;
	virtual int getNumberOfChannels() const;


    private:
	virtual void frameSynthesis(double *outFrame, double *analysis, int frameLength, int analysisLength, int channel);
	std::unique_ptr<ShortTimeAnalysisImpl> _impl;

};


}


#endif
