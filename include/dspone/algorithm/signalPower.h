/*
* dspHelpers.h
* Copyright 2016 (c) Jordi Adell
* Created on: 2016
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

#ifndef __DSP_SIGNAL_POWER_H_
#define __DSP_SIGNAL_POWER_H_

#include <vector>
#include <boost/shared_array.hpp>

namespace dsp
{

class SignalPower
{
    private:
	SignalPower();
	~SignalPower();

    public:

	static double power(const double* signal, int length);
	static double logPower(const double *signal, int length);
	static double power(const int16_t* signal, int length);
	static double logPower(const int16_t* signal, int length);

	static double power(const boost::shared_array<double> &signal, int length);
	static double logPower(const boost::shared_array<double> &signal, int length);

	static double power(const std::vector<double*> signal, int length);
	static double logPower(const std::vector<double*> signal, int length);
	static double power(const std::vector<int16_t*> signal, int length);
	static double logPower(const std::vector<int16_t*> signal, int length);

	static double power(const std::vector<boost::shared_array<double> > &signal, int length);
	static double logPower(const std::vector<boost::shared_array<double> > &signal, int length);

	static double FFTPower(const std::vector<double*> &fft, int fft_length);
	static double FFTLogPower(const std::vector<double*> &fft, int fft_length);
	static double FFTPower(const std::vector<boost::shared_array<double> > &fft, int fft_length);
	static double FFTLogPower(const std::vector<boost::shared_array<double> > &fft, int fft_length);

};

}

#endif
