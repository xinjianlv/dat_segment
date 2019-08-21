#ifndef TRIE_CREATE_H
#define TRIE_CREATE_H

#include "stdio.h"
#define CUW_COUNT 0X1FFF
#define CN_CI_MAX_LEN 0x20
#define WORDS_END_TAG 0x20
typedef struct Node{
	wchar word;
	Node *p_lefson;  //×ó¶ù×Ó
	Node *p_rightbrother; //ÓÒÐÖµÜ

}*pCnNode,CnNode;
typedef struct treenode{
	wchar Cn_CUW[CUW_COUNT];   //chinese commonly used word 
	pCnNode CnNodeRoot[CUW_COUNT];
	unsigned short CnCuwCount;
}CnTree,*pCnTree;



//int getIndex(wchar word);
short tree_create(const char * toks_fname, const char * cis_fname);
void  tree_destroy();
#endif