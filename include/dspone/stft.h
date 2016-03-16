/*
* stft.h
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
#ifndef __STFT_H_
#define __STFT_H_

#include <dspone/FilterBankMelScale.h>
#include <dspone/ShortTimeProcess.h>
#include <dspone/fft.h>

#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>

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
class STFT : public ShortTimeProcess
{

    public:

	STFT(int order = _fftOrderDefault); // 2^9 = 512 window size and 256 window shift
	STFT(int channels, int order);
	virtual ~STFT();
    protected:
	FFT _fft;
	const int _oneSidedFFTLength; // Depreacted
	const int _fftOrder; // Deprecated
	static const int _fftOrderDefault = 9; // should be private
    private:

	/**
	   * @brief Performes STFT over the given frame
	   * @param inFrame  frame signal
	   * @param analysis  analysis buffer
	   * @param frameLength  length of the given frame
	   * @param analysisLength  length of the analysis
	   */
	virtual void frameAnalysis(BaseType *inFrame, BaseType *analysis, int frameLength, int analysisLength, int channel);

	/**
	   * @brief Computes the inverse STFT over the analysis buffer and stores the result
	   * in the given frmae
	   * @param outFrame  frame where the generated signal is stored
	   * @param analysis  analysis information (STFT complex values)
	   * @param frameLength  length of the output frame
	   * @param analysisLength  length of the analysis
	   */
	virtual void frameSynthesis(BaseType *outFrame, BaseType *analysis, int frameLength, int analysisLength, int channel);

	/**
	   * @brief You need to implement here a function that processes the
	   * analysis buffers in order to apply the digital processing algorithm
	   * to be implemented (core function)
	   */
	virtual void processParametrisation() = 0;
	SignalCPtr _ccsConversion;
    protected:


	/**
	   * @brief getAnalysisLength
	   * @return the length of the analysis buffer (2^order + 2)
	   * The length is 2 samples longer thant the frame length, because analysis
	   * consists of complex numbers representing the FFT of a real singal, and in
	   * a compact form it needs to more values.
	   */
	virtual int getAnalysisLength() const;

};

/**
	 * @brief The STFTAnalysis class
	 */
class STFTAnalysis : public ShortTimeAnalysis
{
    public:
	STFTAnalysis(int order);
	STFTAnalysis(int channels, int order);
	virtual ~STFTAnalysis(){}

    protected:
	FFT _fft;

    private:
	/**
	   * @brief Performes STFT over the given frame
	   * @param inFrame  frame signal
	   * @param analysis  analysis buffer
	   * @param frameLength  length of the given frame
	   * @param analysisLength  length of the analysis
	   */
	virtual void frameAnalysis(BaseType *inFrame, BaseType *analysis, int frameLength, int analysisLength, int channel);
	/**
	   * @brief You need to implement here a function that processes the
	   * analysis buffers in order to apply the digital processing algorithm
	   * to be implemented (core function)
	   */
	virtual void processParametrisation() = 0;

	/**
	   * @brief getAnalysisLength
	   * @return the length of the analysis buffer (2^order + 2)
	   * The length is 2 samples longer thant the frame length, because analysis
	   * consists of complex numbers representing the FFT of a real singal, and in
	   * a compact form it needs to more values.
	   */
	virtual int getAnalysisLength() const;
};


/**
	 * @brief The SubBandSTFTAnalysis class
	 */
class SubBandSTFTAnalysis : public STFTAnalysis
{

    public:
	typedef enum {MEL, LINEAR} FilterBankType;
	SubBandSTFTAnalysis(int nbins, int sampleRate, int order, int channels, float minFreq, float maxFreq, FilterBankType type=MEL);

    protected:
	const int _numberOfBins;
	const int _sampleRate;
	boost::scoped_ptr<FilterBank> _filterBank;
	boost::scoped_array<BaseTypeC> _filterCoeficients;
	int _coeficientsLength;
	SignalVector _processedAnalysisFrames;
	SignalVector _subbandAnalysisFrames;


	virtual void processParametrisation();
	virtual void processSetup() = 0;
	virtual void processOneSubband(const SignalVector &analysisFrame, int length, int bin) = 0;
	virtual void processSumamry() = 0;

	//	void debugPlotFilterBank();

};


/**
	 * @brief The DummySTFT class
	 * A SFTF algorithm that does nothing, simply does STFT and its invers,
	 * the resulting signal has to be equal to the input one.
	 */
class DummySTFT : public STFT
{
    public:
	DummySTFT();
    private:
	void processParametrisation();
};

}


#endif // __STFT_H_
