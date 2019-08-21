#ifndef DAT_CREATE_H
#define DAT_CREATE_H

#include "stdio.h"

short dat_create(const char *fword,const char *fDictionary);
short dat_save();
short dat_load();
short word_seg(const char *fsource,const char *fsave,const char *fstopwords);
#endif