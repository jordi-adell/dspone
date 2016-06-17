/*
* FFTWeightingFilter.h
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
#ifndef __FFTWEIGHTINGFILTER_H_
#define __FFTWEIGHTINGFILTER_H_

#include <dspone/filter/Filter.h>
#include <dspone/dsplogger.h>

#include <boost/shared_array.hpp>

#include <iostream>
#include <fstream>

namespace dsp {

class FFT;

/** @brief This class filters a signal by calculating the FFT of the buffer,
	 * multiplies the spectrum by the vector of coeficients provided and then
	 * the output signal is obtain by obtaining the inverse FFT of the modified
	 * spectrum.
	 **/
class FFTWeightingFilter : public Filter
{
    public:
	/** @brief To construct the filter you need to provide the coeficients.
	   * The length of the coeficients vector needs to be half of the FFT length.
	   * This is because for real signal the FFT is symetric.
	   * @param coefs the coeficients to multiply the spectrum
	   * @param length the length of the supplied vector
	   **/
	FFTWeightingFilter(const double *coefs, int length);
	~FFTWeightingFilter();

	/**
	   * @brief filters the supplied buffer and stores the result in outbuffer
	   * @param inbuffer input buffer to be filtered
	   * @param outbuffer output buffer to store the result
	   * @param length length of input and output buffer. Needs to be twiece the length
	   * of the coeficients vector supplied (at least), this is the length of the FFT.
	   */
	virtual void filterBuffer(const int16_t *inbuffer, int16_t *outbuffer, int length);
	virtual void filterBuffer(const float  *inbuffer, float  *outbuffer, int length);
	virtual void filterBuffer(const double *inbuffer, double *outbuffer, int length);

	void spectralWeighting(double *spectrum, int length) const;
	void fft(const double *signal, int signallength, double *spectrum, int speclength) const;
	void ifft(const double* spectrum, int speclength, double *signal, int signallength) const;

	virtual int getFrameLength() const { return _length;}

    private:
	int _order;
	int _length;
	int _specLength;
	int _coefsLength;
	boost::shared_array<double> _coefs;
	boost::shared_array<double> _spectrum;

	std::shared_ptr<FFT> _fft;

	/** @brief this function is called by the constructor to initialise the IPP filter data */
	void initialiseFilter(const double *coefs, int length);

	/**
	   * @brief This fucntion performs the actual filtering process.
	   * It is called by the public filterBuffer(...) functions.
	   * @param inbuffer  input signal
	   * @param outbuffer  output signal (is assumed to be already allocated)
	   * @param length  length of both buffers
	   */
	inline void filterBufferCore(const double *inbuffer, double *outbuffer, int length) const;


};


}

#endif // FFTWEIGHTINGFILTER_H_
