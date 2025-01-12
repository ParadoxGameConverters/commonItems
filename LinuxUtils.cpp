#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <algorithm>
#include <cerrno>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <filesystem>
#include <iconv.h>
#include <iostream>
#include <set>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>


namespace fs = std::filesystem;



void sprintf_s_Linux(char* __restrict __s, size_t __maxlength, const char* __restrict __format, ...)
{
	va_list arg_ptr;
	va_start(arg_ptr, __format);
	snprintf(__s, __maxlength, __format, arg_ptr);
	va_end(arg_ptr);
}


void strcpy_s_Linux(char* __restrict __dest, const char* __restrict __src)
{
	strcpy(__dest, __src);
}


int fopen_s_Linux(FILE** file, const char* filename, const char* mode)
{
	*file = fopen(filename, mode);
	return *file == NULL;
}


void fprintf_s_Linux(FILE* file, const char* format, ...)
{
	va_list arg_ptr;
	va_start(arg_ptr, format);
	fprintf(file, format, arg_ptr);
	va_end(arg_ptr);
}

#ifdef __linux__
HANDLE GetStdHandle(int nothing)
{
	return 1;
}
#endif

namespace commonItems
{

/*
	Helper function to determine that returns false if the filename is a:
	- a hidden folder or folder (starting with '.')
	- empty
	- a parent directory ('.')
	- the current directory ('..')
*/
bool IsRegularNodeName(const std::string& name)
{
	return !name.empty() && *name.begin() != '.';
}


/*
		returns false if a node name is empty or a placeholder for the current or parent directory, true otherwise
*/
bool IsActualNodeName(const std::string& name)
{
	for (auto i = name.begin(); i != name.end(); ++i)
	{
		if (*i != '.')
		{
			return true;
		}
	}
	return false;
}


const char* StripTrailingSeparators(const char* path, std::size_t length)
{
	switch (length)
	{
		case 0:
			return path;
		case 1:
			if (*path != '/')
			{
				++path;
			}
			return path;
		default:
			const char* i = path + length - 1;
			while (i != path && *i == '/')
			{
				--i;
			}
			++i;
			return i;
	}
}

const char* StripLeadingSeparators(const char* path)
{
	while (true)
	{
		if (*path != '/')
		{
			return path;
		}
		++path;
	}
}


/*
	Concatenates two paths and strips out leading and trailing '/'
*/
std::string ConcatenatePaths(const std::string& first, const std::string& second)
{
	const char* first_begin = first.c_str();
	const char* first_end = StripTrailingSeparators(first_begin, first.length());
	const char* second_begin = StripLeadingSeparators(second.c_str());
	return std::string(first_begin, first_end) + "/" + std::string(second_begin);
}

/*
	Concatenates a path and a valid node name
	A valid node name is a non empty string that does not contain '/'
	use ConcatenatePaths for other concatenations
*/
std::string ConcatenateNodeName(const std::string& path, const std::string& name)
{
	const char* path_begin = path.c_str();
	const char* path_end = StripTrailingSeparators(path_begin, path.length());
	return std::string(path_begin, path_end) + "/" + name;
}

/*
	Splits the node name from the rest of the path and returns both values as a pair (path, node_name)
*/
std::pair<std::string, std::string> SplitNodeNameFromPath(const std::string& path)
{
	if (path.empty())
	{
		return std::make_pair(std::string(), std::string());
	}
	const char* buffer = path.c_str();
	const char* end = buffer + path.length() - 1;
	while (end != buffer && *end == '/')
	{
		--end;
	}
	if (end == buffer)
	{
		return std::make_pair(std::string(buffer, buffer + 1), std::string());
	}
	const char* begin = end;
	++end;
	while (begin != buffer && *begin != '/')
	{
		--begin;
	}
	if (*begin == '/')
	{
		++begin;
	}

	return std::make_pair(std::string(buffer, begin), std::string(begin, end));
}

bool IsLinuxPathElementSeparator(char c)
{
	return c == '/';
}

bool isLinuxPathCharacter(char c)
{
	return c != '/';
}

char* CopyFolderPathElement(const char*& input_begin, const char* input_end, char* output)
{
	while (input_begin != input_end)
	{
		const char c = *input_begin;
		++input_begin;
		if (c == '/')
		{
			return output;
		}

		*output = c;
		++output;
	}
	return output;
}



bool GetFileMode(const std::string& path, mode_t& result)
{
	struct stat status;
	if (stat(path.c_str(), &status) != 0)
	{
		return false;
	}

	result = status.st_mode;
	return true;
}

bool IsRegularFile(const std::string& path)
{
	return !fs::is_directory(fs::u8path(path));
}

bool IsDirectory(const std::string& path)
{
	return fs::is_directory(fs::u8path(path));
}


void WriteToConsole(LogLevel level, const std::string& logMessage)
{
	if (level != LogLevel::Debug) // Don't log debug messages to console.
	{
		std::cout << logMessage;
	}
}

/*
	Implemented all messages for error codes used in LinuxUtils.cpp.
*/
std::string GetLastErrorString()
{
	return strerror(errno);
}

bool deleteFile(const std::string& unresolvedFile)
{
	fs::remove(fs::u8path(unresolvedFile));
	return true;
}


/*
 * Forward declarations for conversion helper function and types
 */
class ConversionInputBuffer;

class ConversionOutputBuffer;

bool ConvertString(const char* toCode, const char* fromCode, ConversionInputBuffer& in, ConversionOutputBuffer& out);

/*
 * helper class to copy buffer from string and keep track of remainder and current position
 */
class ConversionInputBuffer
{
	char* data;
	char* in_buffer;
	std::size_t remainder;

