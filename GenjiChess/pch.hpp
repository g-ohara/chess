#ifndef PCH_H
#define PCH_H

#include <limits.h>
#include <ctime>
#include <iomanip>
#include <array>
#include <vector>
#include <queue>

#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <istream>
#include <algorithm>

#include <bitset>
#include <mutex>

#include <eigen3/Eigen/Dense>

#ifdef UNICODE
typedef std::wstring STRING;
#else
typedef std::string STRING;
#endif // UNICODE

#endif // PCH_H

#if defined(_WIN32) || defined(_WIN64)
#include <tchar.h>
#else
#include <wchar.h>
#ifdef UNICODE
typedef char TCHAR;
#define _T(str) L##str
#else
typedef wchar_t TCHAR;
#define _T(str) str
#endif
#endif

typedef int color_t;
typedef int name_t;
