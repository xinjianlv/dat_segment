#include "dat_create.h"
#include "trie_create.h"
#include "jud.h"
#include <assert.h>

#define MAX_LEN 0x9FFFF
#define CHINESE_TOKS_COUNT 0x1FFF //8191
static int base[MAX_LEN];
static int check[MAX_LEN];
static unsigned short toks_count;
static wchar toks[CHINESE_TOKS_COUNT];
extern const wchar * cnToks;
extern pCnTree CnTrie;

static void toks_init(void)
{
	const wchar * p = cnToks;
	unsigned short i = 2;
	toks[0] = 0xFFFF, toks[1] = 0x20;
	while(*p != 0){
		unsigned short position = i;
		while(*p < toks[position - 1] && position >= 2){
			toks[position] = toks[position - 1];
			position --;
		}
		toks[position] = *p++;
		i++;
	}
	toks_count = i;
}

static short getIndex(const wchar tok)
{
	unsigned short begin, rear;
	begin = 0; rear = toks_count - 1;
	while(rear - begin > 1){
		unsigned short mid = (begin + rear) /2;
		wchar tmp = toks[mid];
		if(tok > tmp)
			begin = mid;
		else if(tok == tmp)
			return mid;
		else
			rear = mid;
	}
	if(toks[rear] == tok) return rear;
	if(toks[begin] == tok) return begin;
	return -1;	
}

unsigned shortisCnTok(const wchar ch)
{
	if(getIndex(ch) > 1) return 1;
	return 0;
}


void dat_create_helper(pCnNode root,int baseNdx = -1)
{
	if(root == 0) return ;

	if(baseNdx == -1)
		if((baseNdx = getIndex(root->word)) == -1) return ;//getIndex 还没写

	int buf[0x400]; // 1024
	short ndx = 0;
	
	pCnNode pSon = root->p_lefson; 
	while(pSon != 0){
		buf[ndx++] = getIndex(pSon->word);
		pSon = pSon->p_rightbrother;
	}
	buf[ndx] = 0;
	int shift = toks_count;	// 偏移量最小阀值
	char conLoop = 1;  ndx = 0;
	int mark = 0;
	for(;conLoop;shift++){	
		conLoop = 0;
		while(buf[ndx] != 0){
			
 			if(base[shift + buf[ndx]] == 0 && check[shift + buf[ndx]] == 0)
				mark = 1;
			if(mark == 0) { conLoop = 1; break;}  
			mark = 0;
			ndx++;
		}
		ndx = 0;
	}
	shift--;	// 此处必须减1

	base[baseNdx] = shift;
	while(buf[ndx] != 0){	
		if(buf[ndx] > 0) {
			check[shift + buf[ndx]] = baseNdx;
			if(buf[ndx] == 1) base[shift + buf[ndx]] = -1;	// 一个词的结束标志为-1
		}
		ndx++;
	}

	ndx = 0; pSon = root->p_lefson;
	while(pSon != 0){
		if(pSon->word != WORDS_END_TAG)
			dat_create_helper(pSon, buf[ndx] + shift);
		ndx++; pSon = pSon->p_rightbrother ;
	}



}
#include <iostream>
short dat_create(const char *fword,const char *fDictionary)
{
	assert(fword != 0 && fDictionary != 0);
	int n = 0;
	if(tree_create(fword,fDictionary) == 1){
		int index = 0;
		toks_init();
		while(index < CnTrie->CnCuwCount){
			pCnNode root = CnTrie->CnNodeRoot[index++];
			dat_create_helper(root);
			//cout<<"dat create : "<<n++<<endl;
		}
		tree_destroy();
	}	

	return 1;
}
short dat_save()
{
	FILE *fDATsave = fopen("DATsave","wb");
	if(fDATsave){
		fwrite(toks,sizeof(wchar),CHINESE_TOKS_COUNT,fDATsave);
		fwrite(base,sizeof(int),MAX_LEN,fDATsave);
		fwrite(check,sizeof(int),MAX_LEN,fDATsave);
		fclose(fDATsave);
		return 1;
		
	}
	else return 0;

}
short dat_load()
{
	FILE *fDATload = fopen("DATsave","rb");
	if(fDATload){
		fread(toks,sizeof(wchar),CHINESE_TOKS_COUNT,fDATload);
		fread(base,sizeof(int),MAX_LEN,fDATload);
		fread(check,sizeof(int),MAX_LEN,fDATload);
		fclose(fDATload);
		toks_count = wstrlen(toks);
		return 1;		
	}

	return 0;

}


