
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <algorithm>
#include <cstdarg>
#include <cstring>
#include <iostream>
#include <set>

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <iconv.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __linux__
#include <sys/sendfile.h>
#endif

#include <filesystem>
namespace fs = std::filesystem;

using namespace std;



void sprintf_s_Linux(char* __restrict __s, size_t __maxlen, const char* __restrict __format, ...)
{
	va_list argptr;
	va_start(argptr, __format);
	snprintf(__s, __maxlen, __format, argptr);
	va_end(argptr);
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
	va_list argptr;
	va_start(argptr, format);
	fprintf(file, format, argptr);
	va_end(argptr);
}

#ifdef __linux__
HANDLE GetStdHandle(int nothing)
{
	return 1;
}
#endif

namespace Utils
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
	using namespace std;
	using namespace std;
	for (string::const_iterator i = name.begin(); i != name.end(); ++i)
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
		char c = *path;
		if (c != '/')
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
	using namespace std;
	const char* first_begin = first.c_str();
	const char* first_end = StripTrailingSeparators(first_begin, first.length());
	const char* second_begin = StripLeadingSeparators(second.c_str());
	return string(first_begin, first_end) + "/" + string(second_begin);
}

/*
	Contatenates a path and a valid node name
	A valid node name is a non empty string that does not contain '/'
	use ConcatenatePaths for other concatenations
*/
std::string ConcatenateNodeName(const std::string& path, const std::string& name)
{
	using namespace std;
	const char* path_begin = path.c_str();
	const char* path_end = StripTrailingSeparators(path_begin, path.length());
	return string(path_begin, path_end) + "/" + name;
}

/*
	Splits the node name from the rest of the path and returns both values as a pair (path, nodename)
*/
std::pair<std::string, std::string> SplitNodeNameFromPath(const std::string& path)
{
	using namespace std;
	if (path.empty())
	{
		return make_pair(string(), string());
	}
	const char* buffer = path.c_str();
	const char* end = buffer + path.length() - 1;
	while (end != buffer && *end == '/')
	{
		--end;
	}
	if (end == buffer)
	{
		return make_pair(string(buffer, buffer + 1), string());
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

	return make_pair(string(buffer, begin), string(begin, end));
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
		else
		{
			*output = c;
			++output;
		}
	}
	return output;
};



bool GetFileMode(const std::string& path, mode_t& result)
{
	using namespace std;
	struct stat status;
	if (stat(path.c_str(), &status) != 0)
	{
		return false;
	}
	else
	{
		result = status.st_mode;
		return true;
	}
}

bool IsRegularFile(const std::string& path)
{
	return !fs::is_directory(fs::u8path(path));
}

bool IsDirectory(const std::string& path)
{
	return fs::is_directory(fs::u8path(path));
}


