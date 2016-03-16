/*
* FilterBankFIR.h
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
#ifndef __FILTERBANKFIR_H_
#define __FILTERBANKFIR_H_

#include <dspone/FilterBank.h>


namespace dsp {

class FilterBankFIR : public FilterBank
{
    public:
	FilterBankFIR(int order, std::vector<double> centerFreqs);
	/**
	   * @brief Copy constructor
	   * @param fbank  filter bank to compy from.
	   */
	FilterBankFIR(const FilterBankFIR &fbank);

    protected:
	FilterBankFIR();
	void initialiseFilters();



};

}

#endif // __FILTERBANKFIR_H_
