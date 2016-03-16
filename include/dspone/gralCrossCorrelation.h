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

#include <dspone/dspdefs.h>

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_array.hpp>


namespace dsp{

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
	BaseTypeC calculateCorrelation(const  BaseTypeC *x, const  BaseTypeC *y, int length, BaseType tau, FFTtype usedFFTtype);

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
	void calculateCorrelationsForTauVector(const BaseTypeC *x, const BaseTypeC *y, BaseTypeC *c, int length,
					       BaseType *samplesDelay, int numSteps, FFTtype usedFFTtype);

	/**
	   * @brief precomputeTauMatrix  Precomputes a delays matrix which will be used to calculate the GCC using the function
	   * "calculateCorrelationsForPrecomputedTauMatrix". This option accelerates significantly the calculation of the GCC.
	   * @param samplesDelay  delays vector in samples that will be used to compute the GCC.
	   * @param numSteps  length of "samplesDelay".
	   * @param length  length of the supplied buffers. It shoud be N_FFT when using two-sided (complete) FFT
	   * and (N_FFT/2+1) when using one-sided (positive semiaxis) FFT.
	   * @param FFTtype  selects the format of the FFT from {ONESIDEDFFT, TWOSIDEDFFT}
	   */
	void precomputeTauMatrix(BaseType *samplesDelay, int numSteps, int length, FFTtype usedFFTtype);

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
	void calculateCorrelationsForPrecomputedTauMatrix(const BaseTypeC *x, const BaseTypeC *y, BaseTypeC *c, int length,
							  int numSteps, FFTtype usedFFTtype);



    private:

	int _length; /**< the length of the provided buffers */
	const FFTtype _usedFFTtype; /**< enum that indicates the type of FFT that is used (one sided / two sided).  */
	bool _delayMatrixIsPrecomputed; /**< flag that indicates if the delays matrix is precomputed.  */
	int _precomputedNumSteps; /**< number of precomputed delays. */


	BaseTypeC _corrValue; /**< used to store the correlation result.  */
	BaseTypeC _mean; /**< used to compute the mean. */
	SignalPtr _phase; /**< used to compute the phase ramp */
	SignalPtr _ones; /**< used to compute the phase ramp */
	SignalCPtr _delay;  /**< used to compute the phase ramp [exp(jw*_phase)] */
	SignalPtr _magnitude; /**< used to compute the PHAT */
	SignalPtr _phat; /**< used to compute the PHAT weights */
	SignalCPtr _weight; /**< used to store the PHAT weights */
	SignalCPtr _correlation; /**< used to compute the correlation */
	SignalCPtr _generalisedcc; /**< used to compute the correlation */
	SignalCPtr _fullx; /**< used to store the two-sided FFT (only when using one-side FFT) */
	SignalCPtr _fully; /**< used to store the two-sided FFT (only when using one-side FFT) */
	std::vector<SignalCPtr> _delayMatrix; /**< matrix used to store the precomupted phase ramps (for optimization purposes) */


	/**
	   * @brief debugPlot  Plots a debug graphic using gnuplot and current values in
	   * memeber variables.
	   * @param left   left channel
	   * @param right   right channel
	   * @param length  length of the signals
	   * @param tau  delay in samples.
	   */
	void debugPlot(const BaseTypeC* left, const BaseTypeC *right, const BaseTypeC *c, int length, int numSteps, float tau);

	/**
	   * @brief generateTwoSidedFFT  Generates the values of the negative semiaxis of the FFT in order to
	   * obtain the two-sided (complete) FFT. It only works for real signals (symmetric spectrum).
	   * @param x   one-sided (positive semiaxis) FFT of the first channel.
	   * @param y   one-sided (positive semiaxis) FFT of the second channel.
	   * @param xf  two-sided (complete) output FFT of the first channel.
	   * @param yf  two-sided (complete) output FFT of the second channel.
	   * @param length  length of the supplied buffers (it should be N_FFT/2+1)
	   */
	void generateTwoSidedFFT(const BaseTypeC *x, const BaseTypeC *y, BaseTypeC *xf, BaseTypeC *yf, int length);

	/**
	   * @brief calculateCorrelationsForPhaseRange  This function is called by the public functions to
	   * compute the correlation.
	   * @param x  two-sided (complete) FFT of the first channel.
	   * @param y  two-sided (complete) FFT of the second channel.
	   * @param c  output vector with the correlation values for the given delays.
	   * @param samplesDelay  delays vector in samples for the GCC.
	   * @param numSteps  length of "samplesDelay".
	   * @param usePrecomputedMatrix  flag that indicates if the precomputed matrix must be used
	   */
	void calculateCorrelationsCore(const  BaseTypeC *x, const  BaseTypeC *y, BaseTypeC *c,
				       BaseType *samplesDelay, int numSteps, bool usePrecomputedMatrix);

	/**
	   * @brief phatWeight   PHAT weighting function for GCC as explained in:
	   *  Badali, Anthony, Jean-Marc Valin, Francois Michaud, and Parham Aarabi. 2009.
	   *  “Evaluating Real-time Audio Localization Algorithms for Artificial Audition in Robotics.”
	   *  In IEEE/RSJ International Conference on Intelligent Robots and Systems, 2033–2038.
	   *  Ieee. doi:10.1109/IROS.2009.5354308. http://ieeexplore.ieee.org/lpdocs/epic03/wrapper.htm?arnumber=5354308.
	   * @param x   first signal channel
	   * @param y   second signal channel
	   * @param w   vector to store the PHAT vector.
	   * @param length   length of the vectors
	   */
	void phatWeight(const  BaseTypeC *x, const  BaseTypeC *y,  BaseTypeC *w);

	/**
	   * @brief computeDelayRamp  Computes the phase ramp for a given delay tau.
	   * @param tau  delay in samples.
	   */
	void computePhaseRamp(BaseType tau);

	void allocate();
};

}

#endif // __GRALCROSSCORRELATION_H_
