#ifndef PAIR_<A, B>_H_
#define PAIR_<A, B>_H_

struct Pair<A, B>
{
	A v1;
	B v2;
};

struct Pair<A, B> Pair<A, B>_make(A v1, B v2)
{
	struct Pair<A><B> retval;
	retval.v1 = v1;
	retval.v2 = v2;
	return retval;
}

#endif  // PAIR_<A, B>_H_
