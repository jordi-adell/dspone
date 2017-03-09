/*
* FilterBank.h
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
#ifndef __FILTERBANK_H_
#define __FILTERBANK_H_

#include <dspone/filter/BandPassFilter.h>

#include <vector>
#include <memory>

namespace dsp {

/**
	 * @brief The FilterBank class
	 * This class can be used to filter signal by a pool of several filters (a filter bank)
	 * You create the filter bank by setting the order of each filter and a vector with
	 * all the frequencies at which a filter has to be places. The bans pass of each filter
	 * spans from the previous frequency in the vector to the following one.
	 */
class FilterBank
{
    public:
	/** @brief constructor
	   * Creates a filter bank of the specified order (this means it used FFT of size 2^order)
	   * @param order   order of each of the filters conained in the filter bank
	   * @param centerFreqs  frequencies at which a filter will be placed
	   **/
	FilterBank(int order, const std::vector<double> &centerFreqs);
	FilterBank(const FilterBank &fbank);
	virtual ~FilterBank() {}

	/**
	   * @brief filterBuffer This function can be used to filter a buffer using the filterbank.
	   * The input signal is filtered sequencially by each of the filters.
	   * @param inbuffer  input signal. Must be 2^order long.
	   * @param residual  signal residual
	   * @param outbuffer  output buffer, the signal resulting from filtering the signal accross each filter,
	   * are placed in this buffer. The size of this buffer needs to be equal to the length of sinal multiplied
	   * by the number of filters.
	   * @param inlength  length of the input signal and the residual
	   * @param outlength  length of the output buffer (inlength*getNBins()))
	   */
	virtual void filterBuffer(int16_t *inbuffer, int16_t *outbuffer, int inlength, int outlength);
	virtual void filterBuffer(double *inbuffer,  double *outbuffer,  int inlength, int outlength);
	virtual void filterBuffer(float  *inbuffer,  float  *outbuffer,  int inlength, int outlength);

	/**
	   * @brief getNBins
	   * @return   The number of filters
	   */
	int getNBins() const {return _filterBank.size();}

	/**
	   * @brief getCenterFrequencies
	   * @param freqs  the sets this vector to the digital center frequencies of all the filters
	   **/
	virtual void getCenterFrequencies(std::vector<double> &freqs) const {freqs = _centerFreqs;}

	/**
	   * @brief Returns the digital frequency of the frequency bin
	   * @param bin  bit for which the center frequency is supplied.
	   * @return the digital frequency
	   */
	virtual double getBinCenterFrequency(int bin) const {return _centerFreqs.at(bin);}

	/**
	   * @brief getFilters  fills the coefs buffer with the length
	   * first frequency coefficients.
	   * @param coefs  coefficients buffer
	   * @param length  number of coefficients to fill.
	   * @return  the number of coeficients actually written into coefs
	   */
	int getFiltersCoeficients(double *coefs, int length) const;


    protected:
	FilterBank();

	typedef std::shared_ptr<BandPassFilter> PtrFilter;
	typedef std::vector<PtrFilter> FilterVector;
	/**
	   * @brief Vector with the digital center frequencies
	   */
	std::vector<double> _centerFreqs; // Digital frequencies

	/**
	   * @brief _order number of coeficients of the each filter.
	   * (Ex: number of taps in a FIR filter, or number of spectral weights in the FFTW)
	   */
	int _order;

	/**
	   * @brief Vector of band pass filters used to implement the filter bank
	   */
	FilterVector _filterBank;

    private:

	/**
	   * @brief This fucntion performs the actual filtering process.
	   * It is called by the public filterBuffer(...) functions.
	   * @param inbuffer  input signal
	   * @param outbuffer  output signal (is assumed to be already allocated)
	   * @param length  length of both buffers
	   */
	template <class T>  void filterBufferCore(T *inbuffer, T *outbuffer, int inlength, int outlength);
};


}

#endif // __FILTERBANK_H_
