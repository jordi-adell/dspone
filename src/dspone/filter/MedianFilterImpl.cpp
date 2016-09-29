#include <dspone/filter/MedianFilterImpl.h>

#include <wipp/wippsignal.h>
#include <algorithm>

namespace dsp
{

MedianFilterImpl::MedianFilterImpl(int maskSize) :
    _maskSize((maskSize <= 0) ? 1 : maskSize),
    _leftMaskSize ((_maskSize%2 == 0) ? (_maskSize-1)/2 : _maskSize/2),
    _rightMaskSize((_maskSize%2 == 0) ? (_maskSize-1)/2 : _maskSize/2)
{

  wipp::init_cirular_buffer<double>(&_cbuffer, _maskSize);
  _frame.reset(new double[_maskSize]);
}


MedianFilterImpl::~MedianFilterImpl()
{
  wipp::delete_circular_buffer(&_cbuffer);
}

int MedianFilterImpl::getFrameLength() const
{
  return _maskSize;
}




template<typename T>
void MedianFilterImpl::filterBuffer(const T *inbuffer, T *outbuffer, int length)
{
  size_t i = 0;

  wipp::cf_occupancy(_cbuffer, &_occupancy);
  while(_occupancy > _maskSize && i < length)
  {
    wipp::cf_next(_cbuffer, _frame.get(), _maskSize, NULL);
    wipp::median(_frame.get(), _maskSize, &outbuffer[i]);
    ++i;
    wipp::cf_skip(_cbuffer, 1, NULL);
  }

  wipp::cf_free(_cbuffer, _free);
  if (length > 0 && _free >= length)
  {
    wipp::cf_write(_cbuffer, inbuffer, length, NULL);
    filterBuffer<T>(NULL, NULL, 0);
  }
  else
  {
    wipp::cf_write(_cbuffer, inbuffer, _free, NULL);
    filterBuffer(&inbuffer[_free], &outbuffer[i], _free);
  }


}


}
