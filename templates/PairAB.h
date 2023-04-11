#ifndef PAIR_<AB>_H_
#define PAIR_<AB>_H_

/**
 * Generated code for a Pair.
 */

struct Pair<AB>
{
	A first;
	B second;
};

struct Pair<AB> Pair<AB>_make(A v1, B v2)
{
	struct Pair<AB> retval;
	retval.first = v1;
	retval.second = v2;
	return retval;
}

#endif  // PAIR_<AB>_H_
