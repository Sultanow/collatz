#include "catalog.h"
#include <iostream>
#include <cstring>
#include <stdlib.h>

Row::Row(unsigned long n)
{
	/*proved = new std::vector<bool>;
	proved->reserve(n);*/
	size = n;
	proved = (bool*) malloc(n * sizeof(bool*));
	memset(proved, false, n * sizeof(bool*));
}

unsigned long find2pow(unsigned long num)
{
	unsigned long result = 0;

	while(num!=1)
	{
		num = num>>1;
		result++;
	}
	return result;
}

Catalog::Catalog()
{
	size = 4;
	memset(eltProveds, 0, sizeof(std::vector<bool>*) * size);
}

Catalog::~Catalog()
{
}

void Catalog::addToProve(unsigned long num)
{
	if (num < 2)
		return;

	unsigned long pow2 = find2pow(num);
	unsigned long index = pow2 - 1;
	unsigned long index2 = num - (1 << pow2) - 1;

	std::cout << num << " " << pow2 << " " << index2 << "\n";

	Row* v = eltProveds[index];

	if (v == NULL)
	{
		v = new Row(1 << (pow2 - 1));
		this->eltProveds[index] = v;
	}

	if (!v->proved[index2])
	{
		v->proved[index2] = true;
	}
}

unsigned long Catalog::getToProve()
{
	return 0;
}

void Catalog::print()
{
	std::cout << "print:\n";
	for(int n = 0; n < 4; n++)
	{
		Row* r = eltProveds[n]; 

		std::cout << n << " : ";
		
		if (r != NULL) 
		for (int i=0; i < r->size; i++)
		{
			if(r->proved[i])
				std::cout << (1 << (n + 1)) + i + 1 << " ";
		}

		std::cout << "\n";
	};
	std::cout << '\n';
}


