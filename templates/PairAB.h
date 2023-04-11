#ifndef PAIR_<AB>_H_
#define PAIR_<AB>_H_

struct Pair<A, B>
{
	A v1;
	B v2;
};

struct Pair<A, B> Pair<AB>_make(A v1, B v2)
{
	struct Pair<A, B> retval;
	retval.v1 = v1;
	retval.v2 = v2;
	return retval;
}

#endif  // PAIR_<AB>_H_
