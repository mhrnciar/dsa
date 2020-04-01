//
//  p_hash.c
//  Projekt 2
//  Matej Hrnčiar
//
//  https://www.sanfoundry.com/c-program-implement-hash-tables/
//

#include<stdio.h>
#include<stdlib.h>
#include "p_hash.h"
 
static struct data *array;

static int capacity = 1000000;
static int size = 0;
 
/* this function gives a unique hash code to the given key */
static int hashcode(int key){
    return (key % capacity);
}

/* to check if given input (i.e n) is prime or not */
static int if_prime(int n){
    int i;
    if ( n == 1  ||  n == 0){
        return 0;
    }
    for (i = 2; i < n; i++){
        if (n % i == 0){
            return 0;
        }
    }
    return 1;
}
 
/* it returns prime number just greater than array capacity */
static int get_prime(int n){
    if (n % 2 == 0){
        n++;
    }
    for (; !if_prime(n); n += 2);
 
    return n;
}
 
void phash_init(){
    int i;
    capacity = get_prime(capacity);
    
    array = (struct data*) malloc(capacity * sizeof(struct data));
    
    for (i = 0; i < capacity; i++){
        array[i].key = -1;
        array[i].value = -1;
    }
}
 
/* to insert a key in the hash table */
void phash_insert(int key){
    int index = hashcode(key);
    
    if (array[index].value == -1){
        /*  key not present, insert it  */
        array[index].key = key;
        array[index].value = 1;
        size++;
    }
    
    else if(array[index].key == key){
        /*  updating already existing key  */
        printf("\n Key (%d) already present, hence updating its value \n", key);
        array[index].value += 1;
    }
    
    else{
        /*  key cannot be insert as the index is already containing some other key  */
        printf("\n ELEMENT CANNOT BE INSERTED \n");
    }
}
 
/* to display all the elements of a hash table */
void phash_display(){
    int i;
    for (i = 0; i < capacity; i++){
        if (array[i].value == -1){
            printf("\n Array[%d] has no elements \n", i);
        }
        else{
            printf("\n array[%d] has elements -:\n key(%d) and value(%d) \t", i, array[i].key, array[i].value);
        }
    }
}

int phash_search(int key){
    int index = hashcode(key);
    if(array[index].value != -1 && array[index].value == key){
        return 1;
    }
    return 0;
}
