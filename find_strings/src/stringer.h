#pragma once

#include <string>

#include "istringer.h"

class Stringer : public IStringer
{
public:
	Stringer() = delete;
	Stringer(vector<string> strings);

protected:
private:
};
