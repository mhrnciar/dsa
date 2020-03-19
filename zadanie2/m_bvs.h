//
//  m_bvs.h
//  Projekt 2
//  Matej Hrnčiar
//

#ifndef m_bvs_h
#define m_bvs_h

#endif /* m_bvs_h */

typedef struct node{
    int key;
    int height;
    struct node *left;
    struct node *right;
} NODE;

extern NODE *mbvs_insert(NODE *node, int key);

extern NODE *mbvs_search(NODE *node, int key);

extern void mbvs_preOrder(NODE *node);
