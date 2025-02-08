#pragma once

// Macro to convert the value of a variable to a string
#define STRINGSIZE2(s) #s
#define STRINGSIZE(s) STRINGSIZE2(s)

#define VER_FILE_VERSION            VERSION_MAJOR, VERSION_MINOR_YY, VERSION_MMDDR, 0
#define VER_FILE_VERSION_STR        STRINGSIZE(VERSION_MAJOR)        \
                                    "." STRINGSIZE(VERSION_MINOR_YY)    \
                                    "." STRINGSIZE(VERSION_MMDDR)    \
                                    "." STRINGSIZE(0)

#define VER_PRODUCT_VERSION         VER_FILE_VERSION
#define VER_PRODUCT_VERSION_STR     VER_FILE_VERSION_STR

#define WSTRING2(x) L# x
#define WSTRING(x) WSTRING2(x)

#define VER_PRODUCT_SEMVER_WSTR     WSTRING(VERSION_MAJOR)        \
                                    L"." WSTRING(VERSION_MINOR)    \
                                    L"." WSTRING(VERSION_YYMMDDRRR)
