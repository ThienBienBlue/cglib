/**
 * Fixed Size Malloc Hash Map implemented with robin hood linear probing and
 * backshift.
 *
 * #include
 * - base.h
 */

struct FM_Hash_Map<KV>
{
	K* keys;
	V* values;
	i32* rh_displacements;

	i32 capacity;
	i32 length;

	i32 (*hash)(K);
	bool (*eq)(K, K);
};

function struct FM_Hash_Map<KV> FM_Hash_Map<KV>_sinit(i32 capacity,
		i32 (*hash)(K), bool (*eq)(K, K));

function V FM_Hash_Map<KV>_get(struct FM_Hash_Map<KV>* self, K k, V fallback);

function V* FM_Hash_Map<KV>_get_ref(struct FM_Hash_Map<KV>* self, K k);

function bool FM_Hash_Map<KV>_put(struct FM_Hash_Map<KV>* self, K k, V v);

function bool FM_Hash_Map<KV>_remove(struct FM_Hash_Map<KV>* self, K k);
