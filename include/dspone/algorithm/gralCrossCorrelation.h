/*
* gralCrossCorrelation.h
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
#ifndef __GRALCROSSCORRELATION_H_
#define __GRALCROSSCORRELATION_H_

#include <dspone/complex.h>

#include <memory>

namespace dsp{

class GeneralisedCrossCorrelationImpl;

class GeneralisedCrossCorrelation
{
	// This function calculates generalised correlation (GCC) in the frequency domain,
	// As stated in:
	//          Badali, Anthony, Jean-Marc Valin, Francois Michaud, and Parham Aarabi. 2009.
	//          “Evaluating Real-time Audio Localization Algorithms for Artificial Audition in Robotics.”
	//          In IEEE/RSJ International Conference on Intelligent Robots and Systems, 2033–2038.
	//          Ieee. doi:10.1109/IROS.2009.5354308. http://ieeexplore.ieee.org/lpdocs/epic03/wrapper.htm?arnumber=5354308.
	//
	//                      sum_w [ W(w)X_i(w)X_j*(w)exp(-jwtau) ]
	//          GCC  =     ---------------------------------------
	//                                       N

    public:

	typedef enum {ONESIDEDFFT, TWOSIDEDFFT} FFTtype;

	/**
	   * @brief GeneralisedCrossCorrelation
	   * @param length  length of the buffers that will be supplied, containing the FFT of the signal. It shoud be
	   * N_FFT when using two-sided (complete) FFT and (N_FFT/2+1) when using one-sided (positive semiaxis) FFT.
	   * @param FFTtype  selects the format of the FFT from {ONESIDEDFFT, TWOSIDEDFFT}
	   */
	GeneralisedCrossCorrelation(int length, FFTtype usedFFTtype);
	virtual ~GeneralisedCrossCorrelation();

	/**
	   * @brief calculateCorrelation  Calculates the GCC of a stereo signal for a given delay "tau"
	   * @param x  FFT of the first channel.
	   * @param y  FFT of the second channel.
	   * @param length  length of the supplied buffers. It shoud be N_FFT when using two-sided (complete) FFT
	   * and (N_FFT/2+1) when using one-sided (positive semiaxis) FFT.
	   * @param tau  delay in samples used to calculate the GCC.
	   * @param FFTtype  selects the format of the FFT from {ONESIDEDFFT, TWOSIDEDFFT}
	   * @return  correlation value.
	   */
	Complex calculateCorrelation(const Complex *x, const  Complex *y, int length, double tau, FFTtype usedFFTtype);

	/**
	   * @brief calculateCorrelationForTauVector  Calculates the GCC of a stereo signal for a set of delays
	   * given by the vector "samplesDelay".
	   * @param x  FFT of the first channel.
	   * @param y  FFT of the second channel.
	   * @param c  output vector which contains the correlation values for the given delays, in the same order
	   * as in "samplesDelay".
	   * @param length  length of the supplied buffers. It shoud be N_FFT when using two-sided (complete) FFT
	   * and (N_FFT/2+1) when using one-sided (positive semiaxis) FFT.
	   * @param samplesDelay  delays vector in samples used to compute the GCC.
	   * @param numSteps  length of "samplesDelay".
	   * @param FFTtype  selects the format of the FFT from {ONESIDEDFFT, TWOSIDEDFFT}
	   */
	void calculateCorrelationsForTauVector(const Complex *x, const Complex *y, Complex *c, int length,
					       double *samplesDelay, int numSteps, FFTtype usedFFTtype);

	/**
	   * @brief precomputeTauMatrix  Precomputes a delays matrix which will be used to calculate the GCC using the function
	   * "calculateCorrelationsForPrecomputedTauMatrix". This option accelerates significantly the calculation of the GCC.
	   * @param samplesDelay  delays vector in samples that will be used to compute the GCC.
	   * @param numSteps  length of "samplesDelay".
	   * @param length  length of the supplied buffers. It shoud be N_FFT when using two-sided (complete) FFT
	   * and (N_FFT/2+1) when using one-sided (positive semiaxis) FFT.
	   * @param FFTtype  selects the format of the FFT from {ONESIDEDFFT, TWOSIDEDFFT}
	   */
	void precomputeTauMatrix(double *samplesDelay, int numSteps, int length, FFTtype usedFFTtype);

	/**
	   * @brief calculateCorrelationsForPrecomputedTauMatrix  Calculates the GCC of a stereo signal for a set of delays, using
	   * the delays matrix which has been precomputed with the function "precomputeTauMatrix".
	   * @param x  FFT of the first channel.
	   * @param y  FFT of the second channel.
	   * @param c  output vector which contains the correlation values for the delays specified when precomputing the delays matrix.
	   * @param length  length of the supplied buffers. It shoud be N_FFT when using two-sided (complete) FFT
	   * and (N_FFT/2+1) when using one-sided (positive semiaxis) FFT.
	   * @param numSteps  length of the delays vector used for precomputing the delays matrix.
	   * @param FFTtype  selects the format of the FFT from {ONESIDEDFFT, TWOSIDEDFFT}
	   */
	void calculateCorrelationsForPrecomputedTauMatrix(const Complex *x, const Complex *y, Complex *c, int length,
							  int numSteps, FFTtype usedFFTtype);

    private:
	std::unique_ptr<GeneralisedCrossCorrelationImpl> _impl;
};

}

#endif // __GRALCROSSCORRELATION_H_
