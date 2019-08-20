#include "myhead.h"
#include "trie_create.h"
#include "dat_create.h"
#include <iostream>
using namespace std;
void main()
{
	 char *f1 = "cnToks.txt";
	 char *f2 = "cnCi.txt";

	if(!dat_load()){
		dat_create(f1,f2);
		dat_save();
	
	}
	
	word_seg("kk.txt","kk_save.txt","cnStopToks.txt");
	cout<<"This is the end!"<<endl;
	system("pause");

}