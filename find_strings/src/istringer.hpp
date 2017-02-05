#pragma once

#include <string>

class IStringer
{
public:
	virtual std::string operator[] (const unsigned int query) const = 0;
	virtual void addString(const std::string& w) = 0;
	virtual void clearStrings() = 0;
};
