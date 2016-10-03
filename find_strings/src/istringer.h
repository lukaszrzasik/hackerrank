#pragma once

#include <string>

using namespace std;

class IStringer
{
public:
	virtual ~IStringer() {}

	virtual string operator[] (unsigned int query) = 0;

protected:
private:
};
