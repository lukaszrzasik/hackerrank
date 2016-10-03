#include "stringer.h"

Stringer::Stringer(const vector<string> & strings)
{
	for (string s : strings)
	{
		switch (s.size() % 3)
		{
			case 0 : s += '0' + '0' + '0';
			case 1 : s += '0' + '0';
			case 2 : s += '0';
		}
		m_combinedString += s;
	}
}

string Stringer::operator[] (unsigned int query)
{
}
