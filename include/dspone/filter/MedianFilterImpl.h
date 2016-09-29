#ifndef __DSPONE_MEDIAN_FILTER_IMPL_H__
#define __DSPONE_MEDIAN_FILTER_IMPL_H__

#include <wipp/wipputils.h>
#include <wipp/wippsignal.h>
#include <wipp/wippstats.h>

#include <boost/shared_array.hpp>

#include <memory>

namespace  dsp {

class MedianFilterImpl
{
    public:
	/**
	 * @brief MedianFilte
	 * _maskSize is the number of samples used in the median file.
	 * If and even number it is changed into the lower odd number
	 * by substracting one.
	 * @param _maskSize
	 */
	MedianFilterImpl(int _maskSize);
	virtual ~MedianFilterImpl();

	template<typename T>
	void filterBuffer(const T* inbuffer, T* outbuffer, int length);

	int getFrameLength() const;

    private:
	// maskSize = leftMaskSize + rightMaskSize + 1;
	const int _maskSize;
	const int _leftMaskSize;
	const int _rightMaskSize;
	std::unique_ptr<double> _frame;
	size_t _occupancy;
	size_t _free;
	wipp::wipp_circular_buffer_t *_cbuffer;

};




}



#endif
