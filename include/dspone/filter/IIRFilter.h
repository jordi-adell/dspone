/*
* IIRFilter.h
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
#ifndef __IIRFILTER_H_
#define __IIRFILTER_H_

#include <dspone/filter/Filter.h>

#include <string>

namespace dsp {

class IIRFilter_t_;

/**
	 * @brief The IIRFilter class
	 *
	 * An IIR filter (Infinite Impulse Response) implementatino based on IPP
	 * This filters do contained recursive operations. Be carefull on
	 * designing the coefficients of the filter, because they can be unstable.
	 *
	 *        Y(Z)    sum_i{ a_i*z^(-i) }
	 * H(Z) = ---- = ----------------------
	 *        X(Z)    sum_j{ b_i*z^(-j) }
	 *
	 * H(Z) is the transfer function of the filter.
	 *
	 * These filters can also be expressed in term of FDE (Finite differential equations)
	 *
	 * y[n] = sum_j { a_i*x[n-j] } - sum_i{ b_i*y[n-i] }
	 *
	 * The coefficients vector has to be expressed in the following for:
	 * a_coefs[] = {a_0, a_1, a_2, ..., a_M}
	 * b_coefs[] = {b_0, b_1, b_2, .. b_N};
	 *
	 */

class IIRFilter : public Filter
{
    public:
	/**
	   * @brief IIRFilter
	   * @param coefs   coefficients of the filter: coefs[] = {a_0, a_1, a_2, ..., a_M, b_0, b_1, b_2, .. b_N};
	   * @param length  number of coefficients
	   */
	IIRFilter(const double *coefs, int length);
	~IIRFilter();

	/**
	   * @brief filter  filters the input signal an replaces the values in the fiven array.
	   * @param signal  input/output array of signal values.
	   * @param length  length of the input array
	   */
	void filter(int16_t *signal, int length);
	void filter(float *signal, int length);
	void filter(double *signal, int length);

	/**
	   * @brief filter     filters the input signals and places the result in the output signal array.
	   * @param insignal   input  array of signal values.
	   * @param outsignal  output array of signal values.
	   * @param length     number of values in input signal and output signal.
	   */
	virtual void filterBuffer(const int16_t *insignal, int16_t *outsignal, int length);
	virtual void filterBuffer(const double *insignal, double *outsignal, int length);
	virtual void filterBuffer(const float *insignal, float *outsignal, int length);

	virtual int getFrameLength() const {return -1;}

    private:

	unsigned int _order; /**< order of the peremphasis filter */
	IIRFilter_t_ *_iir_filter;
};

}



#endif // IIRFILTER_H_
