typedef double type;

typedef struct complex{
	type real;
	type imag;
}Complex;

#define PI 3.1415926535897932384626433832795028841971


void conjugate_complex(int n, Complex in[], Complex out[]);
void c_plus(Complex a, Complex b, Complex *c);
void c_mul(Complex a, Complex b, Complex *c);
void c_sub(Complex a, Complex b, Complex *c);
void c_div(Complex a, Complex b, Complex *c);
void fft(int N, Complex f[]);
void ifft(int N, Complex f[]);
void c_abs(Complex f[], type out[], int n);
