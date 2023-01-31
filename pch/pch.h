// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "BulkParser.h"
#include "Color.h"
#include "CommonFunctions.h"
#include "CommonRegexes.h"
#include "ConvenientParser.h"
#include "ConverterVersion.h"
#include "Date.h"
#include "GameVersion.h"
#include "Localization/LocalizationBlock.h"
#include "Localization/LocalizationDatabase.h"
#include "Localization/LocalizationLambdas.h"
#include "Log.h"
#include "ModLoader/Mod.h"
#include "ModLoader/ModFilesystem.h"
#include "ModLoader/ModLoader.h"
#include "ModLoader/ModParser.h"
#include "OSCompatibilityLayer.h"
#include "Parser.h"
#include "ParserHelpers.h"
#include "StringUtils.h"
#include "iconvlite.h"
#include "targa.h"

#endif // PCH_H
