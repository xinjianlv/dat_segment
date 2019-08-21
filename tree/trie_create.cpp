#include "trie_create.h"
#include <iostream>

const wchar * cnToks;
pCnTree CnTrie;
short tree_create_helper2(pCnNode root, wchar *word)
{
	if(root == 0 && root->word != *word) return 1;
	wchar *p = word;
	pCnNode node = root;
	while(*p)
	{
		if(*p == node->word){  //指向本节点
			if(node->p_lefson == 0)
				node->p_lefson = (pCnNode)calloc(1,sizeof(CnNode));
			node = node->p_lefson;
		}
		else if(node->word == 0){
			node->word = *p;
			if(node->p_lefson == 0) node->p_lefson = (pCnNode)calloc(1,sizeof(CnNode));
			node = node->p_lefson;
		}
		else{
			pCnNode p_rightbrother;
			if(node->p_rightbrother == 0)
				node->p_rightbrother = (pCnNode)calloc(1,sizeof(CnNode));
			p_rightbrother = node->p_rightbrother;

			while(p_rightbrother->word != 0 && p_rightbrother->p_rightbrother != 0 && p_rightbrother->word != *p)
				p_rightbrother = p_rightbrother->p_rightbrother;
			if(p_rightbrother->word == 0 ) p_rightbrother->word = *p;
			else if(p_rightbrother->p_rightbrother == 0 && p_rightbrother->word != *p){
				p_rightbrother->p_rightbrother = (pCnNode)calloc(1,sizeof(CnNode));
				p_rightbrother = p_rightbrother->p_rightbrother;
				p_rightbrother->word = *p;
			}

			if(p_rightbrother->p_lefson == 0) 
				p_rightbrother->p_lefson = (pCnNode)calloc(1,sizeof(CnNode));
			node = p_rightbrother->p_lefson;
		}

	p++;
	
	}

	if(node->word != 0 && node->word != WORDS_END_TAG){
		while(node->word != WORDS_END_TAG && node->p_rightbrother != 0)
			node = node->p_rightbrother;
		if(node->p_rightbrother == 0 && node->word != WORDS_END_TAG)
			node->p_rightbrother = (pCnNode)calloc(1,sizeof(CnNode));
			node = node->p_rightbrother;
	}

	node->word =	WORDS_END_TAG;
	return 1;  // 占位
}
//int getIndex(wchar word)
//{
//	return -1;
//}
static unsigned short tok_getIndex(wchar tok){
	int front, rear, half, res;
	
	front = 0; rear = CnTrie->CnCuwCount - 1; res = 0xFFFF;	// (-1)
	while(rear - front > 1){
		half = (rear + front) / 2;
		if(CnTrie->Cn_CUW[half] > tok){
			rear = half;
		}
		else {
			front = half;
		}
	}
	if(CnTrie->Cn_CUW[front] == tok) res = front;
	if(CnTrie->Cn_CUW[rear] == tok) res = rear;
	
	return res;
}
short tree_create_helper1(wchar *word)
{	
	wchar *p = word;
	int index = tok_getIndex(*p);//在本文件中实现的查找，还没有写
	if(index > 0 && index < 0xFFFF){
		if(CnTrie->CnNodeRoot[index] == 0){
			CnTrie->CnNodeRoot[index] = (CnNode *)calloc(1,sizeof(CnNode));
			CnTrie->CnNodeRoot[index]->word = *p;
		}
		pCnNode node = CnTrie->CnNodeRoot[index];

		tree_create_helper2(node,word);
	}
	return 1;   
}

int tree_init(const char *fname)
{  //CnTrie
	//
	

	statb buf;
	int result = 0;
	if((result = stat(fname,&buf)) != 0){
		return -1;	
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
		cnToks = pUcs2;
		
	}
	else return 0;
	wchar word;
	int ndx = 0;

	CnTrie = (pCnTree)calloc(1, sizeof(CnTree));
	
	if(CnTrie != 0){
		const wchar *pstr = cnToks;
		int last_pos = 0;
		CnTrie->Cn_CUW[last_pos] = *pstr++;
		while(*pstr){
			ndx = last_pos;
			while(CnTrie->Cn_CUW[ndx] > *pstr && ndx >= 0){
				CnTrie->Cn_CUW[ndx+1] = CnTrie->Cn_CUW[ndx];
				ndx--;			
			}

			CnTrie->Cn_CUW[++ndx] = *pstr++;
			last_pos++;
		}
		CnTrie->CnCuwCount = ++last_pos; //last_pos  对应的是下标，因此要加1
		return 1;
	}
	else
		return 0;
}

short tree_create(const char * toks_fname, const char * cis_fname)
{	
	if(tree_init(toks_fname)){
		FILE * stream = fopen(cis_fname, "r");
		char utf8Buf[CN_CI_MAX_LEN * 2] = {0};
		wchar wordsBuf[CN_CI_MAX_LEN];
		if(stream){
			while(fgets(utf8Buf, CN_CI_MAX_LEN * 2, stream)){
				Utf8ToUcs2((byte*)utf8Buf, wordsBuf, CN_CI_MAX_LEN);
				size_t len = wstrlen(wordsBuf);
				if(wordsBuf[len - 1] == 0x0A || wordsBuf[len - 1] == 0x0D) wordsBuf[len - 1] = 0;
				tree_create_helper1(wordsBuf);
				
			}
			fclose(stream);
		}
		return 1;
	}
	else return 0;
}
void destroy(pCnNode root)
{	
	if(root == 0) return;

	pCnNode path[0x20];
	int ndx = 0;
	pCnNode node = root;
	while(node != 0){
		path[ndx++] = node;
		node = node->p_lefson;
	}
	path[ndx] = 0;

	pCnNode * ppNode = &(path[ndx - 1]);
	while(*ppNode != root){
		if((*ppNode)->p_rightbrother  != 0){	//	有兄弟节点
			pCnNode pRmv = *ppNode;
			*ppNode = pRmv->p_rightbrother;
			node = (*ppNode)->p_lefson;
			while(node){
				path[ndx++] = node;
				node = node->p_lefson;
			}
			path[ndx] = 0;
			free(pRmv);
		}
		else if((*ppNode)->p_lefson == 0){	// 没有兄弟节点并且没有子节点
			(*(ppNode - 1))->p_lefson = 0;	// 断开
			free(*ppNode);
			*ppNode = 0;
			ndx--;
		}
		ppNode = &(path[ndx - 1]);
	}

	free(root);
	
}
void tree_destroy()
{
	if(cnToks != 0) free((void *)cnToks);
	int ndx = 0;
	while(ndx<CnTrie->CnCuwCount){
		pCnNode proot = CnTrie->CnNodeRoot[ndx];
		destroy(proot);
		CnTrie->CnNodeRoot[ndx++] = 0;
	}

	free(CnTrie);
}


