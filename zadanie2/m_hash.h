//
//  m_hash.h
//  Projekt 2
//  Matej Hrnčiar
//

#ifndef m_hash_h
#define m_hash_h

#endif /* m_hash_h */

typedef struct block{
    int key;
    int data;
    struct block *next;
} BLOCK;

extern void mhash_init(void);

extern void mhash_insert(int data);

extern BLOCK *mhash_search(int data);

extern void mhash_display(void);
