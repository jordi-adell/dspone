#include <dspone/filter/MedianFilter.h>
#include <dspone/filter/MedianFilterImpl.h>

#include <wipp/wippstats.h>
#include <wipp/wipputils.h>
#include <algorithm>

namespace dsp
{


namespace median_filter_impl
{

template<typename T>
void filterBuffer(const T *inbuffer, T *outbuffer, int length, int leftmask, int rightmask)
{
  size_t maskSize = leftmask + rightmask + 1;
  T aux[maskSize + std::max(leftmask, rightmask)];

  wipp::set(inbuffer[0], aux, leftmask);
  memcpy(&aux[leftmask], inbuffer, maskSize*sizeof(T));

  for (size_t i = 0; i < leftmask; ++i)
  {
    wipp::median(&aux[i], maskSize, &outbuffer[i]);
  }

  for (size_t i = leftmask; i < length - rightmask; ++i)
  {
    wipp::median(&inbuffer[i - leftmask], maskSize, &outbuffer[i]);
  }

  memcpy(aux, &inbuffer[length - maskSize], maskSize*sizeof(T));
  wipp::set(inbuffer[length - 1], &aux[maskSize], rightmask);

  for (size_t i = 0; i < rightmask; ++i)
  {
    wipp::median(&aux[i], maskSize, &outbuffer[length - rightmask + i]);
  }

}

}


MedianFilter::MedianFilter(int maskSize)
{
  maskSize = (maskSize <= 0) ? 1 : maskSize;
  _leftMaskSize  = (maskSize%2 == 0) ? (maskSize-1)/2 : maskSize/2;
  _rightMaskSize = (maskSize%2 == 0) ? (maskSize-1)/2 : maskSize/2;
}


MedianFilter::~MedianFilter()
{

}


void MedianFilter::filterBuffer(const double *inbuffer, double *outbuffer, int length)
{
  median_filter_impl::filterBuffer(inbuffer, outbuffer, length, _leftMaskSize, _rightMaskSize);
}

void MedianFilter::filterBuffer(const float *inbuffer, float *outbuffer, int length)
{
  median_filter_impl::filterBuffer(inbuffer, outbuffer, length, _leftMaskSize, _rightMaskSize);
}

void MedianFilter::filterBuffer(const int16_t *inbuffer, int16_t *outbuffer, int length)
{
  median_filter_impl::filterBuffer(inbuffer, outbuffer, length, _leftMaskSize, _rightMaskSize);
}

void MedianFilter::filterBuffer(const int32_t *inbuffer, int32_t *outbuffer, int length)
{
  median_filter_impl::filterBuffer(inbuffer, outbuffer, length, _leftMaskSize, _rightMaskSize);
}

void MedianFilter::filterBuffer(const uint16_t *inbuffer, uint16_t *outbuffer, int length)
{
  median_filter_impl::filterBuffer(inbuffer, outbuffer, length, _leftMaskSize, _rightMaskSize);
}


int MedianFilter::getFrameLength() const
{
  return _leftMaskSize + _rightMaskSize + 1;
}

}
