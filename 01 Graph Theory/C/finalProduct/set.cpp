#include "set.h"
#include <omp.h>
#include <iostream>

#define OMP_NUM_THREADS 64

SetStep::SetStep()
{}

void SetStep::push(TYPE a)
{
	TYPE one = 1;
	if(a < one << 32)
		size1.insert((int) a);
	else
		size2.insert(a);
}

TYPE SetStep::pop()
{
	TYPE r = 0;
	if(size1.size() != 0)
	{
		r = *size1.begin();
		size1.erase(size1.begin());
	}
	else if(size2.size() != 0)
	{
		r = *size2.begin();
		size2.erase(size2.begin());
	}

	return r;
}

SetBool::SetBool(TYPE m)
{
	first = 0;
	last = 0;
	max = m;

	v.assign(max,false);
}

void SetBool::push(TYPE a)
{
	a = a >> 1;
	v[a] = true;

	if (a < first)
		first = a;
	else if (a > last)
		last = a;

	//std::cout << "push " << a << std::endl;
	//std::cout << "first " << first << std::endl;
	//std::cout << "last " << last << std::endl;
}

TYPE SetBool::pop()
{
	TYPE one = 1;
	TYPE index = 0;
	
	#pragma omp parallel reduction(min:index)
	{
		TYPE n = max/omp_get_num_threads();
		int id = omp_get_thread_num();
		TYPE start = n*id;
		TYPE stop = start+n;
		if (last<stop)
			stop = last+1;

		for(index=start; index<stop; index++)
		{
			if (v[index])
				break;
		}


		if (index >= stop)
			index = max;
	}
	if (index > last)
		return 0;

	first = index;
	v[index] = false;

	return (first << 1) + 1;
}
