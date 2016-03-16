/*
* BandPassFFTWFilterImpl.h
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
#ifndef __BANDPASSFFTWFILTERIMPL_H__
#define __BANDPASSFFTWFILTERIMPL_H__


#include <dspone/BandPassFilter.h>
#include <dspone/dspdefs.h>
#include <boost/scoped_ptr.hpp>

namespace dsp {

class FFTWeightingFilter;

/** @brief implementation of a band pass filter using the spectrum weighting technique.
	 * By default uses a triangular shape with 1 as maxium gain. If the central frequency
	 * is supplied the triangle is assymetric. I filters buffers of 2^order samples.
	 * This is due to the FFT algorithm restrictions.
	 **/
class BandPassFFTWFilterImpl : public BandPassFilter
{
    public:
	/**
	   * @brief BandPassFFTWFilterImpl Contructs the object from the cut off frequencies
	   * @param order  order of the filter. The FFT used to filter witll contain 2^order samples
	   * @param lowFreq  low cut off frequency
	   * @param highFreq  high edge of the filter.
	   * @param centerFreq  central frequency (maximum gain applied here)
	   */
	BandPassFFTWFilterImpl(const int &order, const double &lowFreq, const double &highFreq);
	BandPassFFTWFilterImpl(const int &order, const double &lowFreq, const double &highFreq, const double &centerFreq);
	virtual ~BandPassFFTWFilterImpl();

	/**
	   * @brief filterBuffer filters a buffer of 2^order samples long, thre different sample format can be supplied.
	   * @param inbuffer  input buffer were the signal is stored.
	   * @param outbuffer  output buffer to store the filtered signal.
	   * @param length  length of both input and output signals.
	   */
	virtual void filterBuffer(const int16_t *inbuffer, int16_t *outbuffer, int length);
	virtual void filterBuffer(const double *inbuffer, double *outbuffer, int length);
	virtual void filterBuffer(const float  *inbuffer, float  *outbuffer, int length);

	void spectralWeighting(double *spectrum, int length) const;
	void fft(const double *signal, int signallength, double *spectrum, int speclength) const;
	void ifft(const double *spectrum, int speclength, double *signal, int signallength) const;

	/** @brief getLowFreq
	   * @return the low edge frequency
	   **/
	virtual inline double getLowFreq()  const {return _lowFreq;}
	/**
	   * @brief getHighFreq
	   * @return the high cut off frequency
	   */
	virtual inline double getHighFreq() const {return _highFreq;}
	/**
	   * @brief getCenterFreq
	   * @return returns the central digital frequency
	   */
	virtual inline double getCenterFreq() const {return _centerFreq;}

	/**
	   * @brief getCoeficients This function fils the coefs array with the length first coefficients of the filter.
	   * They function assumes the necessary memory has been already allocated.
	   * @param coefs  array to wehere coeficients are stored
	   * @param length  number of coeficients stored
	   */
	virtual void getCoeficients(double *coefs, int length) const;

	virtual int getFrameLength() const;

    private:

	double _lowFreq;
	double _highFreq;
	double _centerFreq;
	int _order;  // (log_2(length))
	/** length is always a power of 2, because length = 2^order. This is a FFT restriction **/
	int _length;

	int _coefsLength;
	BaseTypePtr _coefs;

	/** Actual implementation of the filter **/
	std::unique_ptr<FFTWeightingFilter> _fftwFilter;


	/**
	   * @brief initialiseFilter  Initialised the actual filter (_fftwFilter)
	   * @param order  the length of the filter will be 2^order
	   * @param lowFreq  low edge
	   * @param highFreq  high edge
	   * @param centerFreq  central frequency
	   */
	virtual void initialiseFilter(const int &order, const double &lowFreq, const double &highFreq, const double &centerFreq);
	/**
	   * @brief initialiseFilterCore Initialised the IPP core filter with member values
	   */
	void initialiseFilterCore();

	/**
	   * @brief setFilterShape  Sets the coefs of the filter to the desired filter shape.
	   * You can make a derived class and overload this method, if you want to have a filter with a different shape.
	   * @param coefs  buffer of coeficients that will be used to multiply the FFT for filtering.
	   * @param length  number of coeficients (needs to be 2^order = _length)
	   * @param startId   Shape start coeficient corresponding to _lowFreq
	   * @param endId  Shape end coeficient correspoinding to _highFreq
	   * @param centerId  Shape cnetral coeficient corresponding to _centerFreq
	   */
	virtual void setFilterShape(BaseType *coefs, const int &length, const int &startId, const int &endId, const int &centerId) const;

	/**
	   * @brief setTriangularFilterShape This function is call by the virtual function setFilterShape
	   * and sets the actual shape of the filter to a triangle.
	   * @param coefs  buffer of coeficients that will be used to multiply the FFT for filtering.
	   * @param length  number of coeficients (needs to be 2^order = _length)
	   * @param startId   Shape start coeficient corresponding to _lowFreq
	   * @param endId  Shape end coeficient correspoinding to _highFreq
	   * @param centerId  Shape cnetral coeficient corresponding to _centerFreq
	   */
	void setTriangularFilterShape(BaseType *coefs, const int &length, const int &startId, const int &endId) const;
	void setTriangularFilterShape(BaseType *coefs, const int &length, const int &startId, const int &endId, const int &centerId) const;

	/** @brief This function performes the actual filter process and is called by the three public
	   * filterBuyffer(...) functions.
	   * @param inbuffer  The signal buffer to be filtered
	   * @param outbuffer  The outputbuffer where filtered signal will be placed
	   * @param length  the size of the input/output buffers
	   **/
	template <class T> void filterBufferCore(const T *inbuffer, T *outbuffer, int length);


	/**
	   * @brief This function checks whether there has been an error executing an IPP fucntion
	   * and throws an exception if this was the case
	   * @param status  Variable containing the status variable usually returned by IPP functions.
	   */
	void checkIPPStatus(int status) const; // Throws if IPP error found.

};


}


#endif // __BANDPASSFFTWFILTER_H_
