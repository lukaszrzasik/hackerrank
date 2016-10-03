#pragma once

#include <vector>

#include "istringer.h"

class Stringer : public IStringer
{
public:
	Stringer() = delete;
	Stringer(const vector<string> & strings);

	virtual string operator[] (unsigned int query);

protected:
private:
	string m_combinedString;
};
