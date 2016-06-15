/*
* ShortTimeFourierTransform.h
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
#ifndef __STFT_IMPL_H_
#define __STFT_IMPL_H_

#include <dspone/rt/ShortTimeFourierTransform.h>
#include <dspone/rt/ShortTimeFourierAnalysis.h>
#include <dspone/algorithm/fftImpl.h>

namespace dsp {

/**
	 * @brief The STFT class
	 * In this class the analysis and synthesis functions are already implemented
	 * The analysis consists on optaining the short time Fouried transform (STFT)
	 * of the signal. The synthesis consists on reconstructing the singal performing
	 * and invers STFT over the analysis buffer. Any processing that has to be applied
	 * to the signal has to be implemented in the processParameterisation() function,
	 * and the modified spectrum stored back to the analysis buffer.
	 */
class STFTImpl
{

    public:
	STFTImpl(int order); // 2^9 = 512 window size and 256 window shift
	STFTImpl(int channels, int order);
	virtual ~STFTImpl();

    private:
	friend class STFT;
	friend class STFTAnalysis;

	static int getFFTLength(int order);
	static int getAnalysisWindowLength(int order);

	FFTImpl _fft;

	/**
	   * @brief Performes STFT over the given frame
	   * @param inFrame  frame signal
	   * @param analysis  analysis buffer
	   * @param frameLength  length of the given frame
	   * @param analysisLength  length of the analysis
	   */
	void frameAnalysis(double *inFrame, double *analysis, int frameLength, int analysisLength, int channel);

	/**
	   * @brief Computes the inverse STFT over the analysis buffer and stores the result
	   * in the given frmae
	   * @param outFrame  frame where the generated signal is stored
	   * @param analysis  analysis information (STFT complex values)
	   * @param frameLength  length of the output frame
	   * @param analysisLength  length of the analysis
	   */
	void frameSynthesis(double *outFrame, double *analysis, int frameLength, int analysisLength, int channel);

	/**
	   * @brief getAnalysisLength
	   * @return the length of the analysis buffer (2^order + 2)
	   * The length is 2 samples longer thant the frame length, because analysis
	   * consists of complex numbers representing the FFT of a real singal, and in
	   * a compact form it needs to more values.
	   */
	int getAnalysisLength() const;

};




}


#endif // __STFT_H_
