#ifndef __DSPONE_MEDIAN_FILTER_H__
#define __DSPONE_MEDIAN_FILTER_H__

#include <dspone/filter/Filter.h>

namespace  dsp {


class MedianFilter : public Filter
{
    public:
	/**
	 * @brief MedianFilte
	 * maskSize is the number of samples used in the median file.
	 * If and even number it is changed into the lower odd number
	 * by substracting one.
	 * @param maskSize
	 */
	MedianFilter(int maskSize);
	virtual ~MedianFilter();

	virtual void filterBuffer(const int16_t *inbuffer, int16_t *outbuffer, int length);
	virtual void filterBuffer(const int32_t *inbuffer, int32_t *outbuffer, int length);
	virtual void filterBuffer(const uint16_t *inbuffer, uint16_t *outbuffer, int length);
	virtual void filterBuffer(const double *inbuffer, double *outbuffer, int length);
	virtual void filterBuffer(const float *inbuffer, float *outbuffer, int length);

	virtual int getFrameLength() const;

    private:
	int _leftMaskSize;
	int _rightMaskSize;
};

}



#endif
