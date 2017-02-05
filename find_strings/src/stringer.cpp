#include "stringer.hpp"

std::string Stringer::operator[] (unsigned int query) const
{
	if (invalidated) {
		sorter.passAndSort(strings);
		invalidated = false;
	}
	return sorter.getSubstr(query);
}

void Stringer::addString(const std::string& w)
{
	strings.push_back(w);
	invalidated = true;
}

void Stringer::clearStrings()
{
	strings.clear();
	invalidated = true;
}
