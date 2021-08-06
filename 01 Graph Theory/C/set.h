#ifndef SET_H
#define SET_H

#define TYPE unsigned long long

#include <set>
#include <vector>

class Set
{
	public:
		virtual void push(TYPE) = 0;
		virtual TYPE pop() = 0;
};

class SetStep : public Set
{
	public:
		SetStep();

		void push(TYPE);
		TYPE pop();

	private:
		std::set<unsigned int> size1;
		std::set<unsigned long> size2;

};

class SetBool : public Set
{
	public:
		SetBool(TYPE);

		void push(TYPE);
		TYPE pop();

	private:
		TYPE first;
		TYPE last;
		TYPE max;
		std::vector<bool> v;

};

#endif
