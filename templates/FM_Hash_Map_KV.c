#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "FM_Hash_Map<KV>.h"

static int coerce_idx(int idx, int capacity)
{
	if (0 < capacity)
	{
		int negative_allowed = idx % capacity;
		int as_positive = negative_allowed + capacity;
		return as_positive % capacity;
	}
	else
	{
		return -1;
	}
}

static int matching_offset(struct FM_Hash_Map<KV>* self, K target, int start)
{
	int capacity = self->capacity;
	int* displacements = self->rh_displacements;
	K* keys = self->keys;
	bool (*eq)(K, K) = self->eq;

	if (start < 0 || capacity <= start)
	{
		return -1;
	}

	// Find the true idx (:idx + :offset) where k lives;
	for (int offset = 0; offset < capacity; offset++)
	{
		int i = (start + offset) % capacity;
		int disp = displacements[i];
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

struct FM_Hash_Map<KV> FM_Hash_Map<KV>_init(int capacity, int (*hash)(K),
		bool (*eq)(K, K))
{
	if (capacity <= 0)
	{
		return (struct FM_Hash_Map<KV>) {0};
	}

	K* keys = (K*)calloc(capacity, sizeof(K));
	V* values = (V*)calloc(capacity, sizeof(V));
	int* rh_disp = (int*)calloc(capacity, sizeof(int));

	for (int i = 0; i < capacity; i++)
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
	int idx = coerce_idx(self->hash(k), self->capacity);
	int offset = matching_offset(self, k, idx);

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
	int* displacements = self->rh_displacements;
	int capacity = self->capacity;
	int length = self->length;
	bool (*eq)(K, K) = self->eq;

	int idx = coerce_idx(self->hash(k), self->capacity);

	if (idx < 0)
	{
		return false;
	}

	int offset = 0;
	int kd = 0;

	for (; offset < capacity; offset++)
	{
		int i = (idx + offset) % capacity;
		K key = keys[i];
		V value = values[i];
		int disp = displacements[i];

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
	int* displacements = self->rh_displacements;
	int capacity = self->capacity;

	int idx = coerce_idx(self->hash(k), capacity);
	int offset = matching_offset(self, k, idx);

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
		int i_shift = (idx + offset) % capacity;
		int i_with = (idx + offset + 1) % capacity;
		int disp_with = displacements[i_with];

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
