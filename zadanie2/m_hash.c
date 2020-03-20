//
//  m_hash.c
//  Projekt 2
//  Matej Hrnčiar
//

#include "m_hash.h"
#include <stdio.h>
#include <stdlib.h>

BLOCK *table[1000] = {NULL};

static int H(int key){
    key = ~key + (key << 15); // key = (key << 15) - key - 1;
    key = key ^ (key >> 12);
    key = key + (key << 2);
    key = key ^ (key >> 4);
    key = (key + (key << 3)) + (key << 11);
    key = key ^ (key >> 16);
    return key % 1000;
}

static BLOCK *newBlock(int data, int key){
    BLOCK *new = (BLOCK *) malloc(sizeof(BLOCK));
    
    new->data = data;
    new->key = key;
    new->next = NULL;
    return new;
}

void mhash_insert(int data){
    int key = H(data);
    
    if(table[key] != NULL){
        BLOCK *point = table[key];
        while(point->next != NULL){
            if(point->data == data){
                printf("Vkladany udaj sa uz v poli nachadza\n");
                return;
            }
            point = point->next;
        }
        if(point->data == data){
            printf("Vkladany udaj sa uz v poli nachadza\n");
            return;
        }
        
        point->next = newBlock(data, key);
        return;
    }
    
    else{
        table[key] = newBlock(data, key);
        return;
    }
}

BLOCK *mhash_search(int data){
    int key = H(data);
    BLOCK *point = table[key];
    while(point != NULL){
        if(point->data == data)
            return point;
        
        else
            point = point->next;
    }
    
    printf("Hladany udaj sa nenasiel\n");
    return NULL;
}
