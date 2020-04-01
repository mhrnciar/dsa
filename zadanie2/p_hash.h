//
//  p_hash.h
//  Projekt 2
//  Matej Hrnčiar
//
//  https://www.sanfoundry.com/c-program-implement-hash-tables/
//

#ifndef p_hash_h
#define p_hash_h

#endif /* p_hash_h */

struct data{
    int key;
    int value;
};

extern void phash_init(void);

extern void phash_insert(int data);

extern int phash_search(int data);

extern void phash_display(void);
