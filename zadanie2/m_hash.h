//
//  m_hash.h
//  Projekt 2
//  Matej Hrnčiar
//

#ifndef m_hash_h
#define m_hash_h

#include <stdio.h>

#endif /* m_hash_h */

typedef struct block{
    int key;
    int value;
    struct node *next;
} BLOCK;

extern void mhash_insert(int key);
