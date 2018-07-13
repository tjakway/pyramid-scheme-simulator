#pragma once

//stringification macros
//see https://stackoverflow.com/questions/2653214/stringification-of-a-macro-value
//DON'T name it str() or there WILL be name conflicts!
//any name that no one would write in a regular C/C++ file will do...
#define XSTRINGIFY_MACRO__(a) STRINGIFY_MACRO__(a)
#define STRINGIFY_MACRO__(a) #a
