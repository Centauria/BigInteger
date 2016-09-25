﻿#ifndef __FFT_H__
#define __FFT_H__

typedef double type;

typedef struct complex{
	type real;
	type imag;
}Complex;

#define PI 3.1415926535897932384626433832795028841971


void conjugate_complex(int n, complex in[], complex out[]);
void c_plus(complex a, complex b, complex *c);
void c_mul(complex a, complex b, complex *c);
void c_sub(complex a, complex b, complex *c);
void c_div(complex a, complex b, complex *c);
void fft(int N, complex f[]);
void ifft(int N, complex f[]);
void c_abs(complex f[], type out[], int n);

#endif