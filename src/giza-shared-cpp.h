#include "giza-version.h"

#define GIZA_VERSION_STRINGIZE_(major, minor, micro)   \
        #major"."#minor"."#micro
#define GIZA_VERSION_STRINGIZE(major, minor, micro)    \
        GIZA_VERSION_STRINGIZE_(major, minor, micro)

#define GIZA_VERSION_STRING CAIRO_VERSION_STRINGIZE(   \
        GIZA_VERSION_MAJOR,                            \
        GIZA_VERSION_MINOR,                            \
        GIZA_VERSION_MICRO)

#define GIZA_LEFT_CLICK 'A'

#define GIZA_LS_SOLID      1
#define GIZA_LS_LONG_DASH  2
#define GIZA_LS_SHORT_DASH 3
#define GIZA_LS_DOT        4
#define GIZA_LS_DASH_DOT   5
#define GIZA_LS_DASH_DOT_DOT_DOT 6

#define GIZA_NUMBER_FORMAT_AUTO 0
#define GIZA_NUMBER_FORMAT_DEC  1
#define GIZA_NUMBER_FORMAT_EXP  2

#define GIZA_UNITS_NORMALIZED 0
#define GIZA_UNITS_INCHES 1
#define GIZA_UNITS_MM 2
#define GIZA_UNITS_PIXELS 3
#define GIZA_UNITS_WORLD 4
#define GIZA_UNITS_DEVICE 5

#define GIZA_BAND_NONE 0
#define GIZA_BAND_LINE 1
#define GIZA_BAND_RECTANGLE 2
#define GIZA_BAND_HORZLINES 3
#define GIZA_BAND_VERTLINES 4
#define GIZA_BAND_HORZLINE 5
#define GIZA_BAND_VERTLINE 6
#define GIZA_BAND_CROSSHAIR 7
#define GIZA_BAND_CIRCLE 8

#define GIZA_BACKGROUND_COLOUR 0
#define GIZA_BACKGROUND_COLOR 0
#define GIZA_FOREGROUND_COLOUR 1
#define GIZA_FOREGROUND_COLOR 1

#define GIZA_COLOUR_INDEX_MIN 0
#define GIZA_COLOUR_INDEX_MAX 271
