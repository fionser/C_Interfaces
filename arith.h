#ifndef ARITH_H
extern int Arith_max(int x, int y);
extern int Arith_min(int x, int y);
/**
 * When x and y is positive, Arith_div equals to
 * x/y. Otherwise Arith_div equals floor(x/y).
 */
extern int Arith_div(int x, int y);
extern int Arith_mod(int x, int y);
extern int Arith_ceiling(int x, int y);
extern int Arith_floor  (int x, int y);
#endif
