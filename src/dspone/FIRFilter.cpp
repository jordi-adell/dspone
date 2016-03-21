/*
* FIRFilter.cpp
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
* along with WIPP.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <dspone/FIRFilter.h>
#include <dspone/dsplogger.h>
#include <dspone/dspdefs.h>

#include <wipp/wippdefs.h>
#include <wipp/wipputils.h>
#include <wipp/wippfilter.h>

#include <boost/filesystem.hpp>

#include <vector>
#include <fstream>
#include <cstdlib>

namespace dsp {

class FIRFilterImpl
{
  public:
    /** Initialises a FIR filter with the specified length and coeficients
	  @param coefs  Vector where coefficients are stored
	  @param length Number of coefficients of the filter
	  **/
    FIRFilterImpl(const double *coefs, int length);
    /** Same as previous constructor, but coefficients are read from a text file */
    FIRFilterImpl(const std::string &file);

    ~FIRFilterImpl();

    /** This fucntion filteres a buffer, taking into account previous filtered signal  as history,
	  then writes the output in outBuffer.
	  @param inBuffer  signal to be filtered
	  @param outBuffer place to store the desired signal, must be already allocated and of size length
	  **/
    void filter(const int16_t *inBuffer, int16_t *outBuffer, int length);
    void filter(const float  *inBuffer, float  *outBuffer, int length);
    void filter(const double *inBuffer, double *outBuffer, int length);

    /** return the order of the filter **/
    int getOrder(){return _order;}

  private:
    wipp::wipp_fir_filter_t *_fir_filter;
    int _order;

    /** This function initialises de filter from a vector of Ipp formated values **/
    void initFilter_BaseType(const double *coefs, int length);
    /** This fcuntion initialises de filter from a vector of double, used previous function **/
    template <class T> void initFilter(const T *coefs, int length);
    /** This function initialised the filter from a vector of values read from a text file,
	  used previous functions**/
    void initFilterFromFile(const std::string &file);

    /** Sets default values, to avoid having uninitialised members **/
    void setDefaultValues();

    /** Closes de filter and frees memory **/
    void closeFilter();

    /** This function filters a buffer taking into account previous hystory stored in state **/
    void filterCore(const double *source, double *dest, int length);
};




FIRFilterImpl::FIRFilterImpl(const double *coefs, int length)
{
  setDefaultValues();
  initFilter(coefs,length);
}

FIRFilterImpl::FIRFilterImpl(const std::string &file)
{
  setDefaultValues();
  if (boost::filesystem::exists(file))
  {
    initFilterFromFile(file);
  }
  else
  {
    ERROR_STREAM("File does not exist: " << file);
  }
}

FIRFilterImpl::~FIRFilterImpl()
{
  closeFilter();
}

void FIRFilterImpl::setDefaultValues()
{
  _order = 0;
  _fir_filter = NULL;
}

void FIRFilterImpl::initFilterFromFile(const std::string &file)
{
  std::vector<std::string> scoefs;
  std::vector<std::string>::const_iterator it;
  std::ifstream ifs(file);
  while(ifs)
  {
    std::string value;
    ifs >> value;
    scoefs.push_back(value);
  }

  size_t length = scoefs.size();
  BaseType coefs[length];
  wipp::setZeros(coefs,length);

  size_t i = 0;
  for (i=0, it = scoefs.begin(); it != scoefs.end() && i<length; ++it, ++i)
  {
    coefs[i] = atof(it->c_str());
  }
  initFilter(coefs,length);
}


template <class T> void FIRFilterImpl::initFilter(const T *coefs, int length)
{
  BaseType taps[length];
  wipp::copyBuffer(coefs, taps, length);
  initFilter_BaseType(taps,length);
}


void FIRFilterImpl::initFilter_BaseType(const BaseType *coefs, int length)
{
  _order = length;
  if (_fir_filter != NULL)
    closeFilter();

  BaseType pastValues[length];
  wipp::setZeros(pastValues,length);
  wipp::init_fir(&_fir_filter, coefs, length, pastValues);
}


void FIRFilterImpl::closeFilter()
{
  if (_fir_filter != NULL)
    wipp::delete_fir(&_fir_filter);
  _fir_filter = NULL;
}

void FIRFilterImpl::filter(const int16_t *inBuffer, int16_t *outBuffer, int length)
{
  BaseType source[length];
  BaseType dest[length];

  wipp::copyBuffer(inBuffer, source, length);
  filterCore(source, dest, length);
  wipp::copyBuffer(dest, outBuffer, length);
}

void FIRFilterImpl::filter(const float *inBuffer, float *outBuffer, int length)
{
  BaseType source[length];
  BaseType dest[length];

  wipp::copyBuffer(inBuffer, source, length);
  filterCore(source, dest, length);
  wipp::copyBuffer(dest, outBuffer, length);
}

void FIRFilterImpl::filter(const double *inBuffer, double *outBuffer, int length)
{
  filterCore(inBuffer, outBuffer, length);
}



void FIRFilterImpl::filterCore(const BaseType *source, BaseType *dest, int length)
{
  if (_fir_filter != NULL)
    wipp::fir_filter(_fir_filter, source, dest, length);
  else
  {
    WARN_STREAM("Wrongly initialised filter, singal will not be filtered.");
    for (int i=0; i<length; ++i)
      dest[i] = source[i];
  }
}



// FIR Filter -- interface


FIRFilter::FIRFilter(const double *coefs, int length)
{
  _impl.reset(new FIRFilterImpl(coefs, length));
}

FIRFilter::FIRFilter(const std::string &file)
{
  _impl.reset(new FIRFilterImpl(file));
}

FIRFilter::~FIRFilter()
{

}

void FIRFilter::filterBuffer(const double *inBuffer, double *outBuffer, int length)
{
  _impl->filter(inBuffer, outBuffer, length);
}

void FIRFilter::filterBuffer(const float *inBuffer, float *outBuffer, int length)
{
  _impl->filter(inBuffer, outBuffer, length);
}

void FIRFilter::filterBuffer(const int16_t *inBuffer, int16_t *outBuffer, int length)
{
  _impl->filter(inBuffer, outBuffer, length);
}


}

