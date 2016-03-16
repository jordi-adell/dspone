/*
* fftImpl.h
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
* along with WIPP.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __FFTIMPL_H_
#define __FFTIMPL_H_

#include <dspone/dspdefs.h>
#include <wipp/wippfft.h>

namespace dsp {

/**
	  * @brief 1D Fast Fourier Transform
	 */
class FFTImpl
{

  private:
    typedef wipp::wipp_fft_t FFTspecType;

    SignalPtr _frame;

  public:
    FFTImpl(int order);
    ~FFTImpl();

    inline int getOrder() const {return _fftOrder;}
    inline int getFFTLength() const {return _fftLength;}
    inline int getOneSidedFFTLength() const {return _oneSidedFFTLength;}
    inline int getAnalysisWindowLength() const {return getAnalysisWindowLength(_fftOrder);}

    /**
	   * @brief getFFTLength   Gets you the length of an FFT given the order
	   * @param order  FFT order
	   * @return   the samples of the FFT
	   */
    static inline int getFFTLength(int order) {return ((1 << order) + 2);}
    /**
	   * @brief getAnalysisWindowLength   Returns the maximum length of the
	   * input signal given the FFT order.
	   * @param order  FFT order
	   * @return   the length of the equivalent square analysis window.
	   */
    static inline int getAnalysisWindowLength(int order) {return (1 << order);}

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

    /**
	    * @brief Auxiliary variable for IPP computing.
	    */
    FFTspecType *_fftspec;
    //    boost::scoped_array<Ipp8u> _fftInternalBuffer;

    /**
	    * @brief _fftOrder order of the FFT (i.e., order=10 --> fft length=1024)
	    */
    const int _fftOrder;

    /**
	    * @brief _fftLength  length of the FFT.
	    * I alsways equal to (1 << _fftOrder)
	    */
    const int _fftLength;

    /**
	    * @brief _onseSidedFFTLength  length of the equivalent FFT for one-sided transform
	    * For the case of real signals, the FFT is simetric, thus only half of it is enough.
	    */
    const int _oneSidedFFTLength;

    /**
	    * @brief Initialised the variable necessary for FFT computation by IPP functions
	    */
    void init();
};

}



#endif 