/*
	Note: since the function signature did not allow for a return value, it clears the fileNames set when an
	error occurs to make sure no operations are done on an incomplete list of files
*/
void GetAllFilesInFolderRecursive(const std::string& path, std::set<std::string>& filenames)
{
	for (auto& p: fs::recursive_directory_iterator(fs::u8path(path)))
	{
		if (!p.is_directory())
		{
			auto lastSlash = p.path().native().find_last_of("/");
			auto tempDir = p.path().native().substr(0, lastSlash);
			lastSlash = tempDir.find_last_of("/");
			auto dirName = tempDir.substr(lastSlash + 1, tempDir.length());
			auto returnName = "/" + dirName + "/" + p.path().filename().string();
			filenames.insert(returnName);
		}
	}
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
	using namespace std;
	switch (errno)
	{
		case EEXIST:
			return string("Error code: EEXIST");
		case EACCES:
			return string("Error code: EACCES");
		case ENOENT:
			return string("Error code: ENOENT");
		case ENOTDIR:
			return string("Error code: ENOTDIR");
		case EPERM:
			return string("Error code: EPERM");
		case EBUSY:
			return string("Error code: EBUSY");
		case ENOTEMPTY:
			return string("Error code: ENOTEMPTY");
		case EINVAL:
			return string("Error code: EINVAL");
		case EISDIR:
			return string("Error code: EISDIR");
		case EROFS:
			return string("Error code: EROFS");
		case E2BIG:
			return string("Error code: E2BIG");
		case EILSEQ:
			return string("Error code: EILSEQ");
		default:
			return string("Error code: unknown");
	}
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
	explicit ConversionInputBuffer(const std::string& input):
		 data(new char[input.length()]), in_buffer(data), remainder(input.length())
	{
		using namespace std;
		// POSIX iconv expects a pointer to char *, not to const char * and consequently does not guarantee that the input
		// sequence is not modified so we copy it into the buffer before attempting conversion
		copy(input.begin(), input.end(), data);
	};

	template <typename String> explicit ConversionInputBuffer(const String& input)
	{
		typedef typename String::value_type Char;
		using namespace std;
		remainder = sizeof(Char) * input.length();
		data = new char[remainder];
		in_buffer = data;
		const char* input_str = reinterpret_cast<const char*>(input.c_str());
		copy(input_str, input_str + remainder, data);
	};

	~ConversionInputBuffer() { delete[] data; };

	bool has_remaining_bytes() const { return remainder != 0; };

	friend bool ConvertBuffer(const char*, const char*, ConversionInputBuffer&, ConversionOutputBuffer&);

  private:
	// declared private to avoid copy of buffer
	// unsure if versions pre c++11  should be supported, if not, copy constructors can be explicitly deleted
	ConversionInputBuffer(const ConversionInputBuffer&);
	ConversionInputBuffer& operator=(const ConversionInputBuffer&);
};

/*
 * helper class to manage buffer pointers and sizes while providing access to internal buffers for use in iconv
 * functions
 */
class ConversionOutputBuffer
{
	std::size_t size;
	std::size_t block_size;
	std::size_t remainder;
	char* data = nullptr;
	char* out_buffer = nullptr;

	template <typename String> struct OutputStrHelper
	{

		typedef typename String::value_type Char;

		static void str(String& output, char* buffer, std::size_t length)
		{
			using namespace std;
			const Char* output_buffer = reinterpret_cast<const Char*>(buffer);
			size_t output_length = length / sizeof(Char);
			output.assign(output_buffer, output_length);
		};
	};

	template <typename Traits, typename Alloc> struct OutputStrHelper<std::basic_string<char, Traits, Alloc>>
	{

		static void str(std::basic_string<char, Traits, Alloc>& output, char* buffer, std::size_t length)
		{
			using namespace std;
			output.assign(buffer, length);
		};
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
	};

	~ConversionOutputBuffer() { delete[] data; };

	bool full() const { return remainder != 0; };

	size_t length() const { return size - remainder; };

	std::string str() const
	{
		using namespace std;
		return string(data, data + length());
	};

	template <typename String> void str(String& output) const { OutputStrHelper<String>::str(output, data, length()); };

	bool ensure_capacity(std::size_t capacity)
	{
		using namespace std;
		if (size < capacity)
		{
			size_t len = length();
			char* ndata = new char[capacity];
			copy(data, data + len, ndata);
			delete[] data;
			data = ndata;
			out_buffer = data + len;
			remainder += (capacity - size);
			size = capacity;
			return true;
		}
		else
		{
			return false;
		}
	};

	bool grow() { return ensure_capacity(size + block_size); };

	friend bool ConvertBuffer(const char*, const char*, ConversionInputBuffer&, ConversionOutputBuffer&);

  private:
	// declared private to avoid copy of buffer
	// unsure if versions pre c++11  should be supported, if not, copy constructors can be explicitly deleted
	ConversionOutputBuffer(const ConversionOutputBuffer&);
	ConversionOutputBuffer& operator=(const ConversionOutputBuffer&);
};

