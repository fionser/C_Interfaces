<<<<<<< HEAD
#include "arith.h"
int Arith_max(int x, int y)
{
        return x ^ ((x ^ y) & -(x < y));
}

int Arith_min(int x, int y)
{
        return y ^ ((x ^ y) & -(x < y));
}

int Arith_div(int x, int y)
{
	/*
	 *  (x < 0) != (y < 0) test if x and y is different sign.
	 *  And y is not divisible to x.
	 *  */
	if (((x < 0) != (y < 0)) && (x % y != 0))
		return x / y - 1;
	else
		return x / y;
}
/* 
 * According to the definition in C Language,
 * mod operation must equals to x - x/y * y when
 * x/y is displayable.
 * */
int Arith_mod(int x, int y)
{
	return x - Arith_div(x, y) * y;
}
/* 
 * In our definition, Arith_floor is equal to
 * Arith_div.
 * */
int Arith_floor(int x, int y)
{
	return Arith_div(x, y);
}
/* 
 * if y is divisible to x, return x / y.
 * else return  x / y + 1.
 * */
int Arith_ceiling(int x, int y)
{
	return Arith_div(x, y) + (x % y != 0);
}

=======
#include "arith.h"
int Arith_max(int x, int y)
{
	return x > y ? x : y;
}

int Arith_min(int x, int y)
{
	return x < y ? x : y;
}

int Arith_div(int x, int y)
{
	/*
	 *  (x < 0) != (y < 0) test if x and y is different sign.
	 *  And y is not divisible to x.
	 *  */
	if (((x < 0) != (y < 0)) && (x % y != 0))
		return x / y - 1;
	else
		return x / y;
}
/* 
 * According to the definition in C Language,
 * mod operation must equals to x - x/y * y when
 * x/y is displayable.
 * */
int Arith_mod(int x, int y)
{
	return x - Arith_div(x, y) * y;
}
/* 
 * In our definition, Arith_floor is equal to
 * Arith_div.
 * */
int Arith_floor(int x, int y)
{
	return Arith_div(x, y);
}
/* 
 * if y is divisible to x, return x / y.
 * else return  x / y + 1.
 * */
int Arith_ceiling(int x, int y)
{
	return Arith_div(x, y) + (x % y != 0);
}

>>>>>>> c6753ad4a4931db5714a4bacb26b73c7f914e394
