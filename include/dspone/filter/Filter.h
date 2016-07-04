/*
* Filter.h
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
#ifndef __DSP_I_FILTER_H_
#define __DSP_I_FILTER_H_


#include <stdint.h>

namespace dsp {

/**
       * @brief The Filter class
       * This is an interface for a filter, it could be a band pass, low pas, IIR, FIR ....
       * any filter. The specification here is that there is an input buffer
       * the is modified and the result is placed in the output buffer.
       * A filter outputs as many samples as the ones contained in the input buffer.
       */
class Filter
{
    public:
	Filter();
	virtual ~Filter();

	/**
	  * @brief filterBuffer filters a buffer of 2^order samples long, thre different sample format can be supplied.
	  * @param inbuffer  input buffer were the signal is stored.
	  * @param outbuffer  output buffer to store the filtered signal.
	  * @param length  length of both input and output signals.
	  */
	virtual void filterBuffer(const int16_t *inbuffer, int16_t *outbuffer, int length)  = 0;
	virtual void filterBuffer(const uint16_t *inbuffer, uint16_t *outbuffer, int length)  = 0;
	virtual void filterBuffer(const double *inbuffer, double *outbuffer, int length) = 0;
	virtual void filterBuffer(const float *inbuffer, float *outbuffer, int length) = 0;

	virtual int getFrameLength() const = 0;

};



}

#endif // FILTER_H_
