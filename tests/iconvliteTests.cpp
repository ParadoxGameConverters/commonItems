#include "../iconvlite.h"
#include "gtest/gtest.h"



char utf8Chars[][4] = {
	 "\xd0\x82",	  // Ђ
	 "\xd0\x83",	  // Ѓ
	 "\xE2\x80\x9A", // ‚
	 "\xD1\x93",	  // ѓ
	 "\xE2\x80\x9E", // „
	 "\xE2\x80\xA6", // …
	 "\xE2\x80\xA0", // †
	 "\xE2\x80\xA1", // ‡

	 "\xE2\x82\xAC", // €
	 "\xE2\x80\xB0", // ‰
	 "\xD0\x89",	  // Љ
	 "\xE2\x80\xB9", // ‹
	 "\xD0\x8A",	  // Њ
	 "\xD0\x8C",	  // Ќ
	 "\xD0\x8B",	  // Ћ
	 "\xD0\x8F",	  // Џ

	 "\xD1\x92",	  // ђ
	 "\xE2\x80\x98", // ‘
	 "\xE2\x80\x99", // ’
	 "\xE2\x80\x9C", // “
	 "\xE2\x80\x9D", // ”
	 "\xE2\x80\xA2", // •
	 "\xE2\x80\x93", // –
	 "\xE2\x80\x94", // —

	 "",				  // nothing
	 "\xE2\x84\xA2", // ™
	 "\xD1\x99",	  // љ
	 "\xE2\x80\xBA", // ›
	 "\xD1\x9A",	  // њ
	 "\xD1\x9C",	  // ќ
	 "\xD1\x9B",	  // ћ
	 "\xD1\x9F",	  // џ

	 "\xC2\xA0", // NBSP
	 "\xD0\x8E", // Ў
	 "\xD1\x9E", // ў
	 "\xD0\x88", // Ј
	 "\xC2\xA4", // ¤
	 "\xD2\x90", // Ґ
	 "\xC2\xA6", // ¦
	 "\xC2\xA7", // §

	 "\xD0\x81", // Ё
	 "\xC2\xA9", // ©
	 "\xD0\x84", // Є
	 "\xC2\xAB", // «
	 "\xC2\xAC", // ¬
	 "\xC2\xAD", // SHY
	 "\xC2\xAE", // ®
	 "\xD0\x87", // Ї

	 "\xC2\xB0", // °
	 "\xC2\xB1", // ±
	 "\xD0\x86", // І
	 "\xD1\x96", // і
	 "\xD2\x91", // ґ
	 "\xC2\xB5", // µ
	 "\xC2\xB6", // ¶
	 "\xC2\xB7", // ·

	 "\xD1\x91",	  // ё
	 "\xE2\x84\x96", // №
	 "\xD1\x94",	  // є
	 "\xC2\xBB",	  // »
	 "\xD1\x98",	  // ј
	 "\xD0\x85",	  // Ѕ
	 "\xD1\x95",	  // ѕ
	 "\xD1\x97",	  // ї

	 "\xD0\x90", // А
	 "\xD0\x91", // Б
	 "\xD0\x92", // В
	 "\xD0\x93", // Г
	 "\xD0\x94", // Д
	 "\xD0\x95", // Е
	 "\xD0\x96", // Ж
	 "\xD0\x97", // З

	 "\xD0\x98", // И
	 "\xD0\x99", // Й
	 "\xD0\x9A", // К
	 "\xD0\x9B", // Л
	 "\xD0\x9C", // М
	 "\xD0\x9D", // Н
	 "\xD0\x9E", // О
	 "\xD0\x9F", // П

	 "\xD0\xA0", // Р
	 "\xD0\xA1", // С
	 "\xD0\xA2", // Т
	 "\xD0\xA3", // У
	 "\xD0\xA4", // Ф
	 "\xD0\xA5", // Х
	 "\xD0\xA6", // Ц
	 "\xD0\xA7", // Ч

	 "\xD0\xA8", // Ш
	 "\xD0\xA9", // Щ
	 "\xD0\xAA", // Ъ
	 "\xD0\xAB", // Ы
	 "\xD0\xAC", // Ь
	 "\xD0\xAD", // Э
	 "\xD0\xAE", // Ю
	 "\xD0\xAF", // Я

	 "\xD0\xB0", // а
	 "\xD0\xB1", // б
	 "\xD0\xB2", // в
	 "\xD0\xB3", // г
	 "\xD0\xB4", // д
	 "\xD0\xB5", // е
	 "\xD0\xB6", // ж
	 "\xD0\xB7", // з

	 "\xD0\xB8", // и
	 "\xD0\xB9", // й
	 "\xD0\xBA", // к
	 "\xD0\xBB", // л
	 "\xD0\xBC", // м
	 "\xD0\xBD", // н
	 "\xD0\xBE", // о
	 "\xD0\xBF", // п

	 "\xD1\x80", // р
	 "\xD1\x81", // с
	 "\xD1\x82", // т
	 "\xD1\x83", // у
	 "\xD1\x84", // ф
	 "\xD1\x85", // х
	 "\xD1\x86", // ц
	 "\xD1\x87", // ч

	 "\xD1\x88", // ш
	 "\xD1\x89", // щ
	 "\xD1\x8A", // ъ
	 "\xD1\x8B", // ы
	 "\xD1\x8C", // ь
	 "\xD1\x8D", // э
	 "\xD1\x8E", // ю
	 "\xd1\x8F", // я
};

