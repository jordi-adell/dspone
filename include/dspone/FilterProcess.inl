#include "FilterProcess.h"

#include <boost/shared_ptr.hpp>

namespace dsp {


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
int FilterProcess::process(const std::vector<boost::shared_array<SampleType> > &signal,
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
int FilterProcess::process(const std::vector<SampleType*> &signal,
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
    BOOST_STATIC_ASSERT_MSG( (boost::is_base_and_derived<Filter, F>::value) ,
			     "The filter type provided MUST inherit from the Filter interface class.");

    FilterProcess *filter = new FilterProcess(nchannels);
    filter->constructFilters<F>(coefs, length);
    return filter;
}

template <class F >
FilterProcess* FilterProcess::make_FilterProcess(int nchannels, int order, double lowFreq, double highFreq)
{
    // convert dyanmic convertions into compilation asserts, to avoid
    // hard to debug errors
    BOOST_STATIC_ASSERT_MSG( (boost::is_base_and_derived<Filter, F>::value) ,
			     "The filter type provided MUST inherit from the Filter interface class.");
    FilterProcess *filter = new FilterProcess(nchannels);
    filter->constructFilters<F>(order, lowFreq, highFreq);
    return filter;
}


}
