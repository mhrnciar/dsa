//
//  main.c
//  Projekt 2
//  Matej Hrnčiar
//

#include <stdio.h>
#include <stdlib.h>
#include "m_bvs.h"
#include "m_hash.h"
#include "p_bvs.h"
#include "p_hash.h"

int main(){
    phash_init();
    
    NODE *root = NULL;
    root = mbvs_insert(root, 10);
    root = mbvs_insert(root, 20);
    root = mbvs_insert(root, 30);
    root = mbvs_insert(root, 40);
    root = mbvs_insert(root, 50);
    root = mbvs_insert(root, 25);
    
    mbvs_preOrder(root);
    printf("\n");
    
    pbvs_insert(10);
    pbvs_insert(20);
    pbvs_insert(30);
    pbvs_insert(40);
    pbvs_insert(50);
    pbvs_insert(25);
    
    pbvs_inOrder();
    
    for(int i = 0; i < 100; i++){
        mhash_insert(i*10);
    }
    
    BLOCK *pointer = mhash_search(100);
     
    return 0;
}
