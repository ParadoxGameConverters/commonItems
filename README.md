# Common Items
Common items for the different game converters. Including parsers, OS utilities, and some common classes.

## Current Status
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/54be2f027eb040448b8bf89410c7ff3d)](https://www.codacy.com/gh/ParadoxGameConverters/commonItems/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=ParadoxGameConverters/commonItems&amp;utm_campaign=Badge_Grade)

## Included Items

### Color.h
Represents a Paradox-defined color.

Can be directly created in either the RGB or HSV color spaces.

Can be imported in:  
* Unspecified with ints (becomes RGB) - "= { 64 128 128 }"
* Unspecified with floats (becomes RGB) - "= { 0.5 0.9 0.1 }"
* RGB - "= rgb { 64 128 128 }"
* Hex - "= hex { 408080 }"
* HSV - "= hsv { 0.5 0.5 0.5 }"
* HSV360 - "= hsv360 { 180 50 50 }"
* Name (requires caching definitions for the named colors in advance) - "= dark_moderate_cyan"

Can be output in:  
* unspecified (rgb) - "= { 64 128 128 }"
* RGB - "= rgb { 64 128 128 }"
* hex - "= hex { 408080 }"
* HSV - "= hsv { 0.5 0.5 0.5 }"
* HSV360 - "= hsv360 { 180 50 50 }"

The individual components can be accessed in both RGB and HSV color spaces, equality and inequality can be checked, the color cache can be reviewed and modified, and colors can have a random fluctuation be applied automatically.

### CommonFunctions.h
A handful of helpful commonly-used functions.

##### trimPath
Given a file with path included (such as '/this/is/a/path.txt' or 'c:\this\is\a\path.txt'), returns the part that's just the filename ('path.txt')

##### trimExtension
Given a filename with an extension (such as 'file.extension' or 'file.name.with.extension'), returns the extension ('extension')

##### replaceCharacter
Given a string (such as 'a file name.eu4'), replaces all instances of the specified character (such as ' ') with underscores (resulting in 'a_file_name.eu4')

##### cardinalToOrdinal
Given a cardinal number (1, 2, 15), returns the equivalent ordinal word ending ('st', 'nd', 'th') for appending to the numbers ('1st', '2nd', '15th')

##### cardinalToRoman
Given a number (3, 12, 2020), returns the number in roman numerals ('III', 'XII', 'MMXX')

##### normalizeStringPath
Given a path, normalizes it in a standard way for all converters that all supported Paradox games will recognize (by replacing all spaces, dashes, and other weird characters (<, >, :, ?...) with underscores, and by converting entire string into ASCII)

### Date.h
A class representing a Paradox-style date.

##### Construction
 * Default construction gives a date of 0001-01-01
 * Can directly specify year, month, day
 * Can directly specify year, month, day, and if this is an AUC (years after the founding of Rome, used in Imperator) format date or not
 * Can pass a paradox-style string specifying the date
 * Can pass a paradox-style string specifying the date, and if this is an AUC (years after the founding of Rome, used in Imperator) format date or not
 
##### Comparison
Dates can be compared using all the standard comparators. Additionally, the difference between two dates (in years) can be found.

##### Modification
Dates can be increased by months or years, and can be decreased by years. In all cases thse must be whole-number changes to months or years.

##### Output
Dates can be output to a stream or converted to a string.

### GameVersion.h
A class and some helpers representing the version of a Paradox game. Assumes the version consists of four integers (1.12.4.5), but versions with fewer parts will work seamlessly.

#### GameVersion
The version class itself.

##### Construction
 * Default construction gives a version of 0.0.0.0
 * Can directly specify all four parts
 * Can construct via string - "1.2.3.4", "1.6.7", ""
 * Can construct via stream - "version = { first = 1 second = 2 third = 3 forth = 4 }"
   * The misspelling of 'fourth' is Paradox's
   
##### Comparison
GameVersions can be compared using all the standard comparators. It is a simple lexicographic comparison in order of the parts.

#### GameVersion::Factory
A factory is provided for an alternate constuction method. It only provides for construction via a stream.

#### Output
A freestanding output function allows writing a GameVersion to output streams.
 

### Log.h
A class to log information during conversion. Everything is logged to log.txt in the directory used to run the program. No configuration or setup is required, just include the header and build the .cpp file, then start logging.

The logger uses C++ stream semantics:  
```Log(LogLevel::Info) << "Message: " << variable;```

Log level specifies a message at the beginning of the logged line, and can be set to any of the following:  
 * LogLevel::Error  
 * LogLevel::Warning  
 * LogLevel::Info  
 * LogLevel::Debug  
 * LogLevel::Progress - this is used to set the progress bar in the Fronter

### OSCompatibilityLayer.h
Description coming soon.

### Parser.h
Description coming soon.

### ParserHelpers.h
Description coming soon.

### StringUtils.h
Description coming soon.

### iconvlite.h
Description coming soon.

### targa.h
Description coming soon.
