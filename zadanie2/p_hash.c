//
//  p_hash.c
//  Projekt 2
//  Matej Hrnčiar
//
//  https://www.programiz.com/dsa/hash-table
//

#include "p_hash.h"
#include <stdio.h>
#include <stdlib.h>

struct set *array;

int capacity = 10;
int size = 0;

static int hashFunction(int key){
    return (key % capacity);
}

static int checkPrime(int n){
    int i;
    if(n == 1 || n == 0)
        return 0;
    
    for(i = 2; i < n / 2; i++){
        if (n % i == 0){
            return 0;
        }
    }
    
    return 1;
}

static int getPrime(int n){
    if (n % 2 == 0)
        n++;
    
    while(!checkPrime(n))
        n += 2;
    
    return n;
}

void phash_init(){
    capacity = getPrime(capacity);
    array = (struct set *) malloc(capacity * sizeof(struct set));
    for (int i = 0; i < capacity; i++){
        array[i].key = 0;
        array[i].data = 0;
    }
}

void phash_insert(int key, int data){
    int index = hashFunction(key);
    if(array[index].data == 0){
        array[index].key = key;
        array[index].data = data;
        size++;
        printf("\n Key (%d) has been inserted \n", key);
    }
    
    else if(array[index].key == key)
        array[index].data = data;
    
    
    else
        printf("\n Collision occured  \n");
    
}
/*
void display(){
    int i;
    for (i = 0; i < capacity; i++){
        if (array[i].data == 0){
            printf("\n array[%d]: / ", i);
        }
        
        else{
            printf("\n key: %d array[%d]: %d \t", array[i].key, i, array[i].data);
        }
    }
}
*/
static int size_of_hashtable(){
    return size;
}
