/*
* FilterBankMelScale.cpp
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
#include <dspone/filter/FilterBankMelScale.h>
#include <dspone/dspdefs.h>

#include <wipp/wipputils.h>
#include <wipp/wippsignal.h>

#include <math.h>

namespace dsp {

FilterBankMelScale::FilterBankMelScale()
{

}


void FilterBankMelScale::setCenterFreqs(int nBins, int sampleRate, float minFreq, float maxFreq, std::vector<double> &centerFreqs)
{

  //
  // Parameters are taken from the HTK book.
  // note that in the web they use log10 and IPP used Ln.
  // http://www.ee.columbia.edu/ln/rosa/doc/HTKBook21/node54.html
  //
  //                                 linear_freq
  //  mel_freq = 2595 * log_10( 1 + ------------- )
  //                                     700
  //

  BaseType32 melFreqs[nBins];
  BaseType32 linearFreqs[nBins];

  BaseType32 linearStart = (minFreq < sampleRate/2) ? minFreq : sampleRate/2;
  BaseType32 linearEnd   = (maxFreq < sampleRate/2) ? maxFreq : sampleRate/2;

  BaseType32 melStart = 0;
  BaseType32 melEnd = 0;

  BaseType32 logFactor = 10;
  wipp::logn(&logFactor, 1);
  BaseType32 melMul = 2595/logFactor;
  BaseType32 melDiv = 700;

  wipp::linear2mel(&linearStart, &melStart, 1, &melMul, &melDiv);
  wipp::linear2mel(&linearEnd, &melEnd, 1, &melMul, &melDiv);

  BaseType32 melDelta = (melEnd - melStart)/(nBins-1);

  for (int i=0; i<nBins; ++i)
    melFreqs[i] = i*melDelta + melStart;

  wipp::mel2linear(melFreqs, linearFreqs, nBins, &melMul, &melDiv);

  // From now on, linearFreqs are in linear scale and are digital freqs.
  // DivC - divide vector by a constant.
  wipp::divC(sampleRate, linearFreqs, nBins);


  for (int i=0; i<nBins; ++i)
    centerFreqs.push_back(linearFreqs[i]);

}


}
