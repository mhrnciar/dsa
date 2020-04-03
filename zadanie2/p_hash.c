//
//  p_hash.c
//  Projekt 2
//  Matej Hrnčiar
//
//  https://www.thecrazyprogrammer.com/2017/06/hashing.html
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "p_hash.h"

static int *array;
static int size = 500000;
static int hFn;

void phash_init(){
    hFn = size;
    array = (int *) malloc(size * sizeof(int));
    for(int i = 0; i < size; i++)
        array[i] = INT_MIN;
}

void phash_insert(int data){
    int pos = data % hFn;
    int n = 0;
    
    while(array[pos]!= INT_MIN) {  // INT_MIN and INT_MAX indicates that cell is empty. So if cell is empty loop will break and goto bottom of the loop to insert element
        if(array[pos]== INT_MAX)
            break;
        pos = (pos+1) % hFn;
        n++;
        
        if(n == size)
            break;      // If table is full we should break, if not check this, loop will go to infinite loop.
    }
    
    if(n==size)
        printf("Hash table was full of elements\nNo Place to insert this element\n\n");
    else
        array[pos] = data;    //Inserting element
}

int phash_search(int key){
    int pos = key % hFn;
    int n = 0;
    
    while(n++ != size){
        if(array[pos] == key){
            return pos;
            break;
        }
        else if(array[pos] == INT_MAX || array[pos] != INT_MIN)
            pos = (pos+1) % hFn;
    }
    
    if(--n == size)
        return 0;
    
    return 0;
}

void phash_display(){
    for(int i = 0; i < size; i++)
        printf("%d\t%d\n", i, array[i]);
}
