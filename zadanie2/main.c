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
    clock_t t;
    int array[500000];
    mhash_init();
    phash_init();
    
    srand((unsigned) time(NULL));
    
    for (int i = 0; i < 500000; i++) {
        array[i] = rand() % 500000;
    }
    
    NODE *root = NULL;
    printf("MOJ AVL BVS\n");
    t = clock();
    for(int i = 0; i < 500000; i++){
        root = mbvs_insert(root, array[i]);
    }
    t = clock() - t;
    printf("Insert: %f\n", (float)t/CLOCKS_PER_SEC);
    
    t = clock();
    NODE *mbvspointer = mbvs_search(root, array[0]);
    for(int i = 1; i < 500000; i++){
        mbvspointer = mbvs_search(root, array[i]);
    }
    t = clock() - t;
    printf("Search: %f\n\n", (float)t/CLOCKS_PER_SEC);
    
    
    printf("CERVENO-CIERNY BVS\n");
    t = clock();
    for(int i = 0; i < 500000; i++){
        pbvs_insert(array[i]);
    }
    t = clock() - t;
    printf("Insert: %f\n", (float)t/CLOCKS_PER_SEC);
    
    t = clock();
    for(int i = 0; i < 500000; i++){
        pbvs_search(array[i]);
    }
    t = clock() - t;
    printf("Search: %f\n\n", (float)t/CLOCKS_PER_SEC);
    
    
    printf("MOJA HASHOVACIA TABULKA\n");
    t = clock();
    for(int i = 0; i < 500000; i++){
        mhash_insert(array[i]);
    }
    t = clock() - t;
    printf("Insert: %f\n", (float)t/CLOCKS_PER_SEC);
    
    t = clock();
    BLOCK *mhashpointer = mhash_search(array[0]);
    for(int i = 1; i < 500000; i++){
        mhashpointer = mhash_search(array[i]);
    }
    t = clock() - t;
    printf("Search: %f\n\n", (float)t/CLOCKS_PER_SEC);

    
    printf("PREBRANA HASHOVACIA TABULKA\n");
    t = clock();
    for(int i = 0; i < 500000; i++){
        phash_insert(array[i]);
    }
    t = clock() - t;
    printf("Insert: %f\n", (float)t/CLOCKS_PER_SEC);

    t = clock();
    for(int i = 0; i < 500000; i++){
        phash_search(array[i]);
    }
    t = clock() - t;
    printf("Search: %f\n", (float)t/CLOCKS_PER_SEC);
    
    /*
    NODE *root = NULL;
    printf("MOJ AVL BVS\n");
    t = clock();
    for(int i = 0; i < 1000000; i++){
        root = mbvs_insert(root, i);
    }
    t = clock() - t;
    printf("Insert: %f\n", (float)t/CLOCKS_PER_SEC);
    
    t = clock();
    NODE *mbvspointer = mbvs_search(root, 0);
    for(int i = 1; i < 1000000; i++){
        mbvspointer = mbvs_search(root, i);
    }
    t = clock() - t;
    printf("Search: %f\n\n", (float)t/CLOCKS_PER_SEC);
    
    
    printf("CERVENO-CIERNY BVS\n");
    t = clock();
    for(int i = 0; i < 1000000; i++){
        pbvs_insert(i);
    }
    t = clock() - t;
    printf("Insert: %f\n", (float)t/CLOCKS_PER_SEC);
    
    t = clock();
    for(int i = 0; i < 1000000; i++){
        pbvs_search(i);
    }
    t = clock() - t;
    printf("Search: %f\n\n", (float)t/CLOCKS_PER_SEC);
    
    
    printf("MOJA HASHOVACIA TABULKA\n");
    t = clock();
    for(int i = 0; i < 1000000; i++){
        mhash_insert(i);
    }
    t = clock() - t;
    printf("Insert: %f\n", (float)t/CLOCKS_PER_SEC);
    
    t = clock();
    BLOCK *mhashpointer = mhash_search(0);
    for(int i = 1; i < 1000000; i++){
        mhashpointer = mhash_search(i);
    }
    t = clock() - t;
    printf("Search: %f\n\n", (float)t/CLOCKS_PER_SEC);

    
    printf("PREBRANA HASHOVACIA TABULKA\n");
    t = clock();
    for(int i = 0; i < 1000000; i++){
        phash_insert(i);
    }
    t = clock() - t;
    printf("Insert: %f\n", (float)t/CLOCKS_PER_SEC);

    t = clock();
    for(int i = 0; i < 1000000; i++){
        phash_search(i);
    }
    t = clock() - t;
    printf("Search: %f\n", (float)t/CLOCKS_PER_SEC);
    */
    return 0;
}
