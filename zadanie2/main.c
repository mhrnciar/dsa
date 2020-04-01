//
//  main.c
//  Projekt 2
//  Matej Hrnčiar
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "m_bvs.h"
#include "m_hash.h"
#include "p_bvs.h"
#include "p_hash.h"

int main(){
    clock_t time;
    mhash_init();
    phash_init();
    
    NODE *root = NULL;
    printf("MOJ AVL BVS\n");
    time = clock();
    for(int i = 0; i < 1000000; i++){
        root = mbvs_insert(root, i);
    }
    time = clock() - time;
    printf("Insert: %f\n", (float)time/CLOCKS_PER_SEC);
    
    time = clock();
    NODE *mbvspointer = mbvs_search(root, 0);
    for(int i = 1; i < 1000000; i++){
        mbvspointer = mbvs_search(root, i);
    }
    time = clock() - time;
    printf("Search: %f\n\n", (float)time/CLOCKS_PER_SEC);
    
    
    printf("CERVENO-CIERNY BVS\n");
    time = clock();
    for(int i = 0; i < 1000000; i++){
        pbvs_insert(i);
    }
    time = clock() - time;
    printf("Insert: %f\n", (float)time/CLOCKS_PER_SEC);
    
    time = clock();
    for(int i = 0; i < 1000000; i++){
        pbvs_search(i);
    }
    time = clock() - time;
    printf("Search: %f\n\n", (float)time/CLOCKS_PER_SEC);
    
    
    printf("MOJA HASHOVACIA TABULKA\n");
    time = clock();
    for(int i = 0; i < 1000000; i++){
        mhash_insert(i);
    }
    time = clock() - time;
    printf("Insert: %f\n", (float)time/CLOCKS_PER_SEC);
    
    time = clock();
    BLOCK *mhashpointer = mhash_search(0);
    for(int i = 1; i < 1000000; i++){
        mhashpointer = mhash_search(i);
    }
    time = clock() - time;
    printf("Search: %f\n\n", (float)time/CLOCKS_PER_SEC);
    
    
    printf("PREBRANA HASHOVACIA TABULKA\n");
    time = clock();
    for(int i = 0; i < 1000000; i++){
        phash_insert(i);
    }
    time = clock() - time;
    printf("Insert: %f\n", (float)time/CLOCKS_PER_SEC);

    time = clock();
    for(int i = 0; i < 1000000; i++){
        phash_search(i);
    }
    time = clock() - time;
    printf("Search: %f\n", (float)time/CLOCKS_PER_SEC);
    
    return 0;
}
