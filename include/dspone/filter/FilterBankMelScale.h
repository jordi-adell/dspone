/*
* FilterBankMelScale.h
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
#ifndef __FILTERBANKMELSCALE_H_
#define __FILTERBANKMELSCALE_H_

#include <dspone/filter/FilterBankFIR.h>
#include <dspone/filter/FilterBankFFTW.h>

namespace dsp {

/**
	 * @brief This class implements a filter bank
	 * using the spectral weighting technique.
	 * However, band-pass filters are equally spaced in the mel-scale.
	 * So, in the linear scale filters are close to ecah other for low
	 * frequencies and more separated for high frequencies.
	 *
	 *     -   -                       -       -
	 *    - - - -                    -   -   -   -
	 *   -   -   -                 -       -       -
	 *  -   - -   -              -        -  -       -
	 * |---|---|---|-----------|-------|------|--------| ....  --| ->  w
	 * 0                                                         1
	 */
class FilterBankMelScale
{
    public:
	FilterBankMelScale();
    protected:

	/**
	   * @brief
	   * @param nBins
	   * @param sampleRate
	   * @param minFreq
	   * @param maxFreq
	   */
	void setCenterFreqs(int nBins, int sampleRate, float minFreq, float maxFreq, std::vector<double> &centerFreqs);
};

class FilterBankFIRMelScale : public FilterBankFIR, FilterBankMelScale
{
    public:
	/**
	   * @brief constructor
	   * @param order  Order of the FFT used in the band-pass filters
	   * FFT will be computed with 2^order samples.
	   * FIR filter witll be 2^order length.
	   * @param nBins   Number of band-pass filters used
	   * @param sampleRate   sample rate of acquisition of the digital signal
	   * @param minFreq   Low cut-off frequency of the first filter
	   * @param maxFreq   Hight cut-off frequency of the last filter.
	   */
	FilterBankFIRMelScale(int order, int nBins, int sampleRate, float minFreq, float maxFreq)
	{
	    _order = (1 <<  order);
	    setCenterFreqs(nBins, sampleRate, minFreq, maxFreq, _centerFreqs);
	    initialiseFilters();
	}

	/**
	   * @brief constructor
	   * @param order  Order of the FFT used in the band-pass filters
	   * Calls the previous constructor with minFreq = 0 and maxFreq = sampleRate/2
	   * @param nBins   Number of band-pass filters used
	   * @param sampleRate   sample rate of acquisition of the digital signal
.           */
	FilterBankFIRMelScale(int order, int nBins, int sampleRate)
	{
	    _order = (1 << order);
	    setCenterFreqs(nBins, sampleRate, 0, sampleRate/2, _centerFreqs);
	    initialiseFilters();
	}
};


class FilterBankFFTWMelScale : public FilterBankFFTW, FilterBankMelScale
{
    public:
	/**
	   * @brief constructor
	   * @param order  Order of the FFT used in the band-pass filters
	   * FFT will be computed with 2^order samples.
	   * FIR filter witll be 2^order length.
	   * @param nBins   Number of band-pass filters used
	   * @param sampleRate   sample rate of acquisition of the digital signal
	   * @param minFreq   Low cut-off frequency of the first filter
	   * @param maxFreq   Hight cut-off frequency of the last filter.
	   */
	FilterBankFFTWMelScale(int order, int nBins, int sampleRate, float minFreq, float maxFreq)
	{
	    _order = (1 << (order-1)) + 1;
	    _fftOrder = order;
	    setCenterFreqs(nBins, sampleRate, minFreq, maxFreq, _centerFreqs);
	    initialiseFilters();
	}

	/**
	   * @brief constructor
	   * @param order  Order of the FFT used in the band-pass filters
	   * Calls the previous constructor with minFreq = 0 and maxFreq = sampleRate/2
	   * @param nBins   Number of band-pass filters used
	   * @param sampleRate   sample rate of acquisition of the digital signal
.           */
	FilterBankFFTWMelScale(int order, int nBins, int sampleRate)
	{
	    _order = (1 << (order-1)) + 1;
	    _fftOrder = order;
	    setCenterFreqs(nBins, sampleRate, 0, sampleRate/2, _centerFreqs);
	    initialiseFilters();
	}
};



}


#endif // __FILTERBANKMELSCALE_H_
