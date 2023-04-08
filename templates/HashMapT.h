/**
 * Generated file for a generic HashMap.
 */

#include <stdlib.h>

struct HashMap<T> {
	unsigned int capacity;
	unsigned int capacity_power_of_two;
	unsigned int* keys;
	T* values;
};

struct HashMap<T>* HashMap<T>_init(unsigned int capacity)
{
	unsigned int actual_capacity = 0b1;
	unsigned int actual_capacity_power_of_two = 1;
	while (actual_capacity < capacity)
	{
		actual_capacity = actual_capacity << 1;
		actual_capacity_power_of_two++;
	}

	unsigned int* keys = (unsigned int*)calloc(actual_capacity, sizeof(unsigned int));
	T* values = (T*)calloc(actual_capacity, sizeof(T));
	HashMap<T>* retval = (HashMap<T>*)malloc(sizeof(HashMap<T>));
	retval->capacity = actual_capacity;
	retval->capacity_power_of_two = actual_capacity_power_of_two;
	retval->keys = keys;
	retval->values = values;

	return retval;
}
