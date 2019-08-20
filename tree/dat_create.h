#ifndef DAT_CREATE_H
#define DAT_CRAETE_H

short dat_create(const char *fword,const char *fDictionary);
short dat_save();
short dat_load();
short word_seg(const char *fsource,const char *fsave,const char *fstopwords);
#endif