char windows1252Chars[][3] = {"\x80",
	 "\x81",
	 "\x82",
	 "\x83",
	 "\x84",
	 "\x85",
	 "\x86",
	 "\x87",
	 "\x88",
	 "\x89",
	 "\x8A",
	 "\x8B",
	 "\x8C",
	 "\x8D",
	 "\x8E",
	 "\x8F",
	 "\x90",
	 "\x91",
	 "\x92",
	 "\x93",
	 "\x94",
	 "\x95",
	 "\x96",
	 "\x97",
	 "\x98",
	 "\x99",
	 "\x9A",
	 "\x9B",
	 "\x9C",
	 "\x9D",
	 "\x9E",
	 "\x9F",
	 "\xA0",
	 "\xA1",
	 "\xA2",
	 "\xA3",
	 "\xA4",
	 "\xA5",
	 "\xA6",
	 "\xA7",
	 "\xA8",
	 "\xA9",
	 "\xAA",
	 "\xAB",
	 "\xAC",
	 "\xAD",
	 "\xAE",
	 "\xAF",
	 "\xB0",
	 "\xB1",
	 "\xB2",
	 "\xB3",
	 "\xB4",
	 "\xB5",
	 "\xB6",
	 "\xB7",
	 "\xB8",
	 "\xB9",
	 "\xBA",
	 "\xBB",
	 "\xBC",
	 "\xBD",
	 "\xBE",
	 "\xBF",
	 "\xC0",
	 "\xC1",
	 "\xC2",
	 "\xC3",
	 "\xC4",
	 "\xC5",
	 "\xC6",
	 "\xC7",
	 "\xC8",
	 "\xC9",
	 "\xCA",
	 "\xCB",
	 "\xCC",
	 "\xCD",
	 "\xCE",
	 "\xCF",
	 "\xD0",
	 "\xD1",
	 "\xD2",
	 "\xD3",
	 "\xD4",
	 "\xD5",
	 "\xD6",
	 "\xD7",
	 "\xD8",
	 "\xD9",
	 "\xDA",
	 "\xDB",
	 "\xDC",
	 "\xDD",
	 "\xDE",
	 "\xDF",
	 "\xE0",
	 "\xE1",
	 "\xE2",
	 "\xE3",
	 "\xE4",
	 "\xE5",
	 "\xE6",
	 "\xE7",
	 "\xE8",
	 "\xE9",
	 "\xEA",
	 "\xEB",
	 "\xEC",
	 "\xED",
	 "\xEE",
	 "\xEF",
	 "\xF0",
	 "\xF1",
	 "\xF2",
	 "\xF3",
	 "\xF4",
	 "\xF5",
	 "\xF6",
	 "\xF7",
	 "\xF8",
	 "\xF9",
	 "\xFA",
	 "\xFB",
	 "\xFC",
	 "\xFD",
	 "\xFE",
	 "\xFF"};



