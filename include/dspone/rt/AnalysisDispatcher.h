/*
* SignalProcessingDispatcher.h
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
#ifndef __SHORTTIMEPROCESSDISPATCHER_H_
#define __SHORTTIMEPROCESSDISPATCHER_H_

#include <boost/shared_array.hpp>
#include <vector>

namespace dsp {


/**
	 * @brief The SignalAnalysisDispatcher to have a teplate
	 * funtion process that can process data of any type.
	 * Then, depends on the Derived class implementation to
	 * actually implement a process call for each type.
	 * However, any user having a pointer to this class
	 * will be able to call process with any data type.
	 */
template<class Derived>
class AnalysisDispatcher
{
    public:

	AnalysisDispatcher(){}
	virtual ~AnalysisDispatcher(){}

	/**
	   * @brief process a list of input channels an places the result in a
	   * list of output channels. Each channels is representated as a shared
	   * pointer in a standard vector.
	   * @param signal    input channels
	   * @param inbuffersize    length of the input buffers.
	   * @param output    output channels
	   * @param outbuffersize   length of the output buffers.
	   * @return    number of samples added to each output buffer.
	   */
	template <typename SampleType>
	int process(const std::vector<boost::shared_array<SampleType> > &signal,
		    unsigned int inbuffersize)
	{
	    return  static_cast<Derived*>(this)->process(signal, inbuffersize);
	}
	/**
	   * @brief process a list of input channels an places the result in a
	   * list of output channels. Each channels is representated as a pointer
	   * in a standard vector. The class does not retain ownership of vectors.
	   * @param signal    input channels
	   * @param inbuffersize    length of the input buffers.
	   * @param output    output channels
	   * @param outbuffersize   length of the output buffers.
	   * @return    number of samples added to each output buffer.
	   */
	template <typename SampleType>
	int process(const std::vector<SampleType*> &signal,
		    unsigned int inbuffersize)
	{
	    return static_cast<Derived*>(this)->process(signal, inbuffersize);
	}


	/**
	   * @brief getLatency   Returns the number of samples pending to process
	   * at the current time.
	   * @return    number of samples pending to process.
	   */
	virtual int getLatency() const = 0;
	/**
	   * @brief getMaxLatency  The maximum number of samples that can be pending to process.
	   * @return     number of samples pending to process
	   */
	virtual int getMaxLatency() const = 0;

	/**
	 * @brief getNumberOfChannels
	 * @return   number of channels to process.
	 */
	virtual int getNumberOfChannels() const = 0;
};


}


#endif // __SHORTTIMEPROCESSDISPATCHER_H_

