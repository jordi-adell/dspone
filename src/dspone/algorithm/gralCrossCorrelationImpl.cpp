/*
* gralCrossCorrelationImpl.cpp
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
#include <dspone/algorithm/gralCrossCorrelationImpl.h>
#include <dspone/DspException.h>
#include <dspone/dspdefs.h>
#include <dspone/dsplogger.h>

#include <wipp/wipputils.h>
#include <wipp/wippsignal.h>
#include <wipp/wippstats.h>

#include <math.h>



namespace dsp{

GeneralisedCrossCorrelationImpl::GeneralisedCrossCorrelationImpl(int length, FFTtype usedFFTtype) :
  _usedFFTtype(usedFFTtype),
  _delayMatrixIsPrecomputed(false),
  _precomputedNumSteps(0)
{
  // _length is equal to the length of the full FFT in all cases.
  switch (usedFFTtype)
  {
    case GeneralisedCrossCorrelation::ONESIDEDFFT:
      _length = 2*(length-1);
    break;
    case GeneralisedCrossCorrelation::TWOSIDEDFFT:
      _length = length;
    break;
    default:
      throw(DspException("Incorrect FFT type."));
  }

  allocate();
  //          _plot.reset(new Gnuplot("lines"));
}

GeneralisedCrossCorrelationImpl::~GeneralisedCrossCorrelationImpl()
{

}


void GeneralisedCrossCorrelationImpl::allocate()
{
  // Allocate vectors that will be used in the algorithm.
  // The length of all of them is equal to the length of the full FFT.
  _weight.reset(new BaseTypeC[_length]);
  _phase.reset(new BaseType[_length]);
  _phat.reset(new BaseType[_length]);
  _magnitude.reset(new BaseType[_length]);
  _ones.reset(new BaseType[_length]);
  _delay.reset(new BaseTypeC[_length]);
  _correlation.reset(new BaseTypeC[_length]);
  _generalisedcc.reset(new BaseTypeC[_length]);

  wipp::set(1, _ones.get(), _length);

  // These vectors are only used for one-sided FFT.
  if (_usedFFTtype == GeneralisedCrossCorrelation::ONESIDEDFFT)
  {
    _fullx.reset(new BaseTypeC[_length]);
    _fully.reset(new BaseTypeC[_length]);
  }
}

BaseTypeC GeneralisedCrossCorrelationImpl::calculateCorrelation(const  BaseTypeC *x, const  BaseTypeC *y,
							    int length, BaseType tau, FFTtype usedFFTtype)
{
  calculateCorrelationsForTauVector(x, y, &_corrValue, length, &tau, 1, usedFFTtype);
  return _corrValue;
}

void GeneralisedCrossCorrelationImpl::calculateCorrelationsForTauVector(const BaseTypeC *x, const BaseTypeC *y, BaseTypeC *c, int length,
								    BaseType *samplesDelay, int numSteps, FFTtype usedFFTtype)
{
  if (usedFFTtype != _usedFFTtype)
  {
    throw(DspException("Wrong FFT type"));
  }

  if (usedFFTtype == GeneralisedCrossCorrelation::ONESIDEDFFT)
  {
    if (length != (_length/2+1))
    {
      throw(DspException("Wrong vector length"));
    }
    generateTwoSidedFFT(x, y, _fullx.get(), _fully.get(), length);
    calculateCorrelationsCore(_fullx.get(), _fully.get(), c, samplesDelay, numSteps, false);
  }
  else if (usedFFTtype == GeneralisedCrossCorrelation::TWOSIDEDFFT)
  {
    if (length != _length)
    {
      throw(DspException("Wrong vector length"));
    }
    calculateCorrelationsCore(x, y, c, samplesDelay, numSteps, false);
  }
}

void GeneralisedCrossCorrelationImpl::precomputeTauMatrix(BaseType *samplesDelay, int numSteps, int length, FFTtype usedFFTtype)
{
  if (usedFFTtype != _usedFFTtype)
  {
    throw(DspException("Wrong FFT type"));
  }

  if ( ((usedFFTtype == GeneralisedCrossCorrelation::ONESIDEDFFT) && (length != (_length/2+1))) || ((usedFFTtype == GeneralisedCrossCorrelation::TWOSIDEDFFT) && length != _length) )
  {
    throw(DspException("Wrong vector length"));
  }

  for(int i = 0; i < numSteps; i++)
  {
    _delayMatrix.push_back(boost::shared_array<BaseTypeC>(new BaseTypeC[_length]));
    computePhaseRamp(samplesDelay[i]);
    wipp::copyBuffer(reinterpret_cast<wipp::wipp_complex_t*>(_delay.get()),
		     reinterpret_cast<wipp::wipp_complex_t*>(_delayMatrix[i].get()), _length);
  }

  _delayMatrixIsPrecomputed = true;
  _precomputedNumSteps = numSteps;
}

void GeneralisedCrossCorrelationImpl::calculateCorrelationsForPrecomputedTauMatrix(const BaseTypeC *x, const BaseTypeC *y, BaseTypeC *c,
									       int length, int numSteps, FFTtype usedFFTtype)
{
  if (usedFFTtype != _usedFFTtype)
  {
    throw(DspException("Wrong FFT type"));
  }

  if(_delayMatrixIsPrecomputed)
  {
    if (numSteps != _precomputedNumSteps)
    {
      throw(DspException("Wrong numSteps"));
    }

    if (usedFFTtype == GeneralisedCrossCorrelation::ONESIDEDFFT)
    {
      if (length != (_length/2+1))
      {
	throw(DspException("Wrong vector length"));
      }
      generateTwoSidedFFT(x, y, _fullx.get(), _fully.get(), length);
      calculateCorrelationsCore(_fullx.get(), _fully.get(), c, NULL, numSteps, true);
    }
    else if (usedFFTtype == GeneralisedCrossCorrelation::TWOSIDEDFFT)
    {
      if (length != _length)
      {
	throw(DspException("Wrong vector length"));
      }
      calculateCorrelationsCore(x, y, c, NULL, numSteps, true);
    }
  }
  else
  {
    wipp::setZeros(reinterpret_cast<wipp::wipp_complex_t*>(c), _length);
    ERROR_STREAM("Trying to use precomputed delayMatrix when it is not precomputed. Correlation vector is set to zero.");
  }
}

void GeneralisedCrossCorrelationImpl::generateTwoSidedFFT(const BaseTypeC *x, const BaseTypeC *y, BaseTypeC *fullx, BaseTypeC *fully, int length)
{
  //Generate fullx
  wipp::copyBuffer(reinterpret_cast<const wipp::wipp_complex_t*>(x),
		   reinterpret_cast<wipp::wipp_complex_t*>(fullx),
		   length);
  wipp::conjFlip(reinterpret_cast<const wipp::wipp_complex_t*>(&x[1]),
      reinterpret_cast<wipp::wipp_complex_t*>(&fullx[length]),
      length-2);

  //Generate fully
  wipp::copyBuffer(reinterpret_cast<const wipp::wipp_complex_t*>(y),
		   reinterpret_cast<wipp::wipp_complex_t*>(fully),
		   length);
  wipp::conjFlip(reinterpret_cast<const wipp::wipp_complex_t*>(&y[1]),
      reinterpret_cast<wipp::wipp_complex_t*>(&fully[length]), length-2);
}

void GeneralisedCrossCorrelationImpl::calculateCorrelationsCore(const  BaseTypeC *x, const  BaseTypeC *y, BaseTypeC *c,
							    BaseType *samplesDelay, int numSteps, bool usePrecomputedMatrix)
{
  wipp::conj(reinterpret_cast<const wipp::wipp_complex_t*>(x),
	     reinterpret_cast<wipp::wipp_complex_t*>(_correlation.get()),
	     _length);

  wipp::mult(reinterpret_cast<const wipp::wipp_complex_t*>(y),
	     reinterpret_cast<wipp::wipp_complex_t*>(_correlation.get()),
	     _length);

  //PHAT weight vector
  phatWeight(x, y, _weight.get());
  wipp::mult(reinterpret_cast<wipp::wipp_complex_t*>(_weight.get()),
	     reinterpret_cast<wipp::wipp_complex_t*>(_correlation.get()),
	     _length);

  for (int i = 0; i < numSteps; i++)
  {
    // pDst[n] = offset + slope*n, 0 ≤ n < len
    // pDst[n] = (-2*M_PI*tau/N)*n + 0;
    // where N = 2*^fftOrder

    if(usePrecomputedMatrix)
    {
	wipp::mult(reinterpret_cast<wipp::wipp_complex_t*>(_delayMatrix[i].get()),
		   reinterpret_cast<wipp::wipp_complex_t*>(_correlation.get()),
		   reinterpret_cast<wipp::wipp_complex_t*>(_generalisedcc.get()),
		   _length);
    }
    else
    {
      computePhaseRamp(samplesDelay[i]);
	wipp::mult(reinterpret_cast<wipp::wipp_complex_t*>(_delay.get()),
		   reinterpret_cast<wipp::wipp_complex_t*>(_correlation.get()),
		   reinterpret_cast<wipp::wipp_complex_t*>(_generalisedcc.get()),
		   _length);
    }

    wipp::mean(reinterpret_cast<wipp::wipp_complex_t*>(_generalisedcc.get()),
	       _length,
	       reinterpret_cast<wipp::wipp_complex_t*>(&c[i]));
  }

}

void GeneralisedCrossCorrelationImpl::phatWeight(const  BaseTypeC *x, const  BaseTypeC *y,  BaseTypeC *w)
{

  //                       1
  //        W(w) =  -------------------  ==>  PHAT (more or less)
  //                 |X_i(w)||X_j(w)|

  wipp::set(1.0, _phat.get(), _length);

  wipp::magnitude(reinterpret_cast<const wipp::wipp_complex_t*>(x), _magnitude.get(), _length);
  wipp::addC(0.5e-100, _magnitude.get(), _length);
  wipp::div(_magnitude.get(), _phat.get(), _length);

  wipp::magnitude(reinterpret_cast<const wipp::wipp_complex_t*>(y), _magnitude.get(), _length);
  wipp::addC(0.5e-100, _magnitude.get(), _length);
  wipp::div(_magnitude.get(), _phat.get(), _length);

  wipp::threshold_lt(_phat.get(), _length, 0.0001, 0.0001);

  wipp::real2complex(_phat.get(),
		     NULL,
		     reinterpret_cast<wipp::wipp_complex_t*>(w), _length);
}

void GeneralisedCrossCorrelationImpl::computePhaseRamp(BaseType tau)
{
  int positiveSideLength = _length/2 + 1;
  wipp::ramp(_phase.get(), positiveSideLength, 0, -2*M_PI*tau/_length);
  wipp::ramp(&_phase[positiveSideLength], _length-positiveSideLength, 2*M_PI*tau*(positiveSideLength-2)/_length, -2*M_PI*tau/_length);
  wipp::polar2cart(_ones.get(), _phase.get(), reinterpret_cast<wipp::wipp_complex_t*>(_delay.get()), _length);
}

void GeneralisedCrossCorrelationImpl::debugPlot(const BaseTypeC* left, const BaseTypeC *right, const BaseTypeC *c, int length, int numSteps, float tau)
{

  //          static int n = 0;
  //          n++;
  //          if (n < 10*30)
  //            return;

  BaseType real[_length];
  BaseType phase[_length];
  BaseType corrReal[numSteps];
  BaseTypeC complex[_length];

  //        _plot->reset_plot();
  //        _plot->set_grid();
  //        _plot->set_smooth("csplines");

  wipp::real(reinterpret_cast<const wipp::wipp_complex_t*>(c), corrReal, numSteps);

  //        _plot->plot_x(&corrReal[0], numSteps, "Correlation");

  //          BaseTypeC *c = _generalisedcc.get();

  //          // Plot input signals
  //          ippsMagnitude_64fc(left, real, length);
  //          _plot->plot_x(&real[0], _length, "left");
  //          ippsMagnitude_64fc(right, real, length);
  //          _plot->plot_x(&real[0], _length, "right");

  //          // Plot real part of the correlation vector
  //          ippsReal_64fc(c, real, _length);
  //          _plot->plot_x(&real[0], _length, "real part");

  //          // Plot the magnitude of the correlation (has to be one for existing frequencies)
  //          // if PHAT applied
  //          ippsMagnitude_64fc(c, real, _length);
  //          ippsRealToCplx_64f(real, NULL, complex, _length);
  //          ippsMul_64fc_I(complex,c, _length);
  //          _plot->plot_x(&real[0], _length, "magnitude");

  //          // Plot phase phase of correlation (indicates actual correlation 0 -> 1, +-PI -> -1
  //          ippsPhase_64fc(c, phase, _length);
  //         _plot->plot_x(&phase[0], _length, "phase");
  //          _plot->set_yrange(-M_PI, M_PI);

  //          // Plot the real obtained value
  //          ippsSet_64f(1, real, _length);
  //          ippsMulC_64fc_I(_corrValue, real, _length);
  //          _plot->plot_x(&real[0], _length, "correlation");

  //          // plot tau
  //          ippsSet_64f(1, real, _length);
  //          ippsMulC_64f_I(tau, real, _length);
  //          _plot->plot_x(&real[0], _length, "tau");

  //          util::Timer::sleepMs(300);
}


}