TEST(iconvlite_Tests, CpToUtf1LeavesAsciiUnchanged)
{
	char in[] =
		 "\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29"
		 "\x30\x31"
		 " !\"#$%&\'()*+,-./0123456789:;<=>?"
		 "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
		 "`abcdefghijklmnopqrstuvwxyz{|}~\x7f";

	char out[256];

	cp2utf1(out, in);

	ASSERT_EQ(0, strcmp(in, out));
}


TEST(iconvlite_Tests, CpToUtf1Converts1252)
{
	char out[4];
	for (unsigned int i = 0; i < 128; i++)
	{
		out[0] = 0x0;
		out[1] = 0x0;
		out[2] = 0x0;
		cp2utf1(out, windows1252Chars[i]);

		ASSERT_STREQ(utf8Chars[i], out);
	}
}


TEST(iconvlite_Tests, CpToUtfLeavesAsciiStringUnchanged)
{
	const auto in = std::string(
		 "\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29"
		 "\x30\x31"
		 " !\"#$%&\'()*+,-./0123456789:;<=>?"
		 "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
		 "`abcdefghijklmnopqrstuvwxyz{|}~\x7f");

	const auto out = cp2utf(in);

	ASSERT_EQ(in, out);
}


TEST(iconvlite_Tests, CpToUtfConverts1252String)
{
	std::string in;
	for (const auto& windows1252Char: windows1252Chars)
	{
		in += windows1252Char;
	}

	const auto out = cp2utf(in);

	std::string expected;
	for (const auto& utf8Char: utf8Chars)
	{
		expected += utf8Char;
	}

	ASSERT_EQ(expected, out);
}


TEST(iconvlite_Tests, convert_utf8_to_windows1251LeavesAsciiUnchanged)
{
	char in[] =
		 "\x1\x2\x3\x4\x5\x6\x7\x8\x9\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29"
		 "\x30\x31"
		 " !\"#$%&\'()*+,-./0123456789:;<=>?"
		 "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
		 "`abcdefghijklmnopqrstuvwxyz{|}~\x7f";

	char out[256];

	const auto converted = convert_utf8_to_windows1251(in, out, sizeof in);

	ASSERT_TRUE(converted);
	ASSERT_EQ(0, strcmp(in, out));
}


TEST(iconvlite_Tests, convert_utf8_to_windows1251Converts1252)
{
	char out[4];
	for (unsigned int i = 0; i < 128; i++)
	{
		if (static_cast<unsigned char>(utf8Chars[i][0]) >= 0xe0)
		{
			continue; // the conversion routine explicitly doesn't handle e0 and up
		}
		if (i == 0x18)
		{
			continue; // it's unreasonable to expect the empty one to convert
		}

		out[0] = 0x0;
		out[1] = 0x0;
		out[2] = 0x0;
		out[3] = 0x0;
		const auto converted = convert_utf8_to_windows1251(utf8Chars[i], out, strlen(utf8Chars[i]));

		ASSERT_TRUE(converted);
		ASSERT_STREQ(windows1252Chars[i], out);
	}
}


TEST(iconvlite_Tests, utf2cpLeavesAsciiStringUnchanged)
{
	const auto in = std::string(
		 "\x1\x2\x3\x4\x5\x6\x7\x8\x9\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29"
		 "\x30\x31"
		 " !\"#$%&\'()*+,-./0123456789:;<=>?"
		 "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
		 "`abcdefghijklmnopqrstuvwxyz{|}~\x7f");

	const auto out = utf2cp(in);

	ASSERT_EQ(in, out);
}


TEST(iconvlite_Tests, utf2cpConverts1252String)
{
	std::string in;
	std::string expected;

	for (unsigned int i = 0; i < 128; i++)
	{
		if (static_cast<unsigned char>(utf8Chars[i][0]) >= 0xe0)
		{
			continue; // the conversion routine explicitly doesn't handle e0 and up
		}
		if (i == 0x18)
		{
			continue; // it's unreasonable to expect the empty one to convert
		}

		in += utf8Chars[i];
		expected += windows1252Chars[i];
	}

	const auto out = utf2cp(in);

	ASSERT_EQ(expected, out);
}