bool ConvertBuffer(const char* fromCode, const char* toCode, ConversionInputBuffer& from, ConversionOutputBuffer& to)
{
	using namespace std;
	iconv_t descriptor = iconv_open(toCode, fromCode);
	if (descriptor == ((iconv_t)(-1)))
	{
		LOG(LogLevel::Error) << "unable to recode string from '" << fromCode << "' to '" << toCode
									<< ": not supported on this system";
		return false;
	}
	while (from.has_remaining_bytes())
	{
		if (iconv(descriptor, &from.in_buffer, &from.remainder, &to.out_buffer, &to.remainder) == ((size_t)(-1)))
		{
			switch (errno)
			{
				case E2BIG:
					to.grow();
					break;
				case EILSEQ:
					LOG(LogLevel::Error) << "invalid input sequence encountered during conversion from " << fromCode
												<< " to " << toCode;
					return false;
				case EINVAL:
					LOG(LogLevel::Error) << "incomplete input sequence encountered during conversion from " << fromCode
												<< " to " << toCode;
					return false;
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
bool ConvertString(const char* fromCode,
	 const char* toCode,
	 const InputString& from,
	 OutputString& to,
	 std::size_t to_buffer_size = 0)
{
	using namespace std;
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
	else
	{
		return false;
	}
}

template <typename InputString, typename OutputString>
OutputString ConvertString(const char* fromCode,
	 const char* toCode,
	 const InputString& from,
	 std::size_t to_buffer_size = 0)
{
	using namespace std;
	OutputString to;
	ConvertString(fromCode, toCode, from, to, to_buffer_size);
	return to;
}

std::string convertUTF8ToASCII(const std::string& UTF8)
{
	using namespace std;
	return ConvertString<string, string>("UTF-8", "ASCII", UTF8);
}

std::string convertUTF8To8859_15(const std::string& UTF8)
{
	using namespace std;
	return ConvertString<string, string>("UTF−8", "ISO−8859−15", UTF8);
}


std::string convertUTF8ToWin1252(const std::string& UTF8)
{
	using namespace std;
	return ConvertString<string, string>("UTF−8", "CP1252", UTF8);
}


/*
	Warning: The input string should not be encoded in UTF-16 but in the system dependent wchar_t encoding
	see convertUTF8ToUTF16 for full explanation
*/
std::string convertUTF16ToUTF8(const std::wstring& UTF16)
{
	using namespace std;
	return ConvertString<wstring, string>("wchar_t", "UTF-8", UTF16);
}

std::string convert8859_15ToUTF8(const std::string& input)
{
	using namespace std;
	return ConvertString<string, string>("ISO−8859−15", "UTF-8", input);
}

/*
	Warning: Does not actually return an UTF-16 sequence.
	This is an implementation of the original Windows-based API which uses UTF-16 LE as the system dependent wchar_t
	encoding This behaviour is replicated on Linux but it uses the (system dependent) wchar_t encoding.
*/
std::wstring convert8859_15ToUTF16(const std::string& UTF8)
{
	using namespace std;
	return ConvertString<string, wstring>("ISO−8859−15", "wchar_t", UTF8);
}

std::string convertWin1250ToUTF8(const std::string& Win1252)
{
	using namespace std;
	return ConvertString<string, string>("CP1250", "UTF-8", Win1252);
}

std::string convertWin1251ToUTF8(const std::string& Win1252)
{
	using namespace std;
	return ConvertString<string, string>("CP1251", "UTF-8", Win1252);
}

std::string convertWin1252ToUTF8(const std::string& Win1252)
{
	using namespace std;
	return ConvertString<string, string>("CP1252", "UTF-8", Win1252);
}


std::wstring convertWin1252ToUTF16(const std::string& Win1252)
{
	using namespace std;
	return ConvertString<string, wstring>("CP1252", "wchar_t", Win1252);
}

/*
	Warning: Does not actually return an UTF-16 sequence.
	This is an implementation of the original Windows-based API which uses UTF-16 LE as the system dependent wchar_t
	encoding This behaviour is replicated on Linux but it uses the (system dependent) wchar_t encoding.
*/
std::wstring convertUTF8ToUTF16(const std::string& UTF8)
{
	using namespace std;
	return ConvertString<string, wstring>("UTF-8", "wchar_t", UTF8);
}


std::string convertToUTF8(const std::wstring& input)
{
	using namespace std;
	return ConvertString<wstring, string>("wchar_t", "UTF-8", input);
}

std::string normalizeUTF8Path(const std::string& utf_8_path)
{
	return utf_8_path;
};
} // namespace Utils
