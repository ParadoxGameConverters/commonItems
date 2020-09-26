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
