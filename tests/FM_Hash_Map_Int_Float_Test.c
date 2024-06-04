#include <assert.h>
#include <stdbool.h>

#include "../generated/FM_Hash_Map_Int_Float.h"

int hash(int i)
{
	return i % 10;
}

bool eq(int a, int b)
{
	return a == b;
}

void assert_kvd(struct FM_Hash_Map_Int_Float map, int idx, int k, float v, int d)
{
	assert(map.keys[idx] == k);
	assert(map.values[idx] == v);
	assert(map.rh_displacements[idx] == d);
}

void assert_get(struct FM_Hash_Map_Int_Float map, int k, int v)
{
	assert(FM_Hash_Map_Int_Float_get(&map, k, -1) == v);
}

int main()
{
	struct FM_Hash_Map_Int_Float map = FM_Hash_Map_Int_Float_init(10, hash, eq);

	assert(FM_Hash_Map_Int_Float_put(&map, 5, 5.0));
	assert(FM_Hash_Map_Int_Float_put(&map, 15, 15.0));
	assert(map.length == 2);

	assert_get(map, 5, 5.0);
	assert_kvd(map, 5, 5, 5.0, 0);

	// Key clash with k=5, so k=15 will have a displacement of 1.
	assert_get(map, 15, 15.0);
	assert_kvd(map, 6, 15, 15.0, 1);

	assert_get(map, 3, -1);

	assert(FM_Hash_Map_Int_Float_put(&map, 3, 3.0));
	assert(FM_Hash_Map_Int_Float_put(&map, 4, 4.0));

	// Displace k in {4, 5}.
	assert(FM_Hash_Map_Int_Float_put(&map, 33, 33.0));
	assert_kvd(map, 4, 33, 33.0, 1);
	assert_kvd(map, 5, 4, 4.0, 1);
	assert_kvd(map, 6, 15, 15.0, 1);
	assert_kvd(map, 7, 5, 5.0, 2);
	assert(map.rh_displacements[8] == -1);

	assert_get(map, 5, 5.0);
	assert_get(map, 3, 3.0);
	assert_get(map, 4, 4.0);
	assert_get(map, 15, 15.0);

	assert(map.length == 5);

	// Test updates.
	assert(FM_Hash_Map_Int_Float_put(&map, 33, -33.0));
	assert(FM_Hash_Map_Int_Float_put(&map, 15, -15.0));

	assert_get(map, 5, 5.0);
	assert_get(map, 3, 3.0);
	assert_get(map, 15, -15.0);
	assert_get(map, 33, -33.0);
}
