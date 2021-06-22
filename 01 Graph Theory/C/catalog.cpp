#include "catalog.h"
#include <iostream>
#include <cstring>
#include <stdlib.h>

#include <bitset>

using namespace std;

Row::Row(TYPE n)
{
	TYPE i = 1;
	size = i << n;
	nbProveds = 0;
	proved.assign(size,false);
}

Row::~Row()
{}

bool Row::get(TYPE i)
{
	return proved[i];
}

void Row::set(TYPE i)
{
	nbProveds++;
	proved[i] = true;
}

TYPE find2pow(TYPE num) {
	TYPE result = 0;

	while(num!=1) {
		num = num>>1;
		result++;
	}
	return result;
}

Catalog::Catalog(TYPE m)
{
	max = 1;
	max = max << m;
	size = m + 2 ;
	lastline = 1;
	eltProveds = (Row**) malloc(size * sizeof(Row*));
	for (TYPE i = 0; i < size; i++)
	{
		eltProveds[i] = new Row(i);
	}

	numToProcess.insert(3);
	addNumProved(5);
}

Catalog::~Catalog()
{
	for(TYPE i = 0; i < size; i++)
	{
		delete eltProveds[i];
	}
	free(eltProveds);
}

void Catalog::addNumProved(TYPE num)
{
	TYPE i = 1;
	if (num < (i << (lastline + 1)))
		return;

	TYPE pow2 = find2pow(num);
	TYPE index = pow2 - 1;
	TYPE index2 = (num - (1 << pow2)) >> 1;

	//std::cout << num << " " << pow2 << " " << index2 << "\n";

	if (index >= size - 1)
	{
		cout << "Bingo" << num << "\n";
		return;
	}

	Row* row = eltProveds[index];

	if (!row->get(index2))
	{
		row->set(index2);
		if (num < max)
			numToProcess.insert(num);
	}
}

void Catalog::addTab4NumProved(TYPE tab[4])
{
	addNumProved(tab[0]);
	addNumProved(tab[1]);
	addNumProved(tab[2]);
	addNumProved(tab[3]);
}

TYPE Catalog::getToProcess()
{
	if (numToProcess.size() == 0)
		return 0;

	TYPE r = *numToProcess.begin();
	//numToProcess.pop();
	numToProcess.erase(numToProcess.begin());
	return r;
}

void Catalog::check()
{
	Row* row = eltProveds[lastline];
	if (row != NULL && row->nbProveds == row->size)
	{
		delete row;
		eltProveds[lastline] = NULL;
		lastline++;
		printExpense();
		check();
	}
}

void Catalog::print()
{
	std::cout << "Catalog:\n";
	for(TYPE n = 0; n < size; n++)
	{
		Row* r = eltProveds[n];

		cout << n << " : ";

		if (r != NULL)
		{
			cout << r->nbProveds << " -> ";
			for (TYPE i=0; i < r->size; i++)
			{
				if(r->get(i))
					cout << (1 << (n + 1)) + (i * 2) + 1 << " ";
			}
		}

		cout << "\n";
	};
	cout << '\n';
}

void Catalog::printSet()
{
	cout << "set : " << numToProcess.size() << "\n";
	return;
	for (auto it = numToProcess.begin(); it != numToProcess.end(); it++)
	{
		cout << *it << ", ";
	}
	cout << "\n";
}

void Catalog::printExpense()
{
	TYPE t = 0;
	cout << lastline + 1 << "expense," << (1 << lastline) - eltProveds[lastline]->nbProveds << ",1";
	TYPE i = 0;
	for(;i < lastline; i++)
	{
		cout << "," << (1 << i);
	}
	for(; i < size; i++)
	{
		cout << "," << eltProveds[i]->nbProveds;
		t += eltProveds[i]->nbProveds;
	}
	cout << "," << t << "\n";

	//print();

}
