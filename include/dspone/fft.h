/*
* fft.h
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
#ifndef __FFT_H_
#define __FFT_H_

#include <dspone/dspdefs.h>

namespace dsp {

class FFTImpl;

/**
	  * @brief 1D Fast Fourier Transform
	 */
class FFT
{

    public:
	FFT(int order);
	~FFT();

	int getOrder() const;
	int getFFTLength() const;
	int getOneSidedFFTLength() const;
	int getAnalysisWindowLength() const;

	/**
	   * @brief getFFTLength   Gets you the length of an FFT given the order
	   * @param order  FFT order
	   * @return   the samples of the FFT
	   */
	static int getFFTLength(int order);
	/**
	   * @brief getAnalysisWindowLength   Returns the maximum length of the
	   * input signal given the FFT order.
	   * @param order  FFT order
	   * @return   the length of the equivalent square analysis window.
	   */
	static int getAnalysisWindowLength(int order);

	/**
	   * @brief fwdTransform   Performed the forward/direct transform
	   * If the frameLength is not provided, then the value returned by
	   * getFFRLength is assumed.
	   * @param inFrame  Frame to calculate the FFT on.
	   * @param fft    buffer to store the values of tHe FFT
	   * @param frameLength   length of the frame provided.
	   * Has to be lower or equal to the value returned by getFFTLength
	   */
	void fwdTransform(BaseType *inFrame, BaseType *fft);
	void fwdTransform(BaseType *inFrame, BaseType *fft, int frameLength);

	/**
	   * @brief invTransfrom   Performed the inverse transform
	   * If the frameLength is not provided, then the value returned by
	   * getFFRLength is assumed.
	   * @param outFrame  buffer to store the values of the IFFT
	   * @param fft    the FFT to be inverse transformed
	   * @param frameLength   length of the frame provided.
	   * Has to be lower or equal to the value returned by getFFTLength
	   */
	void invTransfrom(BaseType *outFrame, BaseType *fft);
	void invTransfrom(BaseType *outFrame, BaseType *fft, int frameLength);

    private:
	std::unique_ptr<FFTImpl>_impl;

};

}



#endif // __FFT_H_
