/*
* stft.cpp
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
#include "fftImpl.h"
#include <dspone/rt/stft.h>
#include <wipp/wippdefs.h>


namespace dsp {


// ------------ STFT -------------------------------------------------
// (+2) This is to allocate enough memory for CCs IPP compact complex data
STFT::STFT(int order) :
  ShortTimeProcess( _fft.getAnalysisWindowLength(order), _fft.getFFTLength(order), 1),
  _fft(order),
  _oneSidedFFTLength(_analysisLength/2),
  _fftOrder(_fft.getOrder())
{
  _ccsConversion.reset(new BaseTypeC[_analysisLength]);
}

STFT::STFT(int nchannels, int order) :
  ShortTimeProcess( _fft.getAnalysisWindowLength(order), _fft.getFFTLength(order), nchannels),
  _fft(order),
  _oneSidedFFTLength(_analysisLength/2),
  _fftOrder(_fft.getOrder())
{
  _ccsConversion.reset(new BaseTypeC[_analysisLength]);
}

STFT::~STFT()
{

}

void STFT::processParametrisation()
{

}

void STFT::frameAnalysis(BaseType *inFrame, BaseType *analysis, int frameLength, int, int)
{
  _fft.fwdTransform(inFrame, analysis, frameLength);
}

void STFT::frameSynthesis(BaseType *outFrame, BaseType *analysis, int frameLength, int, int)
{
  _fft.invTransfrom(outFrame, analysis, frameLength);
}

int STFT::getAnalysisLength() const
{
  return _fft.getFFTLength();
}

// ------- STFT Analysis only ------------------------------------------------

STFTAnalysis::STFTAnalysis(int channels, int order) :
  ShortTimeAnalysis( _fft.getAnalysisWindowLength(order), _fft.getFFTLength(order), channels),
  _fft(order)
{

}

STFTAnalysis::STFTAnalysis(int order) :
  ShortTimeAnalysis( _fft.getAnalysisWindowLength(order), _fft.getFFTLength(order), 1),
  _fft(order)
{

}

void STFTAnalysis::frameAnalysis(BaseType *inFrame, BaseType *analysis, int , int , int )
{
  _fft.fwdTransform(inFrame, analysis);
}

int STFTAnalysis::getAnalysisLength() const
{
  return _fft.getFFTLength();
}

// ------------- STFT multiband analysis in Mel/Linear Scale --------------------------

SubBandSTFTAnalysis::SubBandSTFTAnalysis(
    int nbins, int sampleRate, int order, int channels, float minFreq, float maxFreq, FilterBankType type) :
  STFTAnalysis(channels, order),
  _numberOfBins(nbins),
  _sampleRate(sampleRate)
{
  _coeficientsLength = _numberOfBins * _analysisLength/2;

  switch(type)
  {
    case MEL:
      _filterBank.reset(new FilterBankFFTWMelScale(_fft.getOrder(), _numberOfBins, _sampleRate, minFreq, maxFreq));
    break;
    case LINEAR:
    {
      double linearStart = (minFreq < sampleRate/2) ? minFreq : sampleRate/2;
      double linearEnd   = (maxFreq < sampleRate/2) ? maxFreq : sampleRate/2;
      std::vector<double> linearFreqs(_numberOfBins);
      double linearDelta = (linearEnd - linearStart)/_numberOfBins;
      for (int i=0; i<_numberOfBins; ++i)
	linearFreqs[i] = (linearStart + (i+1)*linearDelta/2)/_sampleRate;
      _filterBank.reset(new FilterBankFFTW(_fft.getOrder(), linearFreqs));
      break;
    }
    default:
      throw(DspException("Incorrect filter bank type."));
  }

  _filterCoeficients.reset(new BaseTypeC[_coeficientsLength]);
  BaseType coefs[_coeficientsLength];
  int gotNCoefs = _filterBank->getFiltersCoeficients(coefs, _coeficientsLength);
  if (gotNCoefs != _coeficientsLength)
  {
    std::ostringstream oss;
    oss << "Number of filter coeficients different from expected: " << gotNCoefs << " instead of " << _coeficientsLength;
    throw(DspException(oss.str()));
  }

  wipp::real2complex(coefs, NULL,
		     reinterpret_cast<wipp::wipp_complex_t*>(_filterCoeficients.get()),
		     _coeficientsLength);

  for (unsigned int c = 0; c < _nchannels; ++c)
  {
    _processedAnalysisFrames.push_back(SignalPtr(new BaseType[_analysisLength]));
    wipp::setZeros(_processedAnalysisFrames.back().get(), _analysisLength);
    _subbandAnalysisFrames.push_back(SignalPtr(new BaseType[_analysisLength]));
    wipp::setZeros(_subbandAnalysisFrames.back().get(),_analysisLength);
  }
}

void SubBandSTFTAnalysis::processParametrisation()
{
  processSetup();
  for (int bin = 0; bin < _numberOfBins; ++bin) // Residual should not be processed (i<_nBins) instead of (i<=_nBins)
  {
    for (unsigned int c = 0; c < _nchannels; ++c)
    {
      wipp::mult(reinterpret_cast<wipp::wipp_complex_t*>(_analysisFrames[c].get()),
		 reinterpret_cast<wipp::wipp_complex_t*>(&(_filterCoeficients.get()[bin*_analysisLength/2])),
	  reinterpret_cast<wipp::wipp_complex_t*>(_subbandAnalysisFrames[c].get()),
	  _analysisLength/2);
    }

    processOneSubband(_subbandAnalysisFrames, _analysisLength, bin);
  }
  processSumamry();
}

//void SubBandSTFTAnalysis::debugPlotFilterBank()
//{

//  Gnuplot *plot = new Gnuplot("lines");
//  plot->reset_plot();
//  plot->set_style("lines");
//  plot->set_grid();

//  BaseType realCoeficients[_coeficientsLength];
//  BaseType imagCoeficients[_coeficientsLength];
//  BaseType sumCoeficients[_analysisLength/2];

//  ippsCplxToReal_64fc(_filterCoeficients.get(), realCoeficients, imagCoeficients, _coeficientsLength);

//  ippsZero_64f(sumCoeficients, _analysisLength/2);

//  for (int bin = 0; bin < _numberOfBins; ++bin)
//  {
//    plot->plot_x(&realCoeficients[bin*_analysisLength/2], _analysisLength/2, "Filter Coefs");
//    ippsAdd_64f_I(&realCoeficients[bin*_analysisLength/2], sumCoeficients, _analysisLength/2);
//  }

//  plot->plot_x(&sumCoeficients[0], _analysisLength/2, "Filter Sum");
//}


DummySTFT::DummySTFT()
{

}

void DummySTFT::processParametrisation()
{

}

}

