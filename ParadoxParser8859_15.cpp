/*Copyright (c) 2018 The Paradox Game Converters Project

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

/*Copyright (c) 2010 Rolf Andreassen

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

#pragma warning(disable : 4348)	// suppress warnings from Spirit, because they aren't being fixed (or the fixes aren't being released)
#include "ParadoxParser8859_15.h"
#include <fstream>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/spirit/include/qi.hpp>
#include "Log.h"
#include "OSCompatibilityLayer.h"
using namespace boost::spirit;



namespace parser_8859_15
{
	void clearStack();

	static void setLHS(std::wstring key);
	static void pushObj();
	static void setRHSleaf(std::wstring val);
	static void setRHSobject();
	static void setRHSobjlist();
	static void setRHStaglist(std::vector<std::wstring> val);
	static void setEpsilon();
	static void setAssign();

	static std::shared_ptr<Object> topLevel;  // a top level object
	std::vector<std::shared_ptr<Object>>	stack;						// a stack of objects
	std::vector<std::shared_ptr<Object>>	objstack;					// a stack of objects
	bool					epsilon = false;		// if we've tried an episilon for an assign
	bool					inObjList = false;		// if we're inside an object list

	template <typename Iterator>
	struct SkipComment : qi::grammar<Iterator>
	{
		qi::rule<Iterator> comment;	// the rule for identiying comments

		SkipComment() : SkipComment::base_type(comment)
		{
			comment = qi::raw[qi::lexeme[lit("#") >> *(standard_wide::char_ - qi::eol)] >> -qi::eol];
		}
	};

	static bool debugme = false;	// whether or not debugging should be on
	template <typename Iterator>
	struct Parser : public qi::grammar<Iterator, SkipComment<Iterator> >
	{
		// leaf: either left or right side of assignment.  unquoted keyword.
		// example: leaf
		qi::rule<Iterator, std::wstring(), SkipComment<Iterator> >	leaf;

		// taglist: a grouping of anonymous (rhs) leaves or strings
		// examples: { TAG TAG TAG } or { "string" "string" TAG }
		qi::rule<Iterator, std::vector<std::wstring>(), SkipComment<Iterator> >	taglist;

		// assign: assignment
		// examples: lhs = rhs or lhs = { lhs = rhs }
		qi::rule<Iterator, SkipComment<Iterator> >	assign;

		// object: a grouping of assignments
		// example: object = { leaf = "string" }
		qi::rule<Iterator, SkipComment<Iterator> >	object;

		// objlist: a grouping of anonymous (rhs) objects
		// example: objlist = { { leaf = "string" } { leaf = leaf } }
		qi::rule<Iterator, SkipComment<Iterator> >	objlist;

		// str: a quoted literal string.  may include extended and/or reserved characters.
		// example: "I am a string."
		qi::rule<Iterator, std::wstring()>	str;

		// tolleaf: a tolerant leaf.  may include extended and other unreserved characters.  rhs only.
		// example: leaves with accents (names, for instance).
		qi::rule<Iterator, std::wstring(), SkipComment<Iterator> >	tolleaf;

		// braces: a stray set of empty rhs braces (without an lhs)
		// EU3 seems to do this for certain decision mods.
		qi::rule<Iterator, SkipComment<Iterator> >	braces;

		// root: evaluation starts here.  objects recurse to root.
		qi::rule<Iterator, SkipComment<Iterator> >	root;

		Parser() : Parser::base_type(root)
		{
			// { }
			braces = *(standard_wide::space) >> lit('{') >> *(standard_wide::space) >> lit('}') >> *(standard_wide::space);

			// a string enclosed in quotes
			str = lexeme[lit('"') >> raw[*(~standard_wide::char_('"') | lit(0x80) | lit(0x81) | lit(0x82) | lit(0x83) | lit(0x84) | lit(0x85) | lit(0x86) | lit(0x87) | lit(0x88) | lit(0x89) | lit(0x8A) | lit(0x8B) | lit(0x8C) | lit(0x8D) | lit(0x8E) | lit(0x8F) | lit(0x90) | lit(0x91) | lit(0x92) | lit(0x93) | lit(0x94) | lit(0x95) | lit(0x96) | lit(0x97) | lit(0x98) | lit(0x99) | lit(0x9A) | lit(0x9B) | lit(0x9C) | lit(0x9D) | lit(0x9E) | lit(0x9F) | lit(0x96) | lit('&') | lit('/'))] >> lit('"')];

			// a 'forgiving' string without quotes
			tolleaf = raw[+(~standard_wide::char_("\"{}= \t\r\n") | lit(0x80) | lit(0x81) | lit(0x82) | lit(0x83) | lit(0x84) | lit(0x85) | lit(0x86) | lit(0x87) | lit(0x88) | lit(0x89) | lit(0x8A) | lit(0x8B) | lit(0x8C) | lit(0x8D) | lit(0x8E) | lit(0x8F) | lit(0x90) | lit(0x91) | lit(0x92) | lit(0x93) | lit(0x94) | lit(0x95) | lit(0x96) | lit(0x97) | lit(0x98) | lit(0x99) | lit(0x9A) | lit(0x9B) | lit(0x9C) | lit(0x9D) | lit(0x9E) | lit(0x9F) | lit(0x96) | lit('&') | lit('/'))];

			// a strict string without quotes
			leaf = raw[+(standard_wide::alnum | standard_wide::char_("-._:") | lit(0x80) | lit(0x81) | lit(0x82) | lit(0x83) | lit(0x84) | lit(0x85) | lit(0x86) | lit(0x87) | lit(0x88) | lit(0x89) | lit(0x8A) | lit(0x8B) | lit(0x8C) | lit(0x8D) | lit(0x8E) | lit(0x8F) | lit(0x90) | lit(0x91) | lit(0x92) | lit(0x93) | lit(0x94) | lit(0x95) | lit(0x96) | lit(0x97) | lit(0x98) | lit(0x99) | lit(0x9A) | lit(0x9B) | lit(0x9C) | lit(0x9D) | lit(0x9E) | lit(0x9F) | lit(0x96) | lit('&') | lit(' / '))];

			// a list of strings within brackets
			taglist = lit('{') >> omit[*(standard_wide::space)] >> lexeme[((str | skip[tolleaf]) % *(standard_wide::space))] >> omit[*(standard_wide::space)] >> lit('}');

			// a root object contained within brackets
			object = raw[lit('{') >> *(root) >> *(standard_wide::space) >> lit('}')];

			// a list of objects contained within brackets
			objlist = raw[lit('{') >> *(*(standard_wide::space) >> object[&pushObj]) >> *(standard_wide::space) >> lit('}')];

			// an assignment. Left side is a string of some kind, right side is one of many allowed types
			assign = raw[(*(standard_wide::space) >> (leaf[&setLHS] | str[&setLHS] | eps[&setEpsilon]) >> *(standard_wide::space)
				>> lit('=')[&setAssign] >> *(standard_wide::space)
				>> (leaf[&setRHSleaf] | str[&setRHSleaf] | taglist[&setRHStaglist] | objlist[&setRHSobjlist] | object[&setRHSobject]) >> *(standard_wide::space))];

			// the root object (either an assignment, or something in braces)
			root = +(assign | braces);

			str.name("str");
			leaf.name("leaf");
			taglist.name("taglist");
			object.name("object");
			objlist.name("objlist");
			assign.name("assign");
			braces.name("braces");
			root.name("root");

			if (debugme)
			{
				debug(str);
				debug(leaf);
				debug(taglist);
				debug(object);
				debug(objlist);
				debug(assign);
				debug(braces);
				debug(root);
			}
		}
	};


	void initParser()
	{
		topLevel = std::make_shared<Object>("topLevel");
		epsilon = false;
	}

	std::wstring bufferOneObject(std::istream& read)
	{
		int openBraces = 0;				// the number of braces deep we are
		std::wstring currObject;				// the current object being built
		bool isTopLevel = true;			// whether or not we're at the top level
		while (read.good())
		{
			std::string buffer;
			getline(read, buffer);
			if (buffer.empty())
			{
				continue;
			}
			if (buffer.back() == '\r')
			{
				buffer.pop_back();
			}
			std::wstring wide_buffer = Utils::convert8859_15ToUTF16(buffer);

			if (wide_buffer == L"CK2txt")
			{
				continue;
			}
			else if (wide_buffer == L"EU4txt")
			{
				continue;
			}
			currObject += L"\n";

			bool opened = false;						// whether or not we just opened a new brace level
			bool isInLiteral = false;						// whether or not we're in a string literal
			const wchar_t* str = wide_buffer.c_str();	// a character string of the text under consideration
			const unsigned int strSize = wide_buffer.size();	// the size of the text under consideration
			for (unsigned int i = 0; i < strSize; ++i)
			{
				if ('"' == str[i])
				{
					isInLiteral = !isInLiteral;
				}
				if (isInLiteral)
				{
					currObject += str[i];
					continue;
				}
				if ('#' == str[i])
				{
					break;
				}
				if ('{' == str[i])
				{
					++openBraces;
					opened = true;
				}
				else if ('}' == str[i])
				{
					--openBraces;
				}
				currObject += str[i];
			}

			if (openBraces > 0)
			{
				isTopLevel = false;
				continue;
			}

			if (currObject == L"")
			{
				continue;
			}

			// Don't try to end an object that hasn't started properly;
			// accounts for such constructions as
			// object =
			// {
			// where openBraces is zero after the first line of the object.
			// Not a problem for non-top-level objects since these will have
			// nonzero openBraces anyway.
			// But don't continue if the object was all on one line.
			if (isTopLevel && !opened)
			{
				continue;
			}

			break;
		}
		return currObject;
	}

	bool readStream(std::istream& read)
	{
		clearStack();
		read.unsetf(std::ios::skipws);

		char firstChar = read.peek();
		if (firstChar == (char)0xEF)
		{
			char bitBucket[3];
			read.read(bitBucket, sizeof(bitBucket));
			LOG(LogLevel::Warning) << "Identified a BOM in a file that shouldn't be UTF-8";
		}

		/* - it turns out that the current implementation of spirit::istream_iterator is ungodly slow...
		static Parser<boost::spirit::istream_iterator> p;
		static SkipComment<boost::spirit::istream_iterator> s;

		boost::spirit::istream_iterator begin(read);
		boost::spirit::istream_iterator end;

		return qi::phrase_parse(begin, end, p, s);
		*/

		const static Parser<std::wstring::iterator> p;
		const static SkipComment<std::wstring::iterator> s;

		/* buffer and parse one object at a time */
		while (read.good())
		{
			std::wstring currObject = bufferOneObject(read);	// the object under consideration
			if (!qi::phrase_parse(currObject.begin(), currObject.end(), p, s))
			{
				clearStack();
				return false;
			}
		}

		clearStack();
		return true;
	}

	void clearStack()
	{
		if (!stack.empty())
		{
			Log logOutput(LogLevel::Warning);	// a section in the log file that won't automatically be broken into lines
			logOutput << "Clearing stack size " << stack.size() << " - this should not happen in normal operation\n";
			for (auto i : stack)
			{
				logOutput << *i << '\n';
			}
		}
		stack.clear();
	}

	void setLHS(std::wstring key)
	{
		//LOG(LogLevel::Debug) << "Setting LHS : " << key;

		std::shared_ptr<Object> p = std::make_shared<Object>(Utils::convertUTF16ToUTF8(key));
		if (0 == stack.size())
		{
			topLevel->setValue(p);
		}
		stack.push_back(p);
		epsilon = false;
	}

	void pushObj()
	{
		inObjList = true;
		//LOG(LogLevel::Debug) << "Pushing objlist";
		std::string key("objlist");			// the key of the object list
		std::shared_ptr<Object> p = std::make_shared<Object>(key);	// the object to hold the object list
		p->setObjList();
		objstack.push_back(p);
	}

	void setRHSleaf(std::wstring val)
	{
		//LOG(LogLevel::Debug) << "Setting RHSleaf : " << val;
		std::shared_ptr<Object> l = stack.back();	// the leaf object
		stack.pop_back();
		l->setValue(Utils::convertUTF16ToUTF8(val));
		if ((!inObjList) && (0 < stack.size()))
		{
			std::shared_ptr<Object> p = stack.back();	// the object holding the leaf
			p->setValue(l);
		}
		else if ((inObjList) && (0 < objstack.size()))
		{
			std::shared_ptr<Object> p = objstack.back();	// the object holding the leaf
			p->setValue(l);
		}
	}

	void setRHStaglist(std::vector<std::wstring> vals)
	{
		//LOG(LogLevel::Debug) << "Setting RHStaglist";
		std::vector<std::string> utf8Vals;
		for (auto val : vals)
		{
			utf8Vals.push_back(Utils::convertUTF16ToUTF8(val));
		}

		std::shared_ptr<Object> l = stack.back();	// the object holding the list
		stack.pop_back();
		l->addToList(utf8Vals.begin(), utf8Vals.end());
		if ((!inObjList) && (0 < stack.size()))
		{
			std::shared_ptr<Object> p = stack.back();	// the object holding the leaf
			p->setValue(l);
		}
		else if ((inObjList) && (0 < objstack.size()))
		{
			std::shared_ptr<Object> p = objstack.back();	// the object holding the leaf
			p->setValue(l);
		}
	}

	void setRHSobject()
	{
		//LOG(LogLevel::Debug) << "Setting RHSobject";
		// No value is set, a bunch of leaves have been defined.
		std::shared_ptr<Object> l = stack.back();
		stack.pop_back();
		if ((!inObjList) && (0 < stack.size()))
		{
			std::shared_ptr<Object> p = stack.back();	// the object holding the leaf
			p->setValue(l);
		}
		else if ((inObjList) && (0 < objstack.size()))
		{
			std::shared_ptr<Object> p = objstack.back();	// the object holding the leaf
			p->setValue(l);
		}
	}

	void setRHSobjlist()
	{
		inObjList = false;
		//LOG(LogLevel::Debug) << "Setting RHSobjlist";
		std::shared_ptr<Object> l = stack.back();	// the object
		l->setValue(objstack);
		objstack.clear();
		stack.pop_back();
		if (0 < stack.size())
		{
			std::shared_ptr<Object> p = stack.back(); // the other object
			p->setValue(l);
		}
	}

	void setEpsilon()
	{
		//LOG(LogLevel::Debug) << "Setting epsilon";
		epsilon = true;
	}

	void setAssign()
	{
		//LOG(LogLevel::Debug) << "In assign";
		if (epsilon)
		{
			std::shared_ptr<Object> e = std::make_shared<Object>("epsilon");
			stack.push_back(e);
		}
		epsilon = false;
	}

	std::shared_ptr<Object> doParseStream(std::istream& theStream)
	{
		/* - when using parser debugging, also ensure that the parser object is non-static!
		debugme = false;
		if (string(filename) == "D:/Victoria 2/technologies/commerce_tech.txt")
			debugme = true;
		*/

		initParser();
		readStream(theStream);

		return topLevel;
	}


	std::shared_ptr<Object> doParseFile(const std::string& filename)
	{
		std::ifstream read(filename);
		if (!read.is_open())
		{
			return {};
		}

		auto temp = doParseStream(read);

		read.close();
		read.clear();

		return temp;
	}
} // namespace parser_8859_15
