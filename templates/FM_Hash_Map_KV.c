#include <stdlib.h>

#include "../base.h"
#include "FM_Hash_Map<KV>.h"

static i32 coerce_idx(i32 idx, i32 capacity)
{
	if (0 < capacity)
	{
		i32 negative_allowed = idx % capacity;
		i32 as_positive = negative_allowed + capacity;
		return as_positive % capacity;
	}
	else
	{
		return -1;
	}
}

static i32 matching_offset(struct FM_Hash_Map<KV>* self, K target, i32 start)
{
	i32 capacity = self->capacity;
	i32* displacements = self->rh_displacements;
	K* keys = self->keys;
	bool (*eq)(K, K) = self->eq;

	if (start < 0 || capacity <= start)
	{
		return -1;
	}

	// Find the true idx (:idx + :offset) where k lives;
	for (i32 offset = 0; offset < capacity; offset++)
	{
		i32 i = (start + offset) % capacity;
		i32 disp = displacements[i];
		K key = keys[i];

		if (disp < offset)
		{
			return -1;
		}
		else if (offset == disp && eq(target, key))
		{
			return offset;
		}
	}

	return -1;
}

struct FM_Hash_Map<KV> FM_Hash_Map<KV>_sinit(i32 capacity, i32 (*hash)(K),
		bool (*eq)(K, K))
{
	if (capacity <= 0)
	{
		return (struct FM_Hash_Map<KV>) {0};
	}

	K* keys = (K*)calloc(capacity, sizeof(K));
	V* values = (V*)calloc(capacity, sizeof(V));
	i32* rh_disp = (i32*)calloc(capacity, sizeof(i32));

	for (i32 i = 0; i < capacity; i++)
	{
		rh_disp[i] = -1;
	}

	return (struct FM_Hash_Map<KV>) {
		.keys = keys,
		.values = values,
		.rh_displacements = rh_disp,
		.capacity = capacity,
		.length = 0,
		.hash = hash,
		.eq = eq
	};
}

V FM_Hash_Map<KV>_get(struct FM_Hash_Map<KV>* self, K k, V fallback)
{
	V* value = FM_Hash_Map<KV>_get_ref(self, k);

	if (value != NULL)
	{
		return *value;
	}
	else
	{
		return fallback;
	}
}

V* FM_Hash_Map<KV>_get_ref(struct FM_Hash_Map<KV>* self, K k)
{
	i32 idx = coerce_idx(self->hash(k), self->capacity);
	i32 offset = matching_offset(self, k, idx);

	if (offset < 0)
	{
		return NULL;
	}

	return self->values + ((idx + offset) % self->capacity);
}

bool FM_Hash_Map<KV>_put(struct FM_Hash_Map<KV>* self, K k, V v)
{
	K* keys = self->keys;
	V* values = self->values;
	i32* displacements = self->rh_displacements;
	i32 capacity = self->capacity;
	i32 length = self->length;
	bool (*eq)(K, K) = self->eq;

	i32 idx = coerce_idx(self->hash(k), self->capacity);

	if (idx < 0)
	{
		return false;
	}

	i32 offset = 0;
	i32 kd = 0;

	for (; offset < capacity; offset++)
	{
		i32 i = (idx + offset) % capacity;
		K key = keys[i];
		V value = values[i];
		i32 disp = displacements[i];

		if (disp < kd)
		{
			if (capacity <= length)
			{
				return false;
			}

			keys[i] = k;
			values[i] = v;
			displacements[i] = kd;

			if (disp < 0)
			{
				self->length++;

				return true;
			}
			else
			{
				k = key;
				v = value;
				kd = disp;
			}
		}
		else if (kd == disp && eq(key, k))
		{
			values[i] = v;

			return true;
		}

		kd++;
	}

	return false;
}

bool FM_Hash_Map<KV>_remove(struct FM_Hash_Map<KV>* self, K k)
{
	K* keys = self->keys;
	V* values = self->values;
	i32* displacements = self->rh_displacements;
	i32 capacity = self->capacity;

	i32 idx = coerce_idx(self->hash(k), capacity);
	i32 offset = matching_offset(self, k, idx);

	if (0 <= offset)
	{
		self->length--;
	}
	else
	{
		return false;
	}

	for (; offset + 1 < capacity; offset++)
	{
		i32 i_shift = (idx + offset) % capacity;
		i32 i_with = (idx + offset + 1) % capacity;
		i32 disp_with = displacements[i_with];

		if (disp_with <= 0)
		{
			displacements[i_shift] = -1;

			return true;
		}
		else
		{
			K k_with = keys[i_with];
			V v_with = values[i_with];

			keys[i_shift] = k_with;
			values[i_shift] = v_with;
			displacements[i_shift] = disp_with - 1;
		}
	}

	displacements[coerce_idx(idx - 1, capacity)] = -1;

	return true;
}
