/*
* FilterBankFIR.cpp
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
* alogn with DSPONE.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <dspone/filter/FilterBankFIR.h>
#include <dspone/filter/BandPassFIRFilter.h>
#include <dspone/DspException.h>

namespace dsp {

FilterBankFIR::FilterBankFIR()
{

}

FilterBankFIR::FilterBankFIR(int order, std::vector<double> centerFreqs) :
  FilterBank(order, centerFreqs)
{
  initialiseFilters();
}

FilterBankFIR::FilterBankFIR(const FilterBankFIR &fbank) : FilterBank(fbank)
{

}

void FilterBankFIR::initialiseFilters()
{
  if (_centerFreqs.empty())
    return;

  std::vector<double>::iterator it = _centerFreqs.begin();
  double initialFreq = 0;
  double centerFreq = *it;
  double endFreq;

  for (++it; it != _centerFreqs.end(); ++it)
  {
    endFreq = *it;
    if (endFreq <= centerFreq)
    {
      std::string msg("Center frequencies need to be monotonically ascending: ");
      msg +=  std::to_string(centerFreq) + " --> " + std::to_string(endFreq);
      throw(DspException(msg));
    }
    _filterBank.push_back(PtrFilter());
    if (endFreq == 0.5)
      endFreq = 0.499;
    if (initialFreq == 0)
      initialFreq =+ 0.05*endFreq;

    _filterBank.back().reset(new BandPassFIRFilter(_order, initialFreq, endFreq, BandPassFIRFilter::TRIANGULAR));
    initialFreq = centerFreq;
    centerFreq = endFreq;
  }

  endFreq = 2*centerFreq - initialFreq;
  if (endFreq > 0.5)
    endFreq = 0.499;
  _filterBank.push_back(PtrFilter());
  _filterBank.back().reset(new BandPassFIRFilter(_order, initialFreq, endFreq, BandPassFIRFilter::TRIANGULAR));

}

}

