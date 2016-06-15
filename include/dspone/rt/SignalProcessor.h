/*
* SignalProcessor.h
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
#ifndef __ISHORTTIMEPROCESS_H_
#define __ISHORTTIMEPROCESS_H_

#include <dspone/rt/ProcessDispatcher.h>
#include <vector>

namespace dsp {

class ShortTimeProcess;
class ShortTimeAnalysis;

/**
	 * @brief The IShortTimeProcess class
	 * Interface class to expose the API for classes based on ShortTimeProcess
	 * ShortTimeProcess is the Overlap and Add process of audio signals for
	 * realtime audio processing.
	 */
class SignalProcessor : public ProcessDispatcher<SignalProcessor>
{
    public:

	SignalProcessor();
	virtual ~SignalProcessor();

	/**
	   * @brief process   Process an input signal as decribed in @class SignalProcessingDispatcher
	   * for int16_t data type.
	   */
	int process(const std::vector<int16_t *> &signal,
		    unsigned int inbuffersize,
		    const std::vector<int16_t *> &output,
		    unsigned int outbuffersize);
	int process(const std::vector<std::shared_ptr<int16_t> > &signal,
		    unsigned int inbuffersize,
		    const std::vector<std::shared_ptr<int16_t> > &output,
		    unsigned int outbuffersize);
	/**
	   * @brief process   Process an input signal as decribed in @class SignalProcessingDispatcher
	   * for float data type.
	   */
	int process(const std::vector<float *> &signal,
		    unsigned int inbuffersize,
		    const std::vector<float *> &output,
		    unsigned int outbuffersize);
	int process(const std::vector<std::shared_ptr<float> > &signal,
		    unsigned int inbuffersize,
		    const std::vector<std::shared_ptr<float> > &output,
		    unsigned int outbuffersize);
	/**
	   * @brief process   Process an input signal as decribed in @class SignalProcessingDispatcher
	   * for double data type.
	   */
	int process(const std::vector<double *> &signal,
		    unsigned int inbuffersize,
		    const std::vector<double *> &output,
		    unsigned int outbuffersize);
	int process(const std::vector<std::shared_ptr<double> > &signal,
		    unsigned int inbuffersize,
		    const std::vector<std::shared_ptr<double> > &output,
		    unsigned int outbuffersize);



	virtual int getLatency() const;
	virtual int getMaxLatency() const;
	virtual int getBufferSize() const;
	virtual int getNumberOfChannels() const;

    protected:
	std::unique_ptr<ShortTimeProcess> _impl;  /**< Actual implementation of the algorithm for signal processing. */
};



/**
	 * @brief The IShortTimeAnalysis class
	 * Interface class to expose the API for classes based on ShortTimeAnalysis
	 * ShortTimeProcess is the Overlap and Add process of audio signals for
	 * realtime audio processing.
	 */
class SignalAnalyser : public ProcessDispatcher<SignalAnalyser>
{
    public:

	SignalAnalyser();
	virtual ~SignalAnalyser();

	/**
	   * @brief process   Process an input signal as decribed in @class SignalProcessingDispatcher
	   * for int16_t data type.
	   */
	virtual int process(const std::vector<int16_t *> &signal, unsigned int buffersize);
	virtual int process(const std::vector<std::shared_ptr<int16_t> > &signal, unsigned int buffersize);
	/**
	   * @brief process   Process an input signal as decribed in @class SignalProcessingDispatcher
	   * for float data type.
	   */
	virtual int process(const std::vector<float *> &signal, unsigned int buffersize);
	virtual int process(const std::vector<std::shared_ptr<float> > &signal, unsigned int buffersize);
	/**
	   * @brief process   Process an input signal as decribed in @class SignalProcessingDispatcher
	   * for double data type.
	   */
	virtual int process(const std::vector<double *> &signal, unsigned int buffersize);
	virtual int process(const std::vector<std::shared_ptr<double> > &signal, unsigned int buffersize);


	/** Latency managment functions **/
	virtual int getLatency() const;
	virtual int getMaxLatency() const;
	virtual int getBufferSize() const;
	virtual int getNumberOfChannels() const;

    protected:
	std::unique_ptr<ShortTimeAnalysis> _impl; /**< Actual implementation of the algorithm for signal analysis. */
};



}


#endif // __ISHORTTIMEPROCESS_H_
