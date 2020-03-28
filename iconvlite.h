/*
iconvlite.h
Iconv Lite
Simple cpp functions to convert strings from cp1251 to utf8 and ftom utf8 to cp1251
*/

#ifndef ICONVLITE_H
#define ICONVLITE_H

static void cp2utf1(char *out, const char *in);
std::string cp2utf(std::string s);
int convert_utf8_to_windows1251(const char* utf8, char* windows1251, size_t n);
std::string utf2cp(std::string s);

typedef struct ConvLetter {
        char    win1251;
        int             unicode;
} Letter;

static Letter g_letters[] = {
        {static_cast<char>(0x82), 0x201A}, // SINGLE LOW-9 QUOTATION MARK
        {static_cast<char>(0x83), 0x0453}, // CYRILLIC SMALL LETTER GJE
        {static_cast<char>(0x84), 0x201E}, // DOUBLE LOW-9 QUOTATION MARK
        {static_cast<char>(0x85), 0x2026}, // HORIZONTAL ELLIPSIS
        {static_cast<char>(0x86), 0x2020}, // DAGGER
        {static_cast<char>(0x87), 0x2021}, // DOUBLE DAGGER
        {static_cast<char>(0x88), 0x20AC}, // EURO SIGN
        {static_cast<char>(0x89), 0x2030}, // PER MILLE SIGN
        {static_cast<char>(0x8A), 0x0409}, // CYRILLIC CAPITAL LETTER LJE
        {static_cast<char>(0x8B), 0x2039}, // SINGLE LEFT-POINTING ANGLE QUOTATION MARK
        {static_cast<char>(0x8C), 0x040A}, // CYRILLIC CAPITAL LETTER NJE
        {static_cast<char>(0x8D), 0x040C}, // CYRILLIC CAPITAL LETTER KJE
        {static_cast<char>(0x8E), 0x040B}, // CYRILLIC CAPITAL LETTER TSHE
        {static_cast<char>(0x8F), 0x040F}, // CYRILLIC CAPITAL LETTER DZHE
        {static_cast<char>(0x90), 0x0452}, // CYRILLIC SMALL LETTER DJE
        {static_cast<char>(0x91), 0x2018}, // LEFT SINGLE QUOTATION MARK
        {static_cast<char>(0x92), 0x2019}, // RIGHT SINGLE QUOTATION MARK
        {static_cast<char>(0x93), 0x201C}, // LEFT DOUBLE QUOTATION MARK
        {static_cast<char>(0x94), 0x201D}, // RIGHT DOUBLE QUOTATION MARK
        {static_cast<char>(0x95), 0x2022}, // BULLET
        {static_cast<char>(0x96), 0x2013}, // EN DASH
        {static_cast<char>(0x97), 0x2014}, // EM DASH
        {static_cast<char>(0x99), 0x2122}, // TRADE MARK SIGN
        {static_cast<char>(0x9A), 0x0459}, // CYRILLIC SMALL LETTER LJE
        {static_cast<char>(0x9B), 0x203A}, // SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
        {static_cast<char>(0x9C), 0x045A}, // CYRILLIC SMALL LETTER NJE
        {static_cast<char>(0x9D), 0x045C}, // CYRILLIC SMALL LETTER KJE
        {static_cast<char>(0x9E), 0x045B}, // CYRILLIC SMALL LETTER TSHE
        {static_cast<char>(0x9F), 0x045F}, // CYRILLIC SMALL LETTER DZHE
        {static_cast<char>(0xA0), 0x00A0}, // NO-BREAK SPACE
        {static_cast<char>(0xA1), 0x040E}, // CYRILLIC CAPITAL LETTER SHORT U
        {static_cast<char>(0xA2), 0x045E}, // CYRILLIC SMALL LETTER SHORT U
        {static_cast<char>(0xA3), 0x0408}, // CYRILLIC CAPITAL LETTER JE
        {static_cast<char>(0xA4), 0x00A4}, // CURRENCY SIGN
        {static_cast<char>(0xA5), 0x0490}, // CYRILLIC CAPITAL LETTER GHE WITH UPTURN
        {static_cast<char>(0xA6), 0x00A6}, // BROKEN BAR
        {static_cast<char>(0xA7), 0x00A7}, // SECTION SIGN
        {static_cast<char>(0xA8), 0x0401}, // CYRILLIC CAPITAL LETTER IO
        {static_cast<char>(0xA9), 0x00A9}, // COPYRIGHT SIGN
        {static_cast<char>(0xAA), 0x0404}, // CYRILLIC CAPITAL LETTER UKRAINIAN IE
        {static_cast<char>(0xAB), 0x00AB}, // LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
        {static_cast<char>(0xAC), 0x00AC}, // NOT SIGN
        {static_cast<char>(0xAD), 0x00AD}, // SOFT HYPHEN
        {static_cast<char>(0xAE), 0x00AE}, // REGISTERED SIGN
        {static_cast<char>(0xAF), 0x0407}, // CYRILLIC CAPITAL LETTER YI
        {static_cast<char>(0xB0), 0x00B0}, // DEGREE SIGN
        {static_cast<char>(0xB1), 0x00B1}, // PLUS-MINUS SIGN
        {static_cast<char>(0xB2), 0x0406}, // CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I
        {static_cast<char>(0xB3), 0x0456}, // CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
        {static_cast<char>(0xB4), 0x0491}, // CYRILLIC SMALL LETTER GHE WITH UPTURN
        {static_cast<char>(0xB5), 0x00B5}, // MICRO SIGN
        {static_cast<char>(0xB6), 0x00B6}, // PILCROW SIGN
        {static_cast<char>(0xB7), 0x00B7}, // MIDDLE DOT
        {static_cast<char>(0xB8), 0x0451}, // CYRILLIC SMALL LETTER IO
        {static_cast<char>(0xB9), 0x2116}, // NUMERO SIGN
        {static_cast<char>(0xBA), 0x0454}, // CYRILLIC SMALL LETTER UKRAINIAN IE
        {static_cast<char>(0xBB), 0x00BB}, // RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
        {static_cast<char>(0xBC), 0x0458}, // CYRILLIC SMALL LETTER JE
        {static_cast<char>(0xBD), 0x0405}, // CYRILLIC CAPITAL LETTER DZE
        {static_cast<char>(0xBE), 0x0455}, // CYRILLIC SMALL LETTER DZE
        {static_cast<char>(0xBF), 0x0457} // CYRILLIC SMALL LETTER YI
};

#endif
