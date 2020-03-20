//
//  p_bvs.h
//  Projekt 2
//  Matej Hrnčiar
//
//  https://www.programiz.com/dsa/red-black-tree
//

#ifndef p_bvs_h
#define p_bvs_h

#endif /* p_bvs_h */

enum nodeColor{
    RED,
    BLACK
};

struct rbNode{
    int data, color;
    struct rbNode *link[2];
};

extern void pbvs_insert(int data);

extern struct rbNode *pbvs_search(int key);

extern void pbvs_inOrder(void);
