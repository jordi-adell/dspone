/*
* PreEmphasisFilter.h
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
#ifndef PREEMPHASISFILTER_H_
#define PREEMPHASISFILTER_H_

#include <dspone/filter/Filter.h>

#include <memory>

namespace dsp {

class IIRFilter;

/**
       * @brief The PreEmphasisFilter class
       * This can be used as a premphasis filter to pre-process speech
       * samples. This is a common action to use a IIR filter to do so.
       * The IIR filter used here is H(z)=1 -  a*z^(-1)
       * I can be expressed also as: y[n] = x[n] - a * y[n-1]
       * The value of a i the factor passes to the constructor
       */
class PreEmphasisFilter : public Filter
{
    public:
	/**
	 * @brief PreEmphasisFilter  creates a PreEmphasis filter with
	 * the given factgor.
	 * @param factor 'a' value in the formula above.
	 */
	PreEmphasisFilter(double factor);
	~PreEmphasisFilter();

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
	virtual void filterBuffer(const double *inbuffer, double *outbuffer, int length);
	virtual void filterBuffer(const float *inbuffer, float *outbuffer, int length);

	virtual int getFrameLength() const;

    private:

	std::unique_ptr<IIRFilter> _filterImpl; /**< This is the filter implementation Currently using a IIR **/
};

}


#endif // PREEMPHASISFILTER_H_
