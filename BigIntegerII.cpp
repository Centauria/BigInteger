// BigIntegerII.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//#define DEBUG

#define O 48
#define ILLEGAL 1
#define OVFLOW 2
#define UNDEFINED -1

#define NUMBER 3
#define NUMCHAR 4

#ifdef DEBUG
#define TLEN 40
#else
#define TLEN 250000
#endif


typedef struct number{
	int positive;
	char digit[TLEN];
	int len;
	int format;
}Number;

typedef struct setnode{
	Number lhs;
	Number rhs;
	char op;
}SetNode;

char one[10][10] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
	{ 0, 2, 4, 6, 8, 0, 2, 4, 6, 8 },
	{ 0, 3, 6, 9, 2, 5, 8, 1, 4, 7 },
	{ 0, 4, 8, 2, 6, 0, 4, 8, 2, 6 },
	{ 0, 5, 0, 5, 0, 5, 0, 5, 0, 5 },
	{ 0, 6, 2, 8, 4, 0, 6, 2, 8, 4 },
	{ 0, 7, 4, 1, 8, 5, 2, 9, 6, 3 },
	{ 0, 8, 6, 4, 2, 0, 8, 6, 4, 2 },
	{ 0, 9, 8, 7, 6, 5, 4, 3, 2, 1 }
};
char ten[10][10] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 },
	{ 0, 0, 0, 0, 1, 1, 1, 2, 2, 2 },
	{ 0, 0, 0, 1, 1, 2, 2, 2, 3, 3 },
	{ 0, 0, 1, 1, 2, 2, 3, 3, 4, 4 },
	{ 0, 0, 1, 1, 2, 3, 3, 4, 4, 5 },
	{ 0, 0, 1, 2, 2, 3, 4, 4, 5, 6 },
	{ 0, 0, 1, 2, 3, 4, 4, 5, 6, 7 },
	{ 0, 0, 1, 2, 3, 4, 5, 6, 7, 8 }
};

