#include "lib.h"

void insert(int* array, int size, int pos, int value)
{
	for (int i = size; i>=pos; i--)
	{
		array[i] = array[i-1];
	}

	array[pos-1] = value;
	
	return;
}
