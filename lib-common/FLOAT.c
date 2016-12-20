#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {

	int s1 = a >> 31;
	int s2 = b >> 31;
	if(s1 != 0) a = -a;
	if(s2 != 0) b = -b;
	unsigned int a1 = (a >> 16),b1 = (b >> 16);
	unsigned int a0 = a & 0xffff,b0 = b & 0xffff;
	unsigned int c0,c1,c2,c3;
	c0 = a0 * b0;
	c1 = c0 / 0xffff + a0 * b1 + a1 * b0; c0 %= 0xffff;
	c2 = c1 / 0xffff + a1 * b1; c1 %= 0xffff;
	c3 = c2 / 0xffff; c2 %= 0xffff;
	int ans = c1 + (c2 << 16);
	if(s1 != s2) ans = -ans;
	return ans;

}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	unsigned int a00 = a << 16;
	unsigned int a01 = a >> 16;
	unsigned int a10 = a >> 31;
	unsigned int a11 = a >> 31;
	int ans = 0, i;
	for(i = 0; i < 64; ++i)
	{
		a11 = (a11 << 1) + (a10 >> 31);
		a10 = (a10 << 1) + (a01 >> 31);
		a01 = (a01 << 1) + (a00 >> 31);
		a00 = a00 << 1;
		ans = ans << 1;
		if(a11 > 0 || a10 >= b) {
			if(a10 < b) a11 --;
			a10 -= b;
			ans++; 
		}
	}
	return ans;
	
//	return (a << 8) / (b >> 8);
}

FLOAT f2F(float a) {
	int i, uf, m, e, s, ans;
	uf = *(int*)&a;
	m = uf & ((1 << 23) - 1);
	e = ((uf >> 23) & ((1 << 8) - 1)) - 127;
	s = uf >> 31;
	ans = 1;
	for(i = 1; i <= e + 16; ++ i) {
		ans = (ans << 1) + ((m & (1 << 22)) >> 22);
		if (ans < 0) return 0x80000000u;
		m = m << 1;
	}
	if (s != 0) ans = (~ans) + 1;
	return (FLOAT)(ans);
}

FLOAT Fabs(FLOAT a) {
	return a >=0 ? a : -a;
}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

