#ifndef __PARTICLE_FILTER_HELPERS_H_
#define __PARTICLE_FILTER_HELPERS_H_

#include <dspone/complex.h>
#include <stdint.h>

namespace dsp
{

namespace math
{

class RandState;

void init_rand_state(RandState **state, double mean, double stddev);
void delete_rand_state(RandState **state);
void rand(RandState *state, double *values, int length);

void set(int value, int* buffer, int length);
void setZeros(int* buffer, int length);
void copy(int* a, int* b, int length);
void add(int* a, int* b, int length);
void mult(int* a, int* b, int length);
void mult(int* a, int* b, int *c, int length);
void div(int* a, int* b, int length);
void div(int* a, int* b, int *c, int length);
void sum(int* a, int length, int* value);
void subC(int value, int *buffer, int length);
void divC(int value, int *buffer, int length);
void multC(int value, int *buffer, int length);
void addC(int value, int *buffer, int length);
void addC(int value, int *a, int *b, int length);

void set(double value, double* buffer, int length);
void setZeros(double* buffer, int length);
void copy(double* a, double* b, int length);
void add(double* a, double* b, int length);
void sum(double* a, int length, double* value);
void mult(double* a, double* b, int length);
void mult(double* a, double* b, double *c, int length);
void div(double* a, double* b, int length);
void div(double* a, double* b, double *c, int length);
void subC(double value, double *buffer, int length);
void divC(double value, double *buffer, int length);
void multC(double value, double *buffer, int length);
void addC(double value, double *buffer, int length);
void addC(double value, double *buffer_in, double *buffer_out, int length);

void set(int16_t value, int16_t* buffer, int length);
void setZeros(int16_t* buffer, int length);
void copy(int16_t* a, int16_t* b, int length);
void add(int16_t* a, int16_t* b, int length);
void sum(int16_t* a, int length, int16_t* value);
void mult(int16_t *a, int16_t* b, int length);
void mult(int16_t *a, int16_t* b, int16_t *c, int length);
void div(int16_t *a, int16_t* b, int length);
void div(int16_t *a, int16_t* b, int16_t *c, int length);
void subC(int16_t value, int16_t *buffer, int length);
void divC(int16_t value, int16_t *buffer, int length);
void multC(int16_t value, int16_t *buffer, int length);
void addC(int16_t value, int16_t *buffer, int length);
void addC(int16_t value, int16_t *a, int16_t *b, int length);

void set(float value, float* buffer, int length);
void setZeros(float* buffer, int length);
void copy(float* a, float* b, int length);
void add(float* a, float* b, int length);
void sum(float* a, int length, float* value);
void mult(float* a, float* b, int length);
void mult(float* a, float* b, float *c, int length);
void div(float* a, float* b, int length);
void div(float* a, float* b, float *c, int length);
void subC(float value, float *buffer, int length);
void divC(float value, float *buffer, int length);
void multC(float value, float *buffer, int length);
void addC(float value, float *buffer, int length);
void addC(float value, float *a, float *b, int length);

void copy(int16_t *a, double *b, int length);

void stddev(int *buffer, int length, double *stddev);
void stddev(double *buffer, int length, double *stddev);
void stddev(float *buffer, int length, double *stddev);
void stddev(int16_t* buffer, int length, double *stddev);

void minmax(int* buffer, int length, int *min, int*max);
void minmax(double *buffer, int length, double *min, double *max);
void minmax(float *buffer, int length, float *min, float *max);
void minmax(int16_t *buffer, int length, int16_t *min, int16_t *max);

void magnitude(Complex *buffer, double *magn, int length);
}


}
#endif // DspOneHelpers_H
