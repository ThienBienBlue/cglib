#include <assert.h>
#include <stdbool.h>

#include "../base.h"
#include "../generated/FM_Hash_Map_Int_Float.h"

i32 hash(int i)
{
	return i % 20;
}

bool eq(int a, int b)
{
	return a == b;
}

void assert_ikvd(struct FM_Hash_Map_Int_Float map, i32 idx, int k, float v, i32 d)
{
	assert(map.keys[idx] == k);
	assert(map.values[idx] == v);
	assert(map.rh_displacements[idx] == d);
}

void assert_get(struct FM_Hash_Map_Int_Float map, int k, i32 v)
{
	assert(FM_Hash_Map_Int_Float_get(&map, k, -1.0) == v);
}

void assert_remove(struct FM_Hash_Map_Int_Float* map, int k, bool success)
{
	assert(FM_Hash_Map_Int_Float_remove(map, k) == success);
}

int main()
{
	struct FM_Hash_Map_Int_Float map = FM_Hash_Map_Int_Float_sinit(10, hash, eq);

	assert(FM_Hash_Map_Int_Float_put(&map, 5, 5.0));
	assert(FM_Hash_Map_Int_Float_put(&map, 15, 15.0));
	assert(map.length == 2);

	assert_get(map, 5, 5.0);
	assert_ikvd(map, 5, 5, 5.0, 0);

	// Key clash with k=5, so k=15 will have a displacement of 1.
	assert_get(map, 15, 15.0);
	assert_ikvd(map, 6, 15, 15.0, 1);

	assert_get(map, 3, -1.0);

	assert(FM_Hash_Map_Int_Float_put(&map, 3, 3.0));
	assert(FM_Hash_Map_Int_Float_put(&map, 4, 4.0));

	// Displace k in {4, 5} with k=33.
	assert(FM_Hash_Map_Int_Float_put(&map, 33, 33.0));
	assert_ikvd(map, 3, 3, 3.0, 0);
	assert_ikvd(map, 4, 33, 33.0, 1);
	assert_ikvd(map, 5, 4, 4.0, 1);
	assert_ikvd(map, 6, 15, 15.0, 1);
	assert_ikvd(map, 7, 5, 5.0, 2);
	assert(map.rh_displacements[8] == -1);

	assert_get(map, 5, 5.0);
	assert_get(map, 3, 3.0);
	assert_get(map, 4, 4.0);
	assert_get(map, 15, 15.0);
	assert_get(map, 333, -1.0);

	assert(map.length == 5);

	// Test updates.
	assert(FM_Hash_Map_Int_Float_put(&map, 33, -33.0));
	assert(FM_Hash_Map_Int_Float_put(&map, 15, -15.0));

	assert_get(map, 5, 5.0);
	assert_get(map, 3, 3.0);
	assert_get(map, 15, -15.0);
	assert_get(map, 33, -33.0);

	assert_ikvd(map, 3, 3, 3.0, 0);
	assert_ikvd(map, 4, 33, -33.0, 1);
	assert_ikvd(map, 5, 4, 4.0, 1);
	assert_ikvd(map, 6, 15, -15.0, 1);
	assert_ikvd(map, 7, 5, 5.0, 2);

	// Test removes.
	assert_remove(&map, 33, true);
	assert_ikvd(map, 3, 3, 3.0, 0);
	assert_ikvd(map, 4, 4, 4.0, 0);
	assert_ikvd(map, 5, 15, -15.0, 0);
	assert_ikvd(map, 6, 5, 5.0, 1);
	assert(map.rh_displacements[7] == -1);

	assert_remove(&map, 5, true);
	assert_ikvd(map, 3, 3, 3.0, 0);
	assert_ikvd(map, 4, 4, 4.0, 0);
	assert_ikvd(map, 5, 15, -15.0, 0);
	assert(map.rh_displacements[6] == -1);
	assert(map.rh_displacements[7] == -1);

	assert_remove(&map, 3, true);
	assert(map.rh_displacements[3] == -1);
	assert_ikvd(map, 4, 4, 4.0, 0);
	assert_ikvd(map, 5, 15, -15.0, 0);
	assert(map.rh_displacements[6] == -1);
	assert(map.rh_displacements[7] == -1);

	assert_get(map, 33, -1.0);
	assert_get(map, 5, -1.0);
	assert_get(map, 3, -1.0);
	assert_get(map, 4, 4.0);
	assert_get(map, 15, -15.0);

	// Test invalid removes
	assert_remove(&map, 33, false);
	assert_remove(&map, 3, false);
	assert_remove(&map, 0, false);

	assert(map.rh_displacements[3] == -1);
	assert_ikvd(map, 4, 4, 4.0, 0);
	assert_ikvd(map, 5, 15, -15.0, 0);
	assert(map.rh_displacements[6] == -1);
	assert(map.rh_displacements[7] == -1);

	// Edge case of Map with space for a single element.
	struct FM_Hash_Map_Int_Float single = FM_Hash_Map_Int_Float_sinit(1, hash, eq);

	assert(FM_Hash_Map_Int_Float_put(&single, 25519, 25519.0));
	assert(FM_Hash_Map_Int_Float_put(&single, 3628800, 10.0) == false);
	assert_get(single, 25519, 25519.0);
	assert_remove(&single, 25519, true);
	assert_get(single, 25519, -1.0);
	assert_remove(&single, 25519, false);
	assert(FM_Hash_Map_Int_Float_put(&single, 3628800, 10.0));
	assert_get(single, 3628800, 10.0);

	// Test wrap around.
	struct FM_Hash_Map_Int_Float wrap = FM_Hash_Map_Int_Float_sinit(2, hash, eq);
	assert(FM_Hash_Map_Int_Float_put(&wrap, 11, 11.0));
	assert(FM_Hash_Map_Int_Float_put(&wrap, 21, 21.0));
	assert(FM_Hash_Map_Int_Float_put(&wrap, 31, 31.0) == false);

	assert_get(wrap, 11, 11.0);
	assert_get(wrap, 21, 21.0);
	assert_get(wrap, 31, -1.0);
	assert_ikvd(wrap, 1, 11, 11.0, 0);
	assert_ikvd(wrap, 0, 21, 21.0, 1);

	assert_remove(&wrap, 11, true);
	assert_remove(&wrap, 11, false);

	assert_get(wrap, 11, -1.0);
	assert_get(wrap, 21, 21.0);
	assert_ikvd(wrap, 1, 21, 21.0, 0);
	assert(wrap.rh_displacements[0] == -1);
}
