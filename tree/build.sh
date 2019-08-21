g++ -g -c -o trie_create.o trie_create.cpp -DUNIX
g++ -g -c -o myhead.o myhead.cpp -DUNIX
g++ -g -c -o dat_create.o dat_create.cpp -DUNIX
g++ -g -c -o main_test.o main_test.cpp -DUNIX
g++ -o test.exe main_test.o dat_create.o trie_create.o myhead.o