void show_error(){
	printf("Error\n");
}
int is_zero(Number *number){
	switch (number->format){
	case NUMBER:
		if ((number->digit[0] == 0) && (number->len == 1)){
			return 1;
		}
		return 0;
	case NUMCHAR:
		if ((number->digit[0] == O) && (number->len == 1)){
			return 1;
		}
		return 0;
	}
	return UNDEFINED;
}
int clear(char *dig, int start, int end){
	memset(dig + start, 0, end - start);
	return 0;
}
int transform_numchar_to_number(Number *number){
	int i = 0;
	int temp;
	char *dig = number->digit;
	int len = number->len;
	for (; i < len / 2; i++){
		temp = dig[i] - O;
		dig[i] = dig[len - 1 - i] - O;
		dig[len - 1 - i] = temp;
	}
	if (len % 2 == 1){
		dig[len / 2] -= O;
	}
	for (i = len; i < TLEN; i++){
		dig[i] = 0;
	}
	i = len - 1;
	if (dig[i] == -3){
		number->positive = 0;
		dig[i] = 0;
		i--;
	}
	else if (dig[i] == -5){
		number->positive = 1;
		dig[i] = 0;
		i--;
	}
	else{
		number->positive = 1;
	}
	while (dig[i] == 0 && i > 0){
		i--;
	}
	number->len = i + 1;
	while (i >= 0){
		if (dig[i]<0 || dig[i]>9){
			return ILLEGAL;
		}
		i--;
	}
	number->format = NUMBER;
	return 0;
}
int transform_number_to_numchar(Number *number){
	int i = 0;
	int temp;
	char *dig = number->digit;
	int len = number->len;
	if (number->positive){
		for (; i < len / 2; i++){
			temp = dig[i] + O;
			dig[i] = dig[len - 1 - i] + O;
			dig[len - 1 - i] = temp;
		}
		if (len % 2 == 1){
			dig[len / 2] += O;
		}
		dig[len] = 0;
	}
	else{
		len++;
		for (; i < len / 2; i++){
			temp = dig[i] + O;
			dig[i] = dig[len - 1 - i] + O;
			dig[len - 1 - i] = temp;
		}
		if (len % 2 == 1){
			dig[len / 2] += O;
		}
		dig[0] = '-';
		dig[len] = 0;
		number->len = len;
	}
	number->format = NUMCHAR;
	return 0;
}
int transform_number_to_complex(char *hs, int len_hs, Complex *comp, int len_comp){
	int i;
	if (len_hs < len_comp){
		for (i = 0; i < len_hs; i++){
			comp[i].real = hs[i];
			comp[i].imag = 0;
		}
		for (i = len_hs; i < len_comp; i++){
			comp[i].real = 0;
			comp[i].imag = 0;
		}
	}
	else{
		for (i = 0; i < len_comp; i++){
			comp[i].real = hs[i];
			comp[i].imag = 0;
		}
	}
	return 0;
}
int* transform_complex_to_intarray_fix_return(Complex *comp, int len_comp){
	int i;
	int *res = new int[len_comp];
	for (i = 0; i < len_comp; i++){
		res[i] = (int)(comp[i].real + 0.5);
	}
	return res;
}
int* transform_complex_to_intarray_round_return(Complex *comp, int len_comp){
	int i;
	int *res = new int[len_comp];
	for (i = 0; i < len_comp; i++){
		res[i] = (int)comp[i].real;
	}
	return res;
}
int transform_intarray_to_number(int *intarray, int len_int, char *dest,int &dl){
	int i;
	int j;
	int k;
	int c;
	int maxlen = 0;
	int *temp = new int[TLEN];
	if (len_int>TLEN){
		return OVFLOW;
	}
	for (i = 0; i < len_int; i++){
		temp[i] = intarray[i];
	}
	for (i = len_int; i < TLEN; i++){
		temp[i] = 0;
	}
	for (i = 0; i < len_int; i++){
		c = temp[i];
		dest[i] = c % 10;
		c /= 10;
		j = 1;
		while (c>0){
			k = i + j;
			if (k >= TLEN){
				return OVFLOW;
			}
			else if (k >= len_int){
				temp[len_int] += c;
				c = 0;
			}
			else{
				temp[k] += (c % 10);
				c /= 10;
				j++;
			}
		}
	}
	c = temp[len_int];
	dest[i] = c % 10;
	c /= 10;
	while (c > 0){
		i++;
		if (i >= TLEN){
			return OVFLOW;
		}
		else{
			dest[i] = c % 10;
			c /= 10;
			i++;
		}
	}
	while (dest[i] == 0){
		i--;
	}
	dl = i + 1;
	for (i = dl; i < TLEN; i++){
		dest[i] = 0;
	}
	delete temp;
	return 0;
}
int input_numchar(Number *number, char* input){
	number->format = NUMCHAR;
	strcpy(number->digit, input);
	number->len = strlen(number->digit);
	return 0;
}
int show_number(Number *number){
	if (number->format == NUMBER){
		transform_number_to_numchar(number);
	}
	printf("%s\n", number->digit);
	return 0;
}
int show_div_mod(Number *div, Number *mod){
	if (div->format == NUMBER){
		transform_number_to_numchar(div);
	}
	if (mod->format == NUMBER){
		transform_number_to_numchar(mod);
	}
	printf("%s %s\n", div->digit, mod->digit);
	return 0;
}
int print_number(Number *number,FILE *file){
	if (number->format == NUMBER){
		transform_number_to_numchar(number);
		fputs(number->digit, file);
		fputc('\n', file);
		transform_numchar_to_number(number);
	}
	else if (number->format == NUMCHAR){
		fputs(number->digit, file);
		fputc('\n', file);
	}
	else{
		return ILLEGAL;
	}
	return 0;
}
int print_set(SetNode *node, FILE *file){
	print_number(&node->lhs, file);
	print_number(&node->rhs, file);
	fputc(node->op, file);
	fputc('\n', file);
	return 0;
}
/*function internal_compare:
lhs: 左操作数组
rhs: 右操作数组
ll: 左数有效长度
rl: 右数有效长度
os: 偏移量
返回1表示左数大，-1表示右数大，0表示一样大。
*/
int internal_compare(char *lhs, char *rhs, int ll, int rl, int os){
	if (ll > rl + os){
		return 1;
	}
	else if (ll < rl + os){
		return -1;
	}
	else{
		int i = ll - 1;
		if (os >= 0){
			for (; i >= os; i--){
				if (lhs[i]>rhs[i - os]){
					return 1;
				}
				else if (lhs[i] < rhs[i - os]){
					return -1;
				}
			}
		}
		else{
			for (; i >= -os; i--){
				if (lhs[i + os]>rhs[i]){
					return 1;
				}
				else if (lhs[i + os] < rhs[i]){
					return -1;
				}
			}
		}
	}
	return 0;
}
/*function internal_add:
lhs: 左操作数组
rhs: 右操作数组
ll: 左数有效长度
rl: 右数有效长度
os: 偏移量
dest: 目标数组
dl: 结果有效长度
例：lhs=188509,rhs=335,ll=6,rl=3,os=2
则竖式为：
	   188509
	+   335
	------------
	   222009
	os=-2时：
	 188509
	+     335
	------------
	 18851235
当然存储时lhs数组为{9,0,5,8,8,1,0,0,...}低位在低下标。
*/
int internal_add(char *lhs, char *rhs, int ll, int rl, int os, char *dest, int &dl){
	int i = 0;
	char exc = 0;
	int mlen;
	if (os >= 0){
		mlen = ll > rl + os ? ll : rl + os;
		for (; i < mlen; i++){
			if (i<os){
				dest[i] = lhs[i] + exc;
			}
			else{
				dest[i] = lhs[i] + rhs[i - os] + exc;
			}
			exc = 0;
			while (dest[i]>9)
			{
				dest[i] -= 10;
				exc++;
			}
		}
	}
	else{
		mlen = ll - os > rl ? ll - os : rl;
		for (; i < mlen; i++){
			if (i<-os){
				dest[i] = rhs[i] + exc;
			}
			else{
				dest[i] = lhs[i + os] + rhs[i] + exc;
			}
			exc = 0;
			while (dest[i]>9)
			{
				dest[i] -= 10;
				exc++;
			}
		}
	}
	if (exc){
		dest[mlen] = exc;
		dl = mlen + 1;
	}
	else{
		dl = mlen;
	}
	return 0;
}
/*function internal_sub:
lhs: 左操作数组
rhs: 右操作数组
ll: 左数有效长度
rl: 右数有效长度
os: 偏移量
dest: 目标数组
dl: 结果有效长度
注意合法的输入必须满足lhs>rhs，否则结果不正确。
*/
int internal_sub(char *lhs, char *rhs, int ll, int rl, int os, char *dest, int &dl){
	int i = 0;
	char exc = 0;
	int mlen;
	if (os >= 0){
		mlen = ll > rl + os ? ll : rl + os;
		for (; i < mlen; i++){
			if (i<os){
				dest[i] = lhs[i] - exc;
			}
			else{
				dest[i] = lhs[i] - rhs[i - os] - exc;
			}
			exc = 0;
			while (dest[i]<0)
			{
				dest[i] += 10;
				exc++;
			}
		}
	}
	else{
		mlen = ll - os > rl ? ll - os : rl;
		for (; i < mlen; i++){
			if (i<-os){
				dest[i] = rhs[i] - exc;
			}
			else{
				dest[i] = lhs[i + os] - rhs[i] - exc;
			}
			exc = 0;
			while (dest[i]<0)
			{
				dest[i] += 10;
				exc++;
			}
		}
	}
	if (exc){
		return ILLEGAL;
	}
	i = mlen - 1;
	while (dest[i] == 0 && i > 0){
		i--;
	}
	if (i == -1){
		dl = 1;
	}
	else{
		dl = i + 1;
	}
	return 0;
}
/*function internal_mul:
internal_mul_fft使用FFT算法，在位数较大时速度较快，
internal_mul_normal使用普通算法，位数较小时较快。
lhs: 左操作数组
rhs: 右操作数组
ll: 左数有效长度
rl: 右数有效长度
os: 偏移量
dest: 目标数组
dl: 结果有效长度
*/
int internal_mul_fft(char *lhs, char *rhs, int ll, int rl, int os, char *dest, int &dl){
	int i = 0;
	int mlen;
	int fft_n = 0;
	int fft_N;
	mlen = ll + rl;
	while (mlen){
		mlen >>= 1;
		fft_n++;
	}
	fft_N = 1 << fft_n;
	Complex *l_f = new Complex[fft_N];
	Complex *r_f = new Complex[fft_N];
	Complex *s_f = new Complex[fft_N];
	transform_number_to_complex(lhs, ll, l_f, fft_N);
	transform_number_to_complex(rhs, rl, r_f, fft_N);
	fft(fft_N, l_f);
	fft(fft_N, r_f);
	for (i = 0; i < fft_N; i++){
		c_mul(l_f[i], r_f[i], &s_f[i]);
	}
	ifft(fft_N, s_f);
	int* res_intarray = transform_complex_to_intarray_fix_return(s_f, fft_N);
	transform_intarray_to_number(res_intarray, fft_N, dest, dl);
	delete res_intarray;
	delete l_f;
	delete r_f;
	delete s_f;
	return 0;
}
int internal_mul_normal(char *lhs, char *rhs, int ll, int rl, int os, char *dest, int &dl){
	int i;
	int j;
	int k;
	clear(dest, 0, TLEN);
	for (i = 0; i < rl; i++){
		for (j = 0; j < ll; j++){
			k = i + j;
			dest[k] += one[lhs[j]][rhs[i]];
			dest[k + 1] += ten[lhs[j]][rhs[i]];
			while (dest[k]>9){
				dest[k] -= 10;
				dest[k + 1]++;
			}
		}
	}
	dl = i + j;
	while (dest[dl] == 0){
		dl--;
	}
	dl++;
	return 0;
}
/*function internal_div:
除法没有快速算法，只有普通算法，时间复杂度O(n^2)。
lhs: 左操作数组
rhs: 右操作数组
ll: 左数有效长度
rl: 右数有效长度
os: 偏移量
dest: 目标数组
dl: 结果有效长度
注意函数返回时lhs中即为余数，ll即为余数位数。
*/
int internal_div(char *lhs, char *rhs, int &ll, int rl, int os, char *dest, int &dl){
	int i;
	int ll_original = ll;
	int compare_result;
	clear(dest, 0, TLEN);
	for (i = ll - rl; i >= 0; i--){
		do{
			compare_result = internal_compare(lhs, rhs, ll, rl, i);
			switch (compare_result){
			case 0:
			case 1:
				internal_sub(lhs, rhs, ll, rl, i, lhs, ll);
				dest[i]++;
				break;
			case -1:
				break;
			}
		} while (compare_result != -1);
	}
	if (dest[ll_original - rl] == 0){
		dl = ll_original - rl;
	}
	else{
		dl = ll_original - rl + 1;
	}
	return 0;
}
Number* add(Number *lhs, Number *rhs){
	Number* res = new Number;
	res->format = NUMBER;
	if (lhs->format == NUMCHAR){
		transform_numchar_to_number(lhs);
	}
	if (rhs->format == NUMCHAR){
		transform_numchar_to_number(rhs);
	}
	if (lhs->positive^rhs->positive){
		switch (internal_compare(lhs->digit, rhs->digit, lhs->len, rhs->len, 0)){
		case 1:
			if (internal_sub(lhs->digit, rhs->digit, lhs->len, rhs->len, 0, res->digit, res->len)){
				delete res;
				return NULL;
			}
			res->positive = lhs->positive;
			break;
		case -1:
			if (internal_sub(rhs->digit, lhs->digit, rhs->len, lhs->len, 0, res->digit, res->len)){
				delete res;
				return NULL;
			}
			res->positive = rhs->positive;
			break;
		case 0:
			res->digit[0] = 0;
			res->len = 1;
			res->positive = 1;
			break;
		}
	}
	else{
		if (internal_add(lhs->digit, rhs->digit, lhs->len, rhs->len, 0, res->digit, res->len)){
			delete res;
			return NULL;
		}
		res->positive = lhs->positive;
	}
	return res;
}
Number* sub(Number *lhs, Number *rhs){
	Number* res = new Number;
	res->format = NUMBER;
	if (lhs->format == NUMCHAR){
		transform_numchar_to_number(lhs);
	}
	if (rhs->format == NUMCHAR){
		transform_numchar_to_number(rhs);
	}
	if (lhs->positive^rhs->positive){
		if (internal_add(lhs->digit, rhs->digit, lhs->len, rhs->len, 0, res->digit, res->len)){
			delete res;
			return NULL;
		}
		res->positive = lhs->positive;
	}
	else{
		switch (internal_compare(lhs->digit, rhs->digit, lhs->len, rhs->len, 0)){
		case 1:
			if (internal_sub(lhs->digit, rhs->digit, lhs->len, rhs->len, 0, res->digit, res->len)){
				delete res;
				return NULL;
			}
			res->positive = lhs->positive;
			break;
		case -1:
			if (internal_sub(rhs->digit, lhs->digit, rhs->len, lhs->len, 0, res->digit, res->len)){
				delete res;
				return NULL;
			}
			res->positive = !lhs->positive;
			break;
		case 0:
			res->digit[0] = 0;
			res->len = 1;
			res->positive = 1;
			break;
		}
	}
	return res;
}
Number* mul(Number *lhs, Number *rhs){
	Number *res = new Number;
	res->format = NUMBER;
	if (lhs->format == NUMCHAR){
		transform_numchar_to_number(lhs);
	}
	if (rhs->format == NUMCHAR){
		transform_numchar_to_number(rhs);
	}
	if (is_zero(lhs) || is_zero(rhs)){
		res->digit[0] = 0;
		res->len = 1;
		return res;
	}
	int minlen = lhs->len > rhs->len ? rhs->len : lhs->len;
	if (minlen < 400){
		internal_mul_normal(lhs->digit, rhs->digit, lhs->len, rhs->len, 0, res->digit, res->len);
	}
	else{
		internal_mul_fft(lhs->digit, rhs->digit, lhs->len, rhs->len, 0, res->digit, res->len);
	}
	if (lhs->positive^rhs->positive){
		res->positive = 0;
	}
	else{
		res->positive = 1;
	}
	return res;
}
Number* div(Number *lhs, Number *rhs){
	if (is_zero(rhs)){
		return NULL;
	}
	Number *res = new Number;
	res->format = NUMBER;
	if (lhs->format == NUMCHAR){
		transform_numchar_to_number(lhs);
	}
	if (rhs->format == NUMCHAR){
		transform_numchar_to_number(rhs);
	}
	if (internal_compare(lhs->digit, rhs->digit, lhs->len, rhs->len, 0) == -1){
		res->digit[0] = 0;
		res->len = 1;
		return res;
	}
	internal_div(lhs->digit, rhs->digit, lhs->len, rhs->len, 0, res->digit, res->len);
	if (lhs->positive^rhs->positive){
		res->positive = 0;
	}
	else{
		res->positive = 1;
	}
	return res;
}
Number* operate(SetNode *node){
	switch (node->op)
	{
	case '+':
		return add(&node->lhs, &node->rhs);
	case '-':
		return sub(&node->lhs, &node->rhs);
	case '*':
		return mul(&node->lhs, &node->rhs);
	case '/':
		return div(&node->lhs, &node->rhs);
	default:
		break;
	}
	return NULL;
}
int main(int argc, char* argv[])
{
	SetNode root;
	char input[TLEN];
	int state = 0;
	int error = 0;
	while (gets(input)){
		switch (state){
		case 0:
			error = 0;
			input_numchar(&root.lhs, input);
			if (transform_numchar_to_number(&root.lhs)){
				error = 1;
			}
			state = 1;
			break;
		case 1:
			input_numchar(&root.rhs, input);
			if (transform_numchar_to_number(&root.rhs)){
				error = 1;
			}
			state = 2;
			break;
		case 2:
			if (strlen(input) == 1){
				switch (input[0])
				{
				case '+':
				case '-':
				case '*':
				case '/':
					root.op = input[0];
					break;
				default:
					error = 1;
					break;
				}
			}
			else{
				error = 1;
			}
			state = 0;
		}
		if (state == 0){
			if (error){
				show_error();
			}
			else{
				Number* res = operate(&root);
				if (res != NULL){
					if (root.op == '/'){
						show_div_mod(res, &root.lhs);
					}
					else{
						show_number(res);
					}
					delete res;
				}
				else{
					show_error();
				}
			}
		}
	}
	return 0;
}

