#ifndef CATALOG_H
#define CATALOG_H

#include <vector>

class Row
{
	public:
		Row(unsigned long nb);

		unsigned long size;
		bool* proved;
};

class Catalog
{
	public:
		Catalog();
		~Catalog();

		void addToProve(unsigned long num);
		unsigned long getToProve();

		void print();

	private:
		Row* eltProveds[4];
		int size;
};

#endif
