#include "stdio.h"

#if defined(WINDOWS) || defined(WIN32)


#elif defined(UNIX) || defined(LINUX)

size_t wstrlen(const wchar * string){
	size_t len = 0;
	if(string == 0 || *string == 0) return len;
	const wchar * pwch = string;
	while(*pwch != 0) {
		len ++, pwch++;
	}
	return len;
}

wchar * wstrcat(wchar * strDestination, const wchar * strSource){
	if(strDestination == 0) return 0;
	if(strSource == 0 || *strSource == 0) return strDestination;

	wchar * pwd = strDestination; 
	const wchar * pws = strSource;
	while(*pwd != 0) pwd++;
	while(*pws != 0) *pwd++ = *pws++;
	*pwd = 0;
	
	return strDestination;
}

wchar * wstrncat(wchar * strDestination, const wchar * strSource, size_t count){
	if(strDestination == 0) return 0;
	if(strSource == 0 || *strSource == 0) return strDestination;

	wchar * pwd = strDestination;
	const wchar * pws = strSource;
	size_t cnt = 0;
	while(*pwd != 0) pwd++;
	while(*pws != 0 && cnt < count) {
		*pwd++ = *pws++;
		cnt++;
	}
	*pwd = 0;
	
	return strDestination;
}

wchar * wstrncpy(wchar * strDestination, const wchar * strSource, size_t count){
	if(strDestination == 0) { return 0; }
	if(strSource == 0 || *strSource == 0){ return strDestination; }

	wchar * pwd = strDestination;
	const wchar * pws = strSource;
	for(size_t cnt = 0; cnt < count; cnt++){
		*pwd++ = *pws++;
	}
	*pwd = 0;
	
	return strDestination;
}

wchar * wstrdup(const wchar * string){
	size_t len = wstrlen(string);
	if(len == 0) return 0;
	wchar * pRes = (wchar*)calloc(len + 2, sizeof(wchar));
	for(size_t i = 0; i < len; i++){
		pRes[i] = string[i];
	}
	
	return pRes;
}

#endif

int Utf8ToUcs2(const byte * pUtf8Str, wchar * pUcs2Buf, size_t bufLen){
	assert(pUtf8Str != 0 && *pUtf8Str != 0);
	assert(pUcs2Buf != 0 && bufLen > 0);
	
	unsigned short val;
	int i = 0, j, len = 0;
	const byte * pUtf8 = pUtf8Str;
	while(*pUtf8++) len++;
	pUtf8 = pUtf8Str;
	
	if(pUtf8[0] == 0xEF && pUtf8[1] == 0xBB && 
		pUtf8[2] == 0xBF) i = 3;
	for(j = 0; i < len && j < bufLen; i++){
		if((pUtf8[i] & 0x80) == 0){	// 0xxx xxxx
			pUcs2Buf[j++] = val = pUtf8[i];
		}
		else if((pUtf8[i] & 0xE0) == 0xC0){	// 110x xxxx 10xx xxxx
			val = (pUtf8[i] & 0x1F); val = val << 6;
			val += (pUtf8[++i] & 0x3F);
			pUcs2Buf[j++] = val;
		}
		else if((pUtf8[i] & 0xF0) == 0xE0){	// 1110 xxxx 10xx xxxx 10xx xxxx
			val = (pUtf8[i] & 0x0F); val = val << 6;
			val += (pUtf8[++i] & 0x3F); val = val << 6;
			val += (pUtf8[++i] & 0x3F);
			pUcs2Buf[j++] = val;
		}
		else if((pUtf8[i] & 0xF8) == 0xF0){	// 1111 0xxx 10xxx xxxx 10xx xxxx 10xx xxxx
			val = (pUtf8[i] & 0x7); val = val << 6;
			val += (pUtf8[++i] & 0x3F); val = val << 6;
			val += (pUtf8[++i] & 0x3F); val = val << 6;
			val += (pUtf8[++i] & 0x3F);
			pUcs2Buf[j++] = val;
		}
		else if((pUtf8[i] & 0xFC) == 0xF8){	// 1111 10xx 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx
			i += 4;
		}
		else if((pUtf8[i] & 0xFE) == 0xFC){	// 1111 110x 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx
			i+= 5;
		}
		pUcs2Buf[j] = 0;
	}
	return j;
}

int Ucs2ToUtf8(const wchar * pUcs2Str, byte * pUtf8Buf, size_t bufLen){
	int j = 0;
	const wchar * pUcs2Char = pUcs2Str;
	if(pUcs2Char[0] == 0xFEFF) pUcs2Char++;
	while(*pUcs2Char != 0 && j < bufLen){
		if(*pUcs2Char < 0x80){
			pUtf8Buf[j++] = *pUcs2Char;
		}
		else if(*pUcs2Char > 0x7F && *pUcs2Char < 0x800){
			char c2 = (*pUcs2Char & 0x3F) | 0x80;
			char c1 = (*pUcs2Char >> 6) | 0xC0;
			pUtf8Buf[j++] = c1;
			pUtf8Buf[j++] = c2;
		}
		else if(*pUcs2Char > 0x7FF && *pUcs2Char < 0xFFFF){
			char c3 = (*pUcs2Char & 0x3F) | 0x80;
			char c2 = ((*pUcs2Char >> 6) & 0x3F) | 0x80;
			char c1 = (*pUcs2Char >> 12) | 0xE0;
			pUtf8Buf[j++] = c1;
			pUtf8Buf[j++] = c2;
			pUtf8Buf[j++] = c3;
		}
		pUcs2Char++;
	}
	pUtf8Buf[j] = 0;
	
	return j;
}
