#include "stdafx.h"


void conjugate_complex(int n, Complex in[], Complex out[])
{
	int i = 0;
	for (i = 0; i<n; i++)
	{
		out[i].imag = -in[i].imag;
		out[i].real = in[i].real;
	}
}

void c_abs(Complex f[], type out[], int n)
{
	int i = 0;
	type t;
	for (i = 0; i<n; i++)
	{
		t = f[i].real * f[i].real + f[i].imag * f[i].imag;
		out[i] = sqrt(t);
	}
}


void c_plus(Complex a, Complex b, Complex *c)
{
	c->real = a.real + b.real;
	c->imag = a.imag + b.imag;
}

void c_sub(Complex a, Complex b, Complex *c)
{
	c->real = a.real - b.real;
	c->imag = a.imag - b.imag;
}

void c_mul(Complex a, Complex b, Complex *c)
{
	c->real = a.real * b.real - a.imag * b.imag;
	c->imag = a.real * b.imag + a.imag * b.real;
}

void c_div(Complex a, Complex b, Complex *c)
{
	c->real = (a.real * b.real + a.imag * b.imag) / (b.real * b.real + b.imag * b.imag);
	c->imag = (a.imag * b.real - a.real * b.imag) / (b.real * b.real + b.imag * b.imag);
}

#define SWAP(a,b)  tempr=(a);(a)=(b);(b)=tempr

void Wn_i(int n, int i, Complex *Wn, char flag)
{
	Wn->real = cos(2 * PI*i / n);
	if (flag == 1)
		Wn->imag = -sin(2 * PI*i / n);
	else if (flag == 0)
		Wn->imag = -sin(2 * PI*i / n);
}


void fft(int N, Complex f[])
{
	Complex t, wn;
	int i, j, k, m, n, l, r, M;
	int la, lb, lc;
	
	for (i = N, M = 1; (i = i / 2) != 1; M++);
	
	for (i = 1, j = N / 2; i <= N - 2; i++)
	{
		if (i<j)
		{
			t = f[j];
			f[j] = f[i];
			f[i] = t;
		}
		k = N / 2;
		while (k <= j)
		{
			j = j - k;
			k = k / 2;
		}
		j = j + k;
	}

	
	for (m = 1; m <= M; m++)
	{
		la = 1<<m; 
		lb = la / 2;    
		
		
		for (l = 1; l <= lb; l++)
		{
			r = (l - 1)*(1<<(M - m));
			for (n = l - 1; n<N - 1; n = n + la) 
			{
				lc = n + lb;  
				Wn_i(N, r, &wn, 1);
				c_mul(f[lc], wn, &t);
				c_sub(f[n], t, &(f[lc]));
				c_plus(f[n], t, &(f[n]));
			}
		}
	}
}


void ifft(int N, Complex f[])
{
	int i = 0;
	conjugate_complex(N, f, f);
	fft(N, f);
	conjugate_complex(N, f, f);
	for (i = 0; i<N; i++)
	{
		f[i].imag = (f[i].imag) / N;
		f[i].real = (f[i].real) / N;
	}
}