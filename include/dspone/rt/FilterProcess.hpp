/*
* FilterProcess.h
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
#ifndef __FILTER_PROCESS_H_
#define __FILTER_PROCESS_H_

#include <dspone/rt/SignalProcessor.h>
#include <dspone/filter/Filter.h>
#include <dspone/DspException.h>
#if BOOST_FOUND
#include <boost/type_traits.hpp>
#endif

#include <boost/shared_array.hpp>

#include <vector>

namespace dsp {

typedef std::unique_ptr<Filter> FilterPtr;

/**
       * @brief The FilterProcess class
       * This class is used to wrap all algorithms for filtering.
       * It is different from ShortTimeProcessing in that
       * here no overlap and add is performed.
       * I the case a specific frame length is used, getBufferSize
       * will return the length of the frame or -1 if it does not care.
       */
class FilterProcess : SignalProcessor
{

    public:

	/**
	 * @brief FilterProcess
	 * Constructs and empty filter process
	 * @param nchannels   number of channels to process
	 */
	FilterProcess(int nchannels);
	virtual ~FilterProcess();

	template <class F>
	/**
	 * @brief make_FilterProcess
	 * Construct a filterprocess with a specific type of filters
	 * and returns a pointer to it.
	 * @param nchannels   number of channels to process
	 * @param coefs   coeficients of the filter
	 * @param length   number of coeficients of the filter.
	 * @return  a pointer to the created class.
	 */
	static FilterProcess* make_FilterProcess(int nchannels, const double *coefs, int length);

	template <class F>
	/**
	 * @brief make_FilterProcess
	 * Construct a filterprocess with a specific type of filters
	 * and returns a pointer to it.
	 * @param nchannels   number of channels to process
	 * @param order  order of the filter
	 * @param lowFreq
	 * @param highFreq
	 * @return  a pointer to the created class.
	 */
	static FilterProcess* make_FilterProcess(int nchannels, int order, double lowFreq, double highFreq);


	template <class F>
	/**
	 * @brief constructFilters
	 * function to construct a filter per channel
	 * @param coefs   coeficients of the filter.
	 * @param length   number of coeficients
	 */
	void constructFilters(const double *coefs, int length);
	template <class F>
	/**
	 * @brief constructFilters
	 * function to construct a filter per channel
	 * @param order    order of the filter
	 * @param lowFreq
	 * @param highFreq
	 */
	void constructFilters(int order, double lowFreq, double highFreq);

	template <class F>
	/**
	 * @brief constructFilters
	 * function to construct a filter per channel
	 * @param order    order of the filter
	 * @param lowFreq
	 * @param highFreq
	 */
	void constructFilters(int order);

	virtual int process(const std::vector<double*> &signal,
		    unsigned int inbuffersize,
		    const std::vector<double*> &output,
		    unsigned int outbuffersize);

	virtual int process(const std::vector<float*> &signal,
		    unsigned int inbuffersize,
		    const std::vector<float*> &output,
		    unsigned int outbuffersize);

	virtual int process(const std::vector<int16_t*> &signal,
		    unsigned int inbuffersize,
		    const std::vector<int16_t*> &output,
		    unsigned int outbuffersize);

	virtual int process(const std::vector<uint16_t*> &signal,
		    unsigned int inbuffersize,
		    const std::vector<uint16_t*> &output,
		    unsigned int outbuffersize);

	virtual int getLatency() const;
	virtual int getMaxLatency() const;
	virtual int getBufferSize() const;
	virtual int getNumberOfChannels() const;


    private:

	int _nchannels;
	std::vector<FilterPtr> _filters;


	template <typename SampleType>
	int process_core(const std::vector<SampleType *> &signal,
		    unsigned int inbuffersize,
		    const std::vector<SampleType *> &output,
		    unsigned int outbuffersize);

	template <typename SampleType>
	int process_core(const std::vector<boost::shared_array<SampleType> > &signal,
		    unsigned int inbuffersize,
		    const std::vector<boost::shared_array<SampleType> > &output,
		    unsigned int outbuffersize);
};


template <class F >  void FilterProcess::constructFilters(const double *coefs, int length)
{
    for (int c = 0; c < _nchannels; ++c)
    {
	_filters.push_back(FilterPtr(new F(coefs, length)));
    }
}

template <class F > void  FilterProcess::constructFilters(int order, double lowFreq, double highFreq)
{
    for (int c = 0; c < _nchannels; ++c)
    {
	_filters.push_back(FilterPtr(new F(order, lowFreq, highFreq)));
    }
}

template <class F > void  FilterProcess::constructFilters(int order)
{
    for (int c = 0; c < _nchannels; ++c)
    {
	_filters.push_back(FilterPtr(new F(order)));
    }
}




template <typename SampleType>
int FilterProcess::process_core(const std::vector<boost::shared_array<SampleType> > &signal,
				unsigned int inbuffersize,
				const std::vector<boost::shared_array<SampleType> > &output,
				unsigned int outbuffersize)
{
    if (inbuffersize != outbuffersize)
    {
	throw(DspException("Filters expect the same length for input and output buffers"));
    }


    for (int c = 0; c < _nchannels && c < _filters.size(); ++c)
    {
	_filters[c].get()->filterBuffer(signal[c].get(), output[c].get(), outbuffersize);
    }

    return outbuffersize;

}

template <typename SampleType>
int FilterProcess::process_core(const std::vector<SampleType*> &signal,
				unsigned int inbuffersize,
				const std::vector<SampleType*> &output,
				unsigned int outbuffersize)
{
    if (inbuffersize != outbuffersize)
    {
	throw(DspException("Filters expect the same length for input and output buffers"));
    }


    for (int c = 0; c < _nchannels && c < _filters.size(); ++c)
    {
	_filters[c]->filterBuffer(signal[c], output[c], outbuffersize);
    }

    return outbuffersize;
}


template <class F >
FilterProcess* FilterProcess::make_FilterProcess(int nchannels, const double *coefs, int length)
{
    // convert dyanmic convertions into compilation asserts, to avoid
    // hard to debug errors
#if BOOST_FOUND
    BOOST_STATIC_ASSERT_MSG( (boost::is_base_and_derived<Filter, F>::value) ,
			     "The filter type provided MUST inherit from the Filter interface class.");
#endif
    FilterProcess *filter = new FilterProcess(nchannels);
    filter->constructFilters<F>(coefs, length);
    return filter;
}

template <class F >
FilterProcess* FilterProcess::make_FilterProcess(int nchannels, int order, double lowFreq, double highFreq)
{
    // convert dyanmic convertions into compilation asserts, to avoid
    // hard to debug errors
#if BOOST_FOUND
    BOOST_STATIC_ASSERT_MSG( (boost::is_base_and_derived<Filter, F>::value) ,
			     "The filter type provided MUST inherit from the Filter interface class.");
#endif
    FilterProcess *filter = new FilterProcess(nchannels);
    filter->constructFilters<F>(order, lowFreq, highFreq);
    return filter;
}


}

#endif // __FILTER_PROCESS_H_

