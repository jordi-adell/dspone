/*
* BandPassFIRFilter.h
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
#ifndef __BANDPASSFIRFILTER_H_
#define __BANDPASSFIRFILTER_H_

#include <dspone/filter/BandPassFilter.h>
#include <dspone/filter/FIRFilter.h>
#include <memory>


namespace dsp {

struct BandPassFIRFilterImpl;

/** @brief This class implements a band pass filter using the FIR approach.
	   * The design of the filter specifications is done by the IPP library by
	   * windowing the impulse response of the ideal filter.
	   */
class BandPassFIRFilter : public BandPassFilter
{
    public:
	typedef enum {RECTANGULAR,
		      TRIANGULAR
		     } FrequencyShape;


	/** @brief The contructor of the band pass filter. The two cutoff digital
	   * frequencies have to be supplied.
	   * @param order Order of the filter, the length of filter impulse response will be 2^order
	   * @param lowFreq lowest frequency that will pass the filter.
	   *        Needs to be in the range [0 - 0.5] because of the Nyquist limit
	   * @param lowFreq highest frequency that will pass the filter.
	   *        Needs to be in the range [0 - 0.5] because of the Nyquist limit
	   */
	BandPassFIRFilter(const int order, const double lowFreq, const double highFreq, FrequencyShape shape = RECTANGULAR);
	virtual ~BandPassFIRFilter();

	/** @brief This function filters a buffer given the filter parmaeters and the previous history
	   * @param inbuffer  buffer to filter
	   * @param outbuffer  buffer to store the result
	   * @param length  number of samples in the input (and output) filter
	   */
	virtual void filterBuffer(const int16_t *inbuffer, int16_t *outbuffer, int length);
	virtual void filterBuffer(const float  *inbuffer, float  *outbuffer, int length);
	virtual void filterBuffer(const double *inbuffer, double *outbuffer, int length);

	virtual inline double getLowFreq() const {return _lowFreq;}
	virtual inline double getHighFreq() const {return _highFreq;}
	virtual inline int    getFrameLength() const {return -1;}

	/**
	   * @brief getCoeficients  Returns a vector with the impulsional response of the filter (useful for debug purposes)
	   * @param coefs  impulsional response
	   * @param length  the fucntion will return the length first values of the impuls response
	   */
	virtual inline void getCoeficients(double *coefs, int length) const;

    private:
	//typedef double  Ipp64f;
	typedef double BaseType;
	friend struct BandPassFIRFilterImpl;
	std::unique_ptr<BandPassFIRFilterImpl> _impl;
	const BaseType _lowFreq;
	const BaseType _highFreq;


	/** @brief This fucntion is called by the constructor to initialise the IPP filter
	   * @param order  order of the filter
	   * @param lowFreq low cut-off frequency
	   * @param highFreq higest cut-off frequency
	   */
	void initialiseFilter();
	void initialiseRectangularCoefs();
	void initialiseTriangularCoefs();

	/**
	   * @brief This function performs the actual filtering process, it is called by the three public
	   * filterBuffer(...) fucntions
	   */
	template <class T> void filterBufferCore(const T *inbuffer, T *outbuffer, int length);

	/** @brief This function checks the parameters supplied to accomlish the requirments of the class
	   * it throws and exception if they do not */
	void checkParameters() const;// throws if error

};


}


#endif // __BANDPASSFIRFILTER_H_
