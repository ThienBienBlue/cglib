struct FM_Hash_Map<KV>
{
	K* keys;
	V* values;
	int* rh_displacements;

	int capacity;
	int length;

	int (*hash)(K);
	bool (*eq)(K, K);
};

struct FM_Hash_Map<KV> FM_Hash_Map<KV>_init(int capacity, int (*hash)(K),
		bool (*eq)(K, K));

V FM_Hash_Map<KV>_get(struct FM_Hash_Map<KV>* self, K k, V fallback);

V* FM_Hash_Map<KV>_get_ref(struct FM_Hash_Map<KV>* self, K k);

bool FM_Hash_Map<KV>_put(struct FM_Hash_Map<KV>* self, K k, V v);

bool FM_Hash_Map<KV>_remove(struct FM_Hash_Map<KV>* self, K k);
