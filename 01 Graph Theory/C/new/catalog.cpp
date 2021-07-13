#include "catalog.h"
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>

#include <bitset>

#define OPENMP_NUM_THREADS 8

using namespace std;

Row::Row(TYPE n)
{
	TYPE i = 1;
	size = i << n;
	nbProven = 0;
	Proven.assign(size,false);
}

Row::~Row()
{}

bool Row::get(TYPE i)
{
	return Proven[i];
}

void Row::set(TYPE i, bool v = true)
{
	nbProven++;
	Proven[i] = v;
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
	lastline = 0;
	eltProven = (Row**) malloc(size * sizeof(Row*));
	for (TYPE i = 0; i < size; i++)
	{
		eltProven[i] = new Row(i);
	}

	toProcess.assign(max/2,false);

	addNumProven(3);
	addNumProven(5);
	beginning = time(NULL);
}

Catalog::~Catalog()
{
	for(TYPE i = 0; i < size; i++)
	{
		delete eltProven[i];
	}
	free(eltProven);
}

void Catalog::addNumProven(TYPE num)
{
	TYPE one = 1;
	if (num < (one << (lastline + 1)))
		return;

	TYPE pow2 = find2pow(num);
	TYPE index = pow2 - 1;
	TYPE index2 = (num - (one << pow2)) >> 1;

	//std::cout << num << " " << pow2 << " " << index2 << "\n";

	if (index >= size - 1)
	{
		cout << "Bingo" << num << "\n";
		return;
	}

	Row* row = eltProven[index];

	if (!row->get(index2))
	{
		row->set(index2);
		if (num < max)
		{
			//numToProcess.insert(num);
			toProcess[(num-1) >> 1] = true;
		}
	}
}

void Catalog::addTab4NumProven(TYPE tab[4])
{
	addNumProven(tab[0]);
	addNumProven(tab[1]);
	addNumProven(tab[2]);
	addNumProven(tab[3]);
}

TYPE Catalog::getToProcess()
{
	/*
	if (numToProcess.size() == 0)
		return 0;

	TYPE r = *numToProcess.begin();
	//numToProcess.pop();
	numToProcess.erase(numToProcess.begin());
	return r;
	*/

	TYPE one = 1;
	TYPE begin = (one<<(lastline))/2; //one<<lastline-1 <=> (one<<lastline) /2
	TYPE end = max/2;
	TYPE index = end;
	//printf("Variables initialisées.\n");
	//printf("Begin : %lld | End : %lld\n",begin,end);
	//printf("Index : %lld\n",index);

	#pragma omp parallel reduction(min:index) 
	{
		TYPE n = (end-begin)/omp_get_num_threads();
		int id = omp_get_thread_num();
		TYPE start = begin + n*id;
		TYPE stop = start + n;

		//printf("Start : %lld | Stop : %lld | Index : %lld\n",start,stop,index);

		for(index=start; index<stop; index++)
		{
			if (toProcess[index])
				break;
		}

		//printf("Post-boucle.\n");

		if (index >= stop)
			index = end;

		//printf("%d : %lld\n",id,index);
	}
	
	//printf("Valeur avant le if : %lld\n",index);
	if (index==end)
		index = 0;
	//printf("Valeur après le if : %lld\n",index);

	toProcess[index] = false;

	//printf("Pré-return.\n");
	return (index * 2) + 1;
}

void Catalog::check()
{
	Row* row = eltProven[lastline];
	if (row != NULL && row->nbProven == row->size)
	{
		delete row;
		eltProven[lastline] = NULL;
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
		Row* r = eltProven[n];

		cout << n << " : ";

		if (r != NULL)
		{
			cout << r->nbProven << " -> ";
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
	/*cout << "set : " << numToProcess.size() << "\n";
	return;
	for (auto it = numToProcess.begin(); it != numToProcess.end(); it++)
	{
		cout << *it << ", ";
	}
	cout << "\n";*/
}

void Catalog::printExpense()
{
	TYPE t = 0;
	cout << lastline + 1 << "expense," << (1 << lastline) - eltProven[lastline]->nbProven << "," << difftime(time(NULL),beginning) << ",1";
	TYPE i = 0;
	for(;i < lastline; i++)
	{
		cout << "," << (1 << i);
	}
	for(; i < size; i++)
	{
		cout << "," << eltProven[i]->nbProven;
		t += eltProven[i]->nbProven;
	}
	cout << "," << t << endl;

	//print();

}
