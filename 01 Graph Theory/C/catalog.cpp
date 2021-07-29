#include "catalog.h"
#include <iostream>
#include <cstring>
#include <stdlib.h>

#include <bitset>

using namespace std;

TYPE find2pow(TYPE num) {
	TYPE result = 0;

	while(num != 0) {
		num = num >> 1;
		result++;
	}

	return result;
}

Catalog::Catalog(TYPE m, bool old = false)
{
	max = 1;
	max = max << m;
	size = m + 2;
	lastLine = 0;
	nbLine = 1;

	eltProven.assign(max << 1,false);
	eltProven[0] = true;
	nbProven.assign(size,0);

	if (old)
		numToProcess = new SetStep();
	else
		numToProcess = new SetBool(max >> 1);

	clock_gettime(CLOCK_REALTIME, &beginning);
}

Catalog::~Catalog()
{
}

void Catalog::add(TYPE num)
{
	TYPE n = num >> 1;
	if (eltProven[(n)])
		return;

	TYPE pow2 = find2pow(n);

	if(pow2 > (max << 2))
	{
		cout << "eltProvent to small" << endl;
		return;
	}

	eltProven[n] = true;
	nbProven[pow2 - 1]++;

	if (num < max)
		numToProcess->push(num);
}

TYPE Catalog::getToProcess()
{
	return numToProcess->pop();
}

void Catalog::check()
{
	if (nbProven[lastLine] == nbLine)
	{
		lastLine++;
		nbLine = nbLine << 1;
		printExpense();
		check();
	}
}

void Catalog::printExpense()
{
	TYPE t = 0;
	TYPE one = 1;
	struct timespec curTime;
	clock_gettime(CLOCK_REALTIME, &curTime);
	curTime.tv_sec -= beginning.tv_sec;
	if (curTime.tv_nsec < beginning.tv_nsec)
	{
		curTime.tv_sec -= 1;
		curTime.tv_nsec = 1000000000+curTime.tv_nsec-beginning.tv_nsec;
	}
	else
	{
		curTime.tv_nsec -= beginning.tv_nsec;
	}
	cout << lastLine + 1 << "expense," << (one << lastLine) - nbProven[lastLine] << "," << (curTime.tv_sec*1000000)+(curTime.tv_nsec/1000) << ",1";
	int i = 0;
	for(;i < lastLine; i++)
	{
		cout << "," << (1 << i);
	}

	for(; i < size; i++)
	{
		cout << "," << nbProven[i];
		t += nbProven[i];
	}
	cout << "," << t << endl;
}