  public:
	explicit ConversionInputBuffer(const std::string& input): data(new char[input.length()]), in_buffer(data), remainder(input.length())
	{
		// POSIX iconv expects a pointer to char *, not to const char * and consequently does not guarantee that the input
		// sequence is not modified so we copy it into the buffer before attempting conversion
		std::copy(input.begin(), input.end(), data);
	}
	ConversionInputBuffer(const ConversionInputBuffer&) = delete;
	ConversionInputBuffer& operator=(const ConversionInputBuffer&) = delete;
	ConversionInputBuffer(ConversionInputBuffer&&) = delete;
	ConversionInputBuffer& operator=(ConversionInputBuffer&&) = delete;

	template <typename String> explicit ConversionInputBuffer(const String& input)
	{
		typedef typename String::value_type Char;
		remainder = sizeof(Char) * input.length();
		data = new char[remainder];
		in_buffer = data;
		const auto* const input_str = reinterpret_cast<const char*>(input.c_str());
		std::copy(input_str, input_str + remainder, data);
	}

	~ConversionInputBuffer() { delete[] data; }

	[[nodiscard]] bool has_remaining_bytes() const { return remainder != 0; }

	friend bool ConvertBuffer(const char* fromCode, const char* toCode, ConversionInputBuffer& from, ConversionOutputBuffer& to);
};

/*
 * helper class to manage buffer pointers and sizes while providing access to internal buffers for use in iconv
 * functions
 */
class ConversionOutputBuffer
{
	std::size_t size;
	std::size_t remainder;
	std::size_t block_size;
	char* data = nullptr;
	char* out_buffer = nullptr;

	template <typename String> struct OutputStrHelper
	{
		typedef typename String::value_type Char;

		static void str(String& output, const char* buffer, std::size_t length)
		{
			size_t output_length = length / sizeof(Char);
			wchar_t output_buffer[output_length];
			memset(output_buffer, 0, output_length * sizeof(wchar_t));
			std::mbstowcs(output_buffer, buffer, output_length);
			output.assign(output_buffer, output_length);
		}
	};

	template <typename Traits, typename Alloc> struct OutputStrHelper<std::basic_string<char, Traits, Alloc>>
	{
		static void str(std::basic_string<char, Traits, Alloc>& output, char* buffer, std::size_t length) { output.assign(buffer, length); }
	};

  public:
	explicit ConversionOutputBuffer(std::size_t initial_size = 0, std::size_t increment_block_size = 1024 * 1024):
		 size(initial_size), remainder(initial_size), block_size(increment_block_size)
	{
		if (size != 0)
		{
			data = new char[size];
			out_buffer = data;
		}
	}
	ConversionOutputBuffer(const ConversionOutputBuffer&) = delete;
	ConversionOutputBuffer& operator=(const ConversionOutputBuffer&) = delete;
	ConversionOutputBuffer(ConversionOutputBuffer&&) = delete;
	ConversionOutputBuffer& operator=(ConversionOutputBuffer&&) = delete;

