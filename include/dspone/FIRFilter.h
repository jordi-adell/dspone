/*
* FIRFilter.h
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
#ifndef __FIRFILTER_H_
#define __FIRFILTER_H_

#include <dspone/Filter.h>

#include <stdint.h>
#include <string>
#include <boost/scoped_ptr.hpp>

namespace dsp {

class FIRFilterImpl;

class FIRFilter : public Filter
{
    public:
	/** Initialises a FIR filter with the specified length and coeficients
	  @param coefs  Vector where coefficients are stored
	  @param length Number of coefficients of the filter
	  **/
	FIRFilter(const double *coefs, int length);
	/** Same as previous constructor, but coefficients are read from a text file */
	FIRFilter(const std::string &file);

	~FIRFilter();

	/** This fucntion filteres a buffer, taking into account previous filtered signal  as history,
	  then writes the output in outBuffer.
	  @param inBuffer  signal to be filtered
	  @param outBuffer place to store the desired signal, must be already allocated and of size length
	  **/
	virtual void filterBuffer(const int16_t *inBuffer, int16_t *outBuffer, int length);
	virtual void filterBuffer(const float  *inBuffer, float  *outBuffer, int length);
	virtual void filterBuffer(const double *inBuffer, double *outBuffer, int length);

	virtual int getFrameLength() const { return -1;}

	/** return the order of the filter **/
	int getOrder();

    private:
	boost::scoped_ptr<FIRFilterImpl> _impl;

};


}

#endif // __FIRFILTER_H_
