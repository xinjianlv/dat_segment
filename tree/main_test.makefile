CXX=g++
SRCDIR=./
OBJDIR=../bin/

all:test.exe
	rm -f *.o

test.exe:main_test.o dat_create.o trie_create.o  myhead.o
	${CXX} -o ${OBJDIR}test.exe main_test.o dat_create.o trie_create.o myhead.o
main_test.o:${SRCDIR}main_test.cpp
	${CXX} -c -o main_test.o ${SRCDIR}main_test.cpp -DUNIX
dat_create.o:${SRCDIR}dat_create.cpp
	${CXX} -c -o dat_create.o ${SRCDIR}dat_create.cpp -DUNIX
trie_create.o:${SRCDIR}trie_create.cpp
	${CXX} -c -o trie_create.o ${SRCDIR}trie_create.cpp -DUNIX
myhead.o:${SRCDIR}myhead.cpp
	${CXX} -c -o myhead.o myhead.cpp -DUNIX
