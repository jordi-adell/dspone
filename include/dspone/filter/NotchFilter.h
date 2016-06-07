/*
* NotchFilter.h
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
#ifndef __NOTCHFILTER_H_
#define __NOTCHFILTER_H_

#include <dspone/filter/Filter.h>

#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>

#include <stdint.h>
#include <string>


namespace dsp {

class IIRFilter;

class NotchFilter : public Filter
{

    public:
	NotchFilter(double freq, double quality);
	~NotchFilter();

	/**
   * @brief filter   Filter the provided buffer with a pre-emphasis filter
   * @param signal   The signal to be filtered.
   * @param length   The length of the signal vector.
   */
	void filter(int16_t *signal, int length);
	/**
   * @brief filter   Same as the previous function but puts the filtered
   * signal in a different vector.
   * @param insignal   input signal
   * @param outsignal  buffer to store the filtered signal
   * @param length  length of both buffers.
   */
	virtual void filterBuffer(const int16_t *insignal, int16_t *outsignal, int length);
	virtual void filterBuffer(const double *insignal, double *outsignal, int length);
	virtual void filterBuffer(const float *insignal, float *outsignal, int length);

    private:

	const unsigned int _order;
	boost::scoped_ptr<IIRFilter> _filterImpl;
	boost::scoped_array<double> _coefs;

};
}


#endif // __NOTCHFILTER_H_
