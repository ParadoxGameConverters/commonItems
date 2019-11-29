/*Copyright (c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

#include "StringUtils.h"

namespace stringutils {

std::string remQuotes(const std::string& in)
{
	int len = in.size();

	if (len < 2)
	{
		return in;
	}

	if (in[0] != '"' || in[len - 1] != '"')
	{
		return in;
	}

	return in.substr(1, len - 2);
}

std::string addQuotes(const std::string& in)
{
	int len = in.size();
	// If the string is empty, return a quoted empty string.
	if (len == 0)
	{
		return "\"\"";
	}
	// If the string is already quoted, return it.
	if (len > 1 && in[0] == '"' && in[len - 1] == '"')
	{
		return in;
	}

	std::string out(len + 2, '"');
	for (int i = 0; i < len; ++i)
	{
		out[i + 1] = in[i];
	}
	return out;
}
}
