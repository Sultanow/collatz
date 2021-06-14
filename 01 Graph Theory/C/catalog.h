#ifndef CATALOG_H
#define CATALOG_H

#define TYPE size_t

#include <vector>
#include <set>

class Row
{
	public:
		Row(TYPE nb);
		~Row();

		bool get(TYPE);
		void set(TYPE);

		TYPE size;
		TYPE nbProveds;
		std::vector<bool> proved;
};

class Catalog
{
	public:
		Catalog(TYPE);
		~Catalog();

		void addNumProved(TYPE num);
		void addTab4NumProved(TYPE tab[4]);
		TYPE getToProsess();

		void check();

		void print();
		void printSet();
		void printExpense();

	private:
		Row** eltProveds;
		TYPE max;
		TYPE size;
		TYPE lastline;
		std::set<TYPE> numToProsess;
};

#endif
