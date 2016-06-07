/*
* FilterBankFFTW.h
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
#ifndef __FILTERBANKFFTW_H_
#define __FILTERBANKFFTW_H_

#include <dspone/filter/FilterBank.h>

namespace dsp {

/**
	 * @brief This class implements a filter bank using
	 * a set of spectral weigthing band-pass filters.
	 * The band pass filters are constructed using
	 * overlaped triangular windows in the frequency domain.
	 */
class FilterBankFFTW : public FilterBank
{
    public:
	/**
	   * @brief Constructor
	   * @param order  order of each band-pass filter.
	   * The FFT used in the filters will be of 2^order samples
	   * @param centerFreqs  list of frequencies at which each band-pass filter
	   * will be centered.
	   */
	FilterBankFFTW(int order, std::vector<double> centerFreqs);
	/**
	   * @brief Copy constructor
	   * @param fbank  filter bank to compy from.
	   */
	FilterBankFFTW(const FilterBankFFTW &fbank);

	virtual void filterBuffer(double *inbuffer, double *residual, double *outbuffer, int inlength, int outlength);
	virtual void filterBuffer(float *inbuffer, float *residual, float *outbuffer, int inlength, int outlength);
	virtual void filterBuffer(int16_t *inbuffer, int16_t *residual, int16_t *outbuffer, int inlength, int outlength);

    protected:

	/**
	   * @brief Constructs and intialises all the bans-pass filters.
	   * As many as centre frequencies have been supplied.
	   */
	void initialiseFilters();

	/**
	   * @brief Empty constructor that can be used by derived classes.
	   * So that they can perform a different initialisation of the
	   * filters (For example the Mel-Scaled filter banks)
	   */
	FilterBankFFTW();
	template <class T> void filterBufferCoreEfficient(T *inbuffer, T *residual, T *outbuffer, int inlength, int outlength);

	int _fftOrder;



};

}


#endif // __FILTERBANKFFTW_H_
