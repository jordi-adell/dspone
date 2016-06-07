#ifndef __PARTICLEFILTERHELPERSIMPL_H_
#define __PARTICLEFILTERHELPERSIMPL_H_

#include <dspone/dspMath.h>

#include <wipp/wippstats.h>
#include <wipp/wipputils.h>

namespace dsp
{
namespace math
{

class RandState
{
    public:
	RandState() {}
	virtual ~RandState(){}
	wipp::wipp_rand_t *_state;
};

void init_rand_state_core(RandState **state, double mean, double stddev)
{
    *state = new RandState();
    wipp::init_rand_gaussian(&((*state)->_state), mean, stddev);
}
void delete_rand_state(RandState **state)
{
    wipp::delete_rand(&((*state)->_state));
    delete *state;
    state = NULL;
}
void rand(RandState *state, double *values, int length) {wipp::rand(state->_state, values, length);}

template <typename T, typename H>
void set_core(T value, H* buffer, int length)
{
    wipp::set(value, buffer, length);
}

template <typename T>
void set_zeros_core(T* buffer, int length)
{
    wipp::setZeros(buffer, length);
}

template <typename T, typename H>
void copy_core(const T *a, H *b, int length)
{
    wipp::copyBuffer(a, b, length);
}

template <typename T, typename H>
void add_core(const T *a, H *b, int length)
{
    wipp::add(a, b, length);
}

template <typename T, typename H>
void mult_core(const T *a, H *b, int length)
{
    wipp::mult(a, b, length);
}

template <typename T, typename H, typename Z>
void mult_core(const T *a, H *b, Z *c, int length)
{
    wipp::mult(a, b, c, length);
}

template <typename T, typename H>
void sum_core(const T *buffer, int length, H *value)
{
    wipp::sum(buffer, length, value);
}

template <typename T, typename H>
void subC_core(T value, H *buffer, int length)
{
    wipp::subC(value, buffer, length);
}

template <typename T, typename H>
void divC_core(T value, H *buffer, int length)
{
    wipp::divC(value, buffer, length);
}

template <typename T, typename H>
void multC_core(T value, H *buffer, int length)
{
    wipp::multC(value, buffer, length);
}

template <typename T, typename H>
void addC_core(T value, H *buffer, int length)
{
    wipp::addC(value, buffer, length);
}

template <typename T, typename H>
void addC_core(T value, const H *buffer_in, H *buffer_out, int length)
{
    wipp::addC(value, buffer_in, buffer_out, length);
}

template <typename T, typename H>
void stddev_core(const T *buffer, int length, H *stddev)
{
    wipp::stddev(buffer, length, stddev);
}

template <typename T>
void minmax_core(T* buffer, int length, T *min, T*max)
{
    wipp::minmax(buffer, length, min, max);
}

}
}

#endif // PARTICLEFILTERHELPERSIMPL_H