wchar *stopwords;
static void stopword_init(const char * fstopwords)
{
	const char *fname = fstopwords;
	
	struct _stat buf;
	int result = 0;
	if((result = stat(fname,&buf)) != 0){
		return ;	
	}
	int len = buf.st_size + 1; 
	byte *pUtf8 = (byte*)calloc(sizeof(byte),len);
	FILE *stream = fopen(fname,"rb");
	if(stream){
		fread(pUtf8,sizeof(byte),buf.st_size,stream);
		int ii = strlen((char *)pUtf8);
		if(pUtf8[ii - 1] == 0xD || pUtf8[ii - 1] == 0xA) pUtf8[--ii] = 0;
		wchar *pUcs2 = (wchar *)calloc(sizeof(wchar),ii+1);
		Utf8ToUcs2(pUtf8,pUcs2,buf.st_size+1);
		stopwords = pUcs2;
		
	}
}
short isStopWord(wchar word)
{	
	int len = wstrlen(stopwords);
	while(--len >= 0 ){
		if(stopwords[len] == word) {
			return 1;
		}
		
	}
		

	return 0;
}
wchar *p_result;
int count_result(0);
int count(0);   //定义为全局变量
wchar *pwstr;

static void addWordToResult(int shift);
void Pretreatment(wchar *str);
void delSharp(wchar *str);
void delStopWord(wchar *str);
short word_seg(const char *fsource,const char *fsave,const char *fstopwords)
{	

	assert(fsource != 0 && fsave != 0 && fstopwords );
	stopword_init(fstopwords);
	//toks_init( );

	p_result = (wchar *)calloc(sizeof(wchar),10000);
	memset(p_result,0,sizeof(wchar)*10000);
	const char *fname = fsource;
	

	struct _stat buf;
	int result = 0;
	if((result = stat(fname,&buf)) != 0){
		return -1;	
	}
	int len = buf.st_size + 1; 
	int len_Ucs2;
	byte *pUtf8 = (byte*)calloc(sizeof(byte),len);
	FILE *stream = fopen(fname,"rb");
	if(stream){
		fread(pUtf8,sizeof(byte),buf.st_size,stream);
		int ii = strlen((char *)pUtf8);
		if(pUtf8[ii - 1] == 0xD || pUtf8[ii - 1] == 0xA) pUtf8[--ii] = 0;
		wchar *pUcs2 = (wchar *)calloc(sizeof(wchar),ii+1);
		len_Ucs2 = Utf8ToUcs2(pUtf8,pUcs2,buf.st_size+1);
		pwstr = pUcs2;
		
	}

	Pretreatment(pwstr);
	
	int shift(0);
	int baseNdx(0);
	int length(0);
	int tok(0);
	int charMark,wordMark,numMark,NumMark;
	charMark = wordMark = numMark = NumMark = 0;
	
	
	while(*pwstr){
	
		//count = 0;
		if(isPunctuation(*pwstr)){
			//addWordToResult(count);
			pwstr++;
			continue;
		}
		if(isnum(*pwstr)){
			 
			if(isnum(*(++pwstr))){
				--pwstr;
				while(isnum(*pwstr++)){
					count++;
				}
				pwstr--;
				addWordToResult(count);
			}
			else
				pwstr--;
		
		}
		if(isChar(*pwstr)){
			 
			if(isChar(*(++pwstr))){
				--pwstr;
				while(isChar(*pwstr++)){
					count++;
				}
				pwstr--;
				addWordToResult(count);
			}
			else
				pwstr--;
		
		}
		if(isNum(*pwstr)){
			 
			if(isNum(*(++pwstr))){
				--pwstr;
				while(isNum(*pwstr++)){
					count++;
				}
				pwstr--;
				addWordToResult(count);
			}
			else
				pwstr--;
		
		}
		if(isPunctuation(*pwstr)){
			//addWordToResult(count);
			pwstr++;
			continue;
		}
		int baseNdx = getIndex(*pwstr);
		count++;
		length = count;
		while(*pwstr){
			shift = base[baseNdx]; tok = getIndex(*(++pwstr));
				if(check[shift+tok] == baseNdx){
					count++;		
					if(check[base[shift + tok] + 1] == shift + tok && base[base[shift+tok]+1] == -1){
						length = count;
					}	
				}
				else{
					addWordToResult(length);
					length = 0;
					break;
				}
				baseNdx = shift+tok;
		
		}
	}
	
	while(count_result-- > 0)  
		p_result--;
	delSharp(p_result);
	delStopWord(p_result);
	pUtf8 = (byte*)calloc(len * 3, sizeof(byte));
    len = Ucs2ToUtf8(p_result, pUtf8, len * 3); free(p_result);
	FILE *fsv = fopen(fsave,"wb");
	fwrite(pUtf8,sizeof(byte),len,fsv);
}

