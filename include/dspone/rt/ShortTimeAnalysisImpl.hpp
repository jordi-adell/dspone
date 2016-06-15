/*
* ShortTimeAnalysisImpl.hpp
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
*/#ifndef __SHORTTIME_ANALISYS_IMPL_H_
#define __SHORTTIME_ANALISYS_IMPL_H_

#include <dspone/rt/ShortTimeAnalysis.h>
#include <dspone/rt/ShortTimeProcessImpl.h>


namespace dsp
{

class ShortTimeAnalysisImpl : public ShortTimeProcessImpl
{
    public:
	ShortTimeAnalysisImpl(ShortTimeProcess *frameProcessor, int windowSize , int analysisLength = 0, int nchannels = 1) :
	    ShortTimeProcessImpl(frameProcessor, windowSize, analysisLength, nchannels)
	{

	}

	virtual ~ShortTimeAnalysisImpl() {}

	template<typename SampleType>
	int process(const std::vector<SampleType *> &signal, unsigned int buffersize);

	template<typename SampleType>
	int process(const std::vector<std::shared_ptr<SampleType> > &signal, unsigned int buffersize);

};




template<typename SampleType>
int ShortTimeAnalysisImpl::process(const std::vector<std::shared_ptr<SampleType> > &signal,
			       unsigned int buffersize)
{
    unsigned int signalSize = signal.size();
    std::vector<SampleType*> auxSignal;

    if (signalSize < _nchannels)
    {
	throw(DspException("Wrong input number of channels"));
    }
    for (unsigned int i = 0; i <signalSize; ++i)
	auxSignal.push_back(signal[i].get());
    return process(auxSignal, buffersize);
}

template<typename SampleType>
int ShortTimeAnalysisImpl::process(const std::vector<SampleType *> &signal, unsigned int buffersize)
{
    std::vector<SampleType*> output;
    return ShortTimeProcessImpl::process(signal, buffersize, output, 0);
}


}


#endif
