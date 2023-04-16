#ifndef HASH_MAP_<KV>_H
#define HASH_MAP_<KV>_H

/**
 * Generated file for a generic HashMap.
 */
#include <stdlib.h>

struct HashMap<KV> {
	unsigned int capacity;
	unsigned int capacity_power_of_two;
	unsigned int (*hash)(K);
	K zero_key;
	K tombstone;
	K* keys;
	V* values;
};

struct HashMap<KV>* HashMap<KV>_init(unsigned int capacity, unsigned int (*hash)(K), K zero_key, K tombstone)
{
	unsigned int actual_capacity = 0b1;
	unsigned int actual_capacity_power_of_two = 1;
	while (actual_capacity < capacity)
	{
		actual_capacity = actual_capacity << 1;
		actual_capacity_power_of_two++;
	}

	K* keys = (K*)calloc(actual_capacity, sizeof(K));
	if (keys != NULL)
	{
		for (int idx = 0; idx < actual_capacity; idx++)
			keys[idx] = zero_key;
	}
	V* values = (V*)calloc(actual_capacity, sizeof(V));
	HashMap<KV>* retval = (HashMap<KV>*)malloc(sizeof(HashMap<KV>));
	if (retval != NULL)
	{
		retval->capacity = actual_capacity;
		retval->capacity_power_of_two = actual_capacity_power_of_two;
		retval->hash = hash;
		retval->zero_key = zero_key;
		retval->tombstone = tombstone;
		retval->keys = keys;
		retval->values = values;
	}

	if (keys == NULL || values == NULL || retval == NULL)
	{
		free(keys);
		free(values);
		free(retval);
		retval = NULL;
	}
	return retval;
}

#endif
