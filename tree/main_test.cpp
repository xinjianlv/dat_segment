#include "myhead.h"
#include "trie_create.h"
#include "dat_create.h"
#include <iostream>
using namespace std;
int main(int argc, char ** argv)
{
    cout<<"run test main function"<<endl;
	 char *f1 = "../data/cnToks.txt";
	 char *f2 = "../data/cnCi.txt";

	if(!dat_load()){
		dat_create(f1,f2);
		dat_save();
	
	}
	
	word_seg("../data/kk.txt","../data/kk_save.txt","../data/cnStopToks.txt");
	cout<<"This is the end!"<<endl;
    return 0;
}