	~ConversionOutputBuffer() { delete[] data; }

	[[nodiscard]] bool full() const { return remainder != 0; }

	[[nodiscard]] size_t length() const { return size - remainder; }

	[[nodiscard]] std::string str() const { return std::string(data, data + length()); }

	template <typename String> void str(String& output) const { OutputStrHelper<String>::str(output, data, length()); }

	bool ensure_capacity(std::size_t capacity)
	{
		if (size < capacity)
		{
			const size_t len = length();
			char* new_data = new char[capacity];
			std::copy(data, data + len, new_data);
			delete[] data;
			data = new_data;
			out_buffer = data + len;
			remainder += capacity - size;
			size = capacity;
			return true;
		}

		return false;
	}

	bool grow() { return ensure_capacity(size + block_size); }

	friend bool ConvertBuffer(const char* fromCode, const char* toCode, ConversionInputBuffer& from, ConversionOutputBuffer& to);
};

bool ConvertBuffer(const char* fromCode, const char* toCode, ConversionInputBuffer& from, ConversionOutputBuffer& to)
{
	iconv_t descriptor = iconv_open(toCode, fromCode);
	if (descriptor == reinterpret_cast<iconv_t>(-1))
	{
		LOG(LogLevel::Error) << "unable to recode string from '" << fromCode << "' to '" << toCode << ": not supported on this system";
		return false;
	}
	while (from.has_remaining_bytes())
	{
		if (iconv(descriptor, &from.in_buffer, &from.remainder, &to.out_buffer, &to.remainder) == static_cast<size_t>(-1))
		{
			switch (errno)
			{
				case E2BIG:
					to.grow();
					break;
				case EILSEQ:
					LOG(LogLevel::Error) << "invalid input sequence encountered during conversion of " << from.data << " from " << fromCode << " to " << toCode
												<< " : " << std::hex << *reinterpret_cast<uint16_t*>(from.in_buffer);
					return false;
				case EINVAL:
					LOG(LogLevel::Error) << "incomplete input sequence encountered during conversion from " << fromCode << " to " << toCode;
					return false;
				default:
					break;
			}
		}
	}
	iconv_close(descriptor);
	return true;
}

/*
 * InputString and OutputString should be instantiations of basic_string, or have a similar API
 * InputString should implement:
 * member type value_type
 * member function value_type begin() const
 * member function value_type end() const
 * member function size_t length() const
 *
 */
template <typename InputString, typename OutputString>
bool ConvertString(const char* fromCode, const char* toCode, const InputString& from, OutputString& to, std::size_t to_buffer_size = 0)
{
	if (to_buffer_size == 0)
	{
		to_buffer_size = from.length();
	}
	ConversionInputBuffer from_buffer(from);
	ConversionOutputBuffer to_buffer(to_buffer_size);
	if (ConvertBuffer(fromCode, toCode, from_buffer, to_buffer))
	{
		to_buffer.str(to);
		return true;
	}

	return false;
}

template <typename InputString, typename OutputString>
OutputString ConvertString(const char* fromCode, const char* toCode, const InputString& from, std::size_t to_buffer_size = 0)
{
	OutputString to;
	ConvertString(fromCode, toCode, from, to, to_buffer_size);
	return to;
}

std::string convertUTF8ToASCII(const std::string& UTF8)
{
	return ConvertString<std::string, std::string>("UTF-8", "ASCII//TRANSLIT", UTF8);
}

std::string convertUTF8To8859_15(const std::string& UTF8)
{
	return ConvertString<std::string, std::string>("UTF−8", "ISO−8859−15//TRANSLIT", UTF8);
}


std::string convertUTF8ToWin1252(const std::string& UTF8)
{
	return ConvertString<std::string, std::string>("UTF−8", "CP1252//TRANSLIT", UTF8);
}

std::string convertUTF8ToWin1251(const std::string& UTF8)
{
	return ConvertString<std::string, std::string>("UTF−8", "CP1251//TRANSLIT", UTF8);
}

std::string convertUTF8ToWin1250(const std::string& UTF8)
{
	return ConvertString<std::string, std::string>("UTF−8", "CP1250//TRANSLIT", UTF8);
}

std::string convert8859_15ToUTF8(const std::string& input)
{
	return ConvertString<std::string, std::string>("ISO−8859−15", "UTF-8//TRANSLIT", input);
}

/*
	Warning: Does not actually return an UTF-16 sequence.
	This is an implementation of the original Windows-based API which uses UTF-16 LE as the system dependent wchar_t
	encoding This behaviour is replicated on Linux but it uses the (system dependent) wchar_t encoding.
*/
std::wstring convert8859_15ToUTF16(const std::string& input)
{
	return ConvertString<std::string, std::wstring>("ISO−8859−15", "wchar_t//TRANSLIT", input);
}

std::string convertWin1250ToUTF8(const std::string& Win1250)
{
	return ConvertString<std::string, std::string>("CP1250", "UTF-8//TRANSLIT", Win1250);
}

std::string convertWin1251ToUTF8(const std::string& Win1251)
{
	return ConvertString<std::string, std::string>("CP1251", "UTF-8//TRANSLIT", Win1251);
}

std::string convertWin1252ToUTF8(const std::string& Win1252)
{
	return ConvertString<std::string, std::string>("CP1252", "UTF-8//TRANSLIT", Win1252);
}


std::wstring convertWin1252ToUTF16(const std::string& Win1252)
{
	return ConvertString<std::string, std::wstring>("CP1252", "wchar_t//TRANSLIT", Win1252);
}

/*
	Warning: Does not actually return an UTF-16 sequence.
	This is an implementation of the original Windows-based API which uses UTF-16 LE as the system dependent wchar_t
	encoding This behaviour is replicated on Linux but it uses the (system dependent) wchar_t encoding.
*/
std::wstring convertUTF8ToUTF16(const std::string& UTF8)
{
	std::vector<unsigned long> unicode;
	for (size_t i = 0; i < UTF8.size();)
	{
		uint32_t uni = '\0';
		size_t todo = 0;
		unsigned char ch = UTF8[i++];
		if (ch <= 0x7F)
		{
			uni = ch;
			todo = 0;
		}
		else if (ch <= 0xBF)
		{
			throw std::logic_error("not a UTF-8 string");
		}
		else if (ch <= 0xDF)
		{
			uni = ch & 0x1F;
			todo = 1;
		}
		else if (ch <= 0xEF)
		{
			uni = ch & 0x0F;
			todo = 2;
		}
		else if (ch <= 0xF7)
		{
			uni = ch & 0x07;
			todo = 3;
		}
		else
		{
			throw std::logic_error("not a UTF-8 string");
		}
		for (size_t j = 0; j < todo; ++j)
		{
			if (i == UTF8.size())
				throw std::logic_error("not a UTF-8 string");
			ch = UTF8[i++];
			if (ch < 0x80 || ch > 0xBF)
				throw std::logic_error("not a UTF-8 string");
			uni <<= 6;
			uni += ch & 0x3F;
		}
		if (uni >= 0xD800 && uni <= 0xDFFF)
			throw std::logic_error("not a UTF-8 string");
		if (uni > 0x10FFFF)
			throw std::logic_error("not a UTF-8 string");
		unicode.push_back(uni);
	}
	std::wstring utf16;
	for (size_t i = 0; i < unicode.size(); ++i)
	{
		unsigned long uni = unicode[i];
		if (uni <= 0xFFFF)
		{
			utf16 += static_cast<wchar_t>(uni);
		}
		else
		{
			uni -= 0x10000;
			utf16 += static_cast<wchar_t>((uni >> 10) + 0xD800);
			utf16 += static_cast<wchar_t>((uni & 0x3FF) + 0xDC00);
		}
	}
	return utf16;
}


std::string convertToUTF8(const std::wstring& input)
{
	return ConvertString<std::wstring, std::string>("wchar_t", "UTF-8//TRANSLIT", input);
}

std::optional<std::filesystem::path> getSteamInstallPath(const std::string& steamID)
{
	// TODO: Write some actual code for this when able.
	return std::nullopt;
}

} // namespace commonItems
