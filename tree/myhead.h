#ifndef MYHEAD_H
#define MYHEAD_H

// UCS2
typedef unsigned short wchar;
typedef unsigned char byte;

#if defined(WINDOWS) || defined(WIN32)
	#include <string.h>
	#include <wchar.h>
	#include <malloc.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <assert.h>
	#include <time.h>
	#include <stdio.h>
	#include <io.h>	// access function
	
	//UCS2
	inline size_t wstrlen(const wchar * string){
		return wcslen((const wchar_t*)string);
	}
	
	inline wchar * wstrcat(wchar * strDestination, const wchar * strSource){
		return (wchar*)wcscat((wchar_t*)strDestination, (const wchar_t*)strSource);
	}
	
	inline wchar * wstrncat(wchar * strDestination, const wchar * strSource, size_t count){
		return (wchar*)wcsncat((wchar_t*)strDestination, (const wchar_t*)strSource, count);
	}
	
	inline wchar * wstrncpy(wchar * strDestination, const wchar * strSource, size_t count){
		return (wchar*)wcsncpy((wchar_t*)strDestination, (const wchar_t*)strSource, count);
	}
	
	inline wchar * wstrdup(const wchar * strSource){
		return (wchar*)wcsdup((const wchar_t*) strSource);
	}
	
	//ISO8859-1
	#define strlen	strlen
	#define strcat	strcat
	#define strcpy	strcpy
	#define strcmp	strcmp
	#define stricmp	_stricmp
	#define strnicmp	_strnicmp
	#define	strdup	strdup
	#define strstr	strstr
	#define stat	_stat
	#define snprintf	_snprintf
	#define access	_access
	#define statb   	struct _stat

#elif defined(LINUX) || defined(UNIX)
	#include <string.h>
	#include <stdlib.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <assert.h>
	#include <time.h>
	#include <unistd.h>	// access function
	#include <stdio.h>	// sprintf function
	
	//UCS2
	size_t wstrlen(const wchar * string);
	wchar * wstrcat(wchar * strDestination, const wchar * strSource);
	wchar * wstrncat(wchar * strDestination, const wchar * strSource, size_t count);
	wchar * wstrncpy(wchar * strDestination, const wchar * strSource, size_t count);
	wchar * wstrdup(const wchar * string);
		
	//ISO8859-1
	#define strlen	strlen
	#define strcat	strcat
	#define strcpy	strcpy
	#define strcmp	strcmp
	#define stricmp	strcasecmp
	#define strnicmp	strncasecmp
	#define stat	stat
	#define snprintf	snprintf
	#define access	access
	#define statb	struct stat
#endif
	int Utf8ToUcs2(const byte * pUtf8, wchar * pUcs2Buf, size_t bufLen);
	int Ucs2ToUtf8(const wchar * pUcs2, byte * pUtf8Buf, size_t bufLen);
	
#endif
