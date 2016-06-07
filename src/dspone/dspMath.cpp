#include <dspone/dspMath.h>
#include <dspone/dspMathImpl.hpp>

namespace dsp
{
namespace math
{

void init_rand_state(RandState **state, double mean, double stddev){ init_rand_state_core(state, mean, stddev);}

void delete_rand_state(RandState **state);
void rand(RandState *state, double *values, int length);

void set(int value, int* buffer, int length){set_core(value, buffer, length);}
void setZeros(int *buffer, int length){set_zeros_core(buffer, length);}
void copy(int* a, int* b, int length){copy_core(a, b, length);}
void add(int* a, int* b, int length){add_core(a, b, length);}
void mult(int* a, int* b, int length){mult_core(a, b, length);}
void mult(int* a, int* b, int *c, int length){mult_core(a, b, c, length);}
void subC(int value, int *buffer, int length){subC_core(value, buffer, length);}
void divC(int value, int *buffer, int length){divC_core(value, buffer, length);}
void multC(int value, int *buffer, int length){multC_core(value, buffer, length);}
void sum(int *buffer, int length, int *value){sum_core(buffer, length, value);}
void addC(int value, int *buffer, int length){addC_core(value, buffer, length);}
void addC(int value, int *buffer_in, int *buffer_out, int length){addC_core(value, buffer_in, buffer_out, length);}


void set(double value, double* buffer, int length){set_core(value, buffer, length);}
void setZeros(double *buffer, int length){set_zeros_core(buffer, length);}
void copy(double* a, double* b, int length){copy_core(a, b, length);}
void add(double* a, double* b, int length){add_core(a, b, length);}
void mult(double* a, double* b, int length){mult_core(a, b, length);}
void mult(double* a, double* b, double *c, int length){mult_core(a, b, c, length);}
void subC(double value, double *buffer, int length){subC_core(value, buffer, length);}
void divC(double value, double *buffer, int length){divC_core(value, buffer, length);}
void multC(double value, double *buffer, int length){multC_core(value, buffer, length);}
void sum(double *buffer, int length, double *value){sum_core(buffer, length, value);}
void addC(double value, double *buffer, int length){addC_core(value, buffer, length);}
void addC(double value, double *buffer_in, double *buffer_out, int length){addC_core(value, buffer_in, buffer_out, length);}

void set(int16_t value, int16_t* buffer, int length){set_core(value, buffer, length);}
void setZeros(int16_t *buffer, int length){set_zeros_core(buffer, length);}
void copy(int16_t* a, int16_t* b, int length){copy_core(a, b, length);}
void add(int16_t* a, int16_t* b, int length){add_core(a, b, length);}
void mult(int16_t* a, int16_t* b, int length){mult_core(a, b, length);}
void mult(int16_t* a, int16_t* b, int16_t* c, int length){mult_core(a, b, c, length);}
void subC(int16_t value, int16_t *buffer, int length){subC_core(value, buffer, length);}
void divC(int16_t value, int16_t *buffer, int length){divC_core(value, buffer, length);}
void multC(int16_t value, int16_t *buffer, int length){multC_core(value, buffer, length);}
void sum(int16_t *buffer, int length, int16_t *value){sum_core(buffer, length, value);}
void addC(int16_t value, int16_t *buffer, int length){addC_core(value, buffer, length);}
void addC(int16_t value, int16_t *buffer_in, int16_t *buffer_out, int length){addC_core(value, buffer_in, buffer_out, length);}

void set(float value, float* buffer, int length){set_core(value, buffer, length);}
void setZeros(float *buffer, int length){set_zeros_core(buffer, length);}
void copy(float* a, float* b, int length){copy_core(a, b, length);}
void add(float* a, float* b, int length){add_core(a, b, length);}
void mult(float* a, float* b, int length){mult_core(a, b, length);}
void mult(float* a, float* b, float *c, int length){mult_core(a, b, c, length);}
void subC(float value, float *buffer, int length){subC_core(value, buffer, length);}
void divC(float value, float *buffer, int length){divC_core(value, buffer, length);}
void multC(float value, float *buffer, int length){multC_core(value, buffer, length);}
void sum(float *buffer, int length, float *value){sum_core(buffer, length, value);}
void addC(float value, float *buffer, int length){addC_core(value, buffer, length);}
void addC(float value, float *buffer_in, float *buffer_out, int length){addC_core(value, buffer_in, buffer_out, length);}

void stddev(int *buffer, int length, double *stddev) {stddev_core(buffer, length, stddev);}
void stddev(double *buffer, int length, double*stddev) {stddev_core(buffer, length, stddev);}
void stddev(float *buffer, int length, double *stddev) {stddev_core(buffer, length, stddev);}
void stddev(int16_t* buffer, int length, double *stddev) {stddev_core(buffer, length, stddev);}

void minmax(int* buffer, int length, int *min, int*max){minmax_core(buffer, length, min, max);}
void minmax(double *buffer, int length, double *min, double *max){minmax_core(buffer, length, min, max);}
void minmax(float *buffer, int length, float *min, float *max){minmax_core(buffer, length, min, max);}
void minmax(int16_t *buffer, int length, int16_t *min, int16_t *max){minmax_core(buffer, length, min, max);}


}

}