static void addWordToResult(int shift)
{	
	if(shift == 0)
		return ;
	while(count-- > 0)
		pwstr--;
	while(shift-- > 0){
		count_result++;
		*p_result++ = *pwstr++;
	}
	count_result++;
	*p_result++ = '#';

	count = 0;

}
void Pretreatment(wchar *str)  //预处理，去标点
{
	int count(0);
	wchar *pstr = str;
	wchar *ps = (wchar *)calloc(sizeof(wchar),100000);
	
	while(*pstr){
		 if(isPunctuation(*pstr)){
		
				if(isPunctuation(*(++pstr))){
						
								continue;
				}
				else if(*(--pstr) == 0x000A || *pstr == 0x000D){
					pstr++;
					//pstr++;
					continue;
				}
			
				*ps++ = 0x0020;
				count++;
				pstr++;
				continue;
			}
		
		*ps++ = *pstr++;
		
		count++;
	}
	while(count-- > 0)
		ps--;
	
	pwstr = ps;
}
void save(const char *fname)
{
	//const char *fname = fname;

//Ucs2ToUtf8(const wchar * pUcs2Str, byte * pUtf8Buf, size_t bufLen)
}
void delSharp(wchar *str)
{
	int count(0);
	wchar *pstr = str;
	wchar *p_save = (wchar *)calloc(sizeof(wchar),100000);
	while(*pstr){
		count++;
		if(*pstr == 0x0023) { // 0x0023 == '#'
			*p_save++ = 0x0020;
			pstr++;
			continue;
		}
	
		*p_save++ = *pstr++;
	}
	while(count-- > 0)
		p_save--;
	p_result = p_save; 
	
}

void delStopWord(wchar *str)
{
	int count(0);
	wchar *pstr = str;
	wchar *p_save = (wchar *)calloc(sizeof(wchar),100000);
	
	while(*pstr){
		count++;
			if(*pstr != 0x0020)
				*p_save++ = *pstr++;
			else{
				*p_save++ = *pstr++;
				if(isStopWord(*pstr)) {
					if(*(++pstr) == 0x0020){
						pstr++;
						continue;
					}
					else{
						pstr--;
						continue;
					
					}
				}
				
				
			}
	}
	while(count-- > 0)
		p_save--;
	p_result = p_save; 
	
}
