/*
* gralCrossCorrelation.cpp
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
#include <dspone/algorithm/gralCrossCorrelation.h>
#include <dspone/algorithm/gralCrossCorrelationImpl.h>


namespace dsp{

GeneralisedCrossCorrelation::GeneralisedCrossCorrelation(int length, FFTtype usedFFTtype)
{
  _impl.reset(new GeneralisedCrossCorrelationImpl(length, usedFFTtype));
}


BaseTypeC GeneralisedCrossCorrelation::calculateCorrelation(const  BaseTypeC *x, const  BaseTypeC *y,
							    int length, BaseType tau, FFTtype usedFFTtype)
{
  _impl->calculateCorrelation(x, y, length, tau, usedFFTtype);
}

void GeneralisedCrossCorrelation::calculateCorrelationsForTauVector(const BaseTypeC *x, const BaseTypeC *y, BaseTypeC *c, int length,
								    BaseType *samplesDelay, int numSteps, FFTtype usedFFTtype)
{
  _impl->calculateCorrelationsForTauVector(x, y, c, length, samplesDelay, numSteps, usedFFTtype);
}

void GeneralisedCrossCorrelation::precomputeTauMatrix(BaseType *samplesDelay, int numSteps, int length, FFTtype usedFFTtype)
{
  _impl->precomputeTauMatrix(samplesDelay, numSteps, length, usedFFTtype);
}

void GeneralisedCrossCorrelation::calculateCorrelationsForPrecomputedTauMatrix(const BaseTypeC *x, const BaseTypeC *y, BaseTypeC *c,
									       int length, int numSteps, FFTtype usedFFTtype)
{
  _impl->calculateCorrelationsForPrecomputedTauMatrix(x, y, c, length, numSteps, usedFFTtype);
}

}
