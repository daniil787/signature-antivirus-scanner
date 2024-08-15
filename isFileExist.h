#ifndef _ISFILEEXIST_H_INCLUDED_
#define _ISFILEEXIST_H_INCLUDED_

#include <windows.h>

inline bool  isFileExist(const char* FileName) {
	return GetFileAttributesA(FileName) != DWORD(-1);
}

#endif
