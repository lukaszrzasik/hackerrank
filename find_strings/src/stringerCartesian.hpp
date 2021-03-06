#pragma once

#include <vector>

#include "istringer.hpp"
#include "substrSorterCartesian.hpp"

class Stringer : public IStringer
{
public:
	virtual std::string operator[] (const unsigned int query) const;
	virtual void addString(const std::string& w);
	virtual void clearStrings();

private:
	std::vector<std::string> strings;
	mutable SubstrSorter sorter;
	mutable bool invalidated = true;
};
