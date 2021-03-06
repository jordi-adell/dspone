/*
* BandPassFFTWFilter.h
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
#ifndef __BANDPASSFFTWFILTER_H__
#define __BANDPASSFFTWFILTER_H__

#include <dspone/filter/BandPassFilter.h>
#include <memory>

namespace dsp {

class BandPassFFTWFilterImpl;

/** @brief implementation of a band pass filter using the spectrum weighting technique.
	 * By default uses a triangular shape with 1 as maxium gain. If the central frequency
	 * is supplied the triangle is assymetric. I filters buffers of 2^order samples.
	 * This is due to the FFT algorithm restrictions.
	 **/
class BandPassFFTWFilter : public BandPassFilter
{
    public:
	typedef enum{ RECTANGULAR, TRIANGULAR} filterShape;

	/**
	   * @brief BandPassFFTWFilter Contructs the object from the cut off frequencies
	   * @param order  order of the filter. The FFT used to filter witll contain 2^order samples
	   * @param lowFreq  low cut off frequency
	   * @param highFreq  high edge of the filter.
	   * @param centerFreq  central frequency (maximum gain applied here)
	   */
	BandPassFFTWFilter(const int &order, const double &lowFreq, const double &highFreq, filterShape shape = RECTANGULAR);
	BandPassFFTWFilter(const int &order, const double &lowFreq, const double &highFreq, const double &centerFreq);
	virtual ~BandPassFFTWFilter();

	/**
	   * @brief filterBuffer filters a buffer of 2^order samples long, thre different sample format can be supplied.
	   * @param inbuffer  input buffer were the signal is stored.
	   * @param outbuffer  output buffer to store the filtered signal.
	   * @param length  length of both input and output signals.
	   */
	virtual void filterBuffer(const int16_t *inbuffer, int16_t *outbuffer, int length);
	virtual void filterBuffer(const int32_t *inbuffer, int32_t *outbuffer, int length);
	virtual void filterBuffer(const uint16_t *inbuffer, uint16_t *outbuffer, int length);
	virtual void filterBuffer(const double *inbuffer, double *outbuffer, int length);
	virtual void filterBuffer(const float  *inbuffer, float  *outbuffer, int length);

	/** @brief getLowFreq
	   * @return the low edge frequency
	   **/
	double getLowFreq()  const;
	/**
	   * @brief getHighFreq
	   * @return the high cut off frequency
	   */
	double getHighFreq() const;
	/**
	   * @brief getCenterFreq
	   * @return returns the central digital frequency
	   */
	double getCenterFreq() const;

	/**
	   * @brief getCoeficients This function fills the coefs array with the <length> first coefficients of the filter.
	   * They function assumes the necessary memory has been already allocated.
	   * @param coefs  array to wehere coeficients are stored
	   * @param length  number of coeficients stored1
	   */
	void getCoeficients(double *coefs, int length) const;

	int getFrameLength() const;
    private:
	std::unique_ptr<BandPassFFTWFilterImpl> _impl;

};


}

#endif // __BANDPASSFFTWFILTER_H_
