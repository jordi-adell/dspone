/*
* BandPassFilter.h
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
#ifndef __BANDPASSFILTER_H_
#define __BANDPASSFILTER_H_

#include <dspone/filter/Filter.h>

namespace dsp {

  /**
   * @brief The BandPassFilter class
   * This is an abstract class that sets general specifications for a band-pass filter.
   * Derived classes may implement the band-pass filter, using several techniques,
   * such as espectral weighting or FIR filters.
   */
  class BandPassFilter : public Filter
  {

  public:
    /**
     * @brief BandPassFilter Does nothing, actually
     */
    BandPassFilter();
    /**
     * @brief BandPassFilter  Does nothing
     * @param order  Order of the filter, order might be used for example to determine the length of the
     * FFT calcuylations, FFT_length = 2^order. Or for FIR imeplementations be the number of taps used.
     * @param lowFreq  Low cut-off frequency of the band-pass filter.
     * @param highFreq  High cut-off frequency of the band-pass filter.
     */
    BandPassFilter(const int &order, const double &lowFreq, const double &highFreq);

    virtual ~BandPassFilter();

    /** @return the low most passing frequency
     * This fucntion is not implementied here, because the calculation
     * of the real value of this frequency depens on the actual implementation
     * of the filter
     **/
    virtual double getLowFreq() const = 0;

    /** @return the low most passing frequency
     * This fucntion is not implementied here, because the calculation
     * of the real value of this frequency depens on the actual implementation
     * of the filter
     **/
    virtual double getHighFreq() const = 0;

    /**
     * @brief getCoeficients This function fils the coefs array with the length first coefficients of the filter.
     * They function assumes the necessary memory has been already allocated.
     * @param coefs  array to wehere coeficients are stored
     * @param length  number of coeficients stored
     */
    virtual void getCoeficients(double *coefs, int length) const = 0;
  private:
  };

}

#endif // __BANDPASSFILTER_H_
