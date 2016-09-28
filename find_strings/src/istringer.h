#pragma once

#include <string>

class IStringer
{
public:
	virtual ~IStringer() {}

	string operator[] (unsigned int query) = 0;

protected:
private:
};
