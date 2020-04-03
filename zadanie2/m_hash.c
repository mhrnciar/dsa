//
//  m_hash.c
//  Projekt 2
//  Matej Hrnčiar
//

#include "m_hash.h"
#include <stdio.h>
#include <stdlib.h>

static BLOCK *mhash_table;

static int capacity = 1000;
static int entries = 0;

// hladanie najblizsieho prvocisla ku kapacite pola
static int check_prime(int n){
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

static int get_prime(int n){
    if (n % 2 == 0)
        n++;
    
    while(!check_prime(n))
        n += 2;
    
    return n;
}

// inicializacia pola - alokuje sa dynamicke pole a vsetky hodnoty sa nastavia na -1 alebo NULL
void mhash_init(){
    capacity = get_prime(capacity);
    mhash_table = (BLOCK *) malloc(capacity * sizeof(BLOCK));
    for (int i = 0; i < capacity; i++){
        mhash_table[i].key = -1;
        mhash_table[i].data = -1;
        mhash_table[i].next = NULL;
    }
}

// hesovacia funkcia je kombinaciou 32-bitovej mixovacej funkcie od Roberta Jenkina a modula prvocislom
static int H(int key){
    key = ~key + (key << 15); // key = (key << 15) - key - 1;
    key = key ^ (key >> 12);
    key = key + (key << 2);
    key = key ^ (key >> 4);
    key = (key + (key << 3)) + (key << 11);
    key = key ^ (key >> 16);
    return key % capacity;
}

// tvorba noveho prvku
static BLOCK *newBlock(int data, int key){
    BLOCK *new = (BLOCK *) malloc(sizeof(BLOCK));
    new->data = data;
    new->key = key;
    new->next = NULL;
    return new;
}

/*
 * Operacia zvacsenia najprv zdvojnasobi kapacitu pola a vytvori nove dynamicke pole, do ktoreho sa
 * vsetky prvky prekopiruju. Treba davat pozor na to, ze na niektorych miestach su spajane zoznamy, takze
 * je potrebne ich prejst cele a taktiez kazdy prvok musi znova prejst hesovacou funkciou lebo po modulom
 * novou kapacitou pola sa zmeni kluc daneho prvku. Nakoniec sa na globalny smernik priradi adresa noveho
 * pola a stare sa uvolni.
 */
static void mhash_resize(){
    int old_capacity = capacity;
    capacity = get_prime(capacity * 2);
    
    BLOCK *temp = (BLOCK *)malloc(capacity * sizeof(BLOCK));
    for (int i = 0; i < capacity; i++){
        temp[i].key = -1;
        temp[i].data = -1;
        temp[i].next = NULL;
    }
    
    for(int i = 0; i < old_capacity; i++){
        if(mhash_table[i].data != -1 && mhash_table[i].next != NULL){
            BLOCK *point = &mhash_table[i];
            while(point != NULL){
                int key = H(point->data);
                if(temp[key].data != -1){
                    BLOCK *temp_point = &temp[key];
                    while(temp_point->next != NULL){
                        temp_point = temp_point->next;
                    }
                    temp_point->next = newBlock(point->data, key);
                }
                
                else{
                    temp[key] = *newBlock(point->data, key);
                }
                point = point->next;
            }
        }
        
        else if(mhash_table[i].data != -1 && mhash_table[i].next == NULL){
            int key = H(mhash_table[i].data);
            if(temp[key].data != -1){
                BLOCK *point = &temp[key];
                while(point->next != NULL){
                    point = point->next;
                }
                point->next = newBlock(mhash_table[i].data, key);
            }
            
            else{
                temp[key] = *newBlock(mhash_table[i].data, key);
            }
        }
    }
    free(mhash_table);
    mhash_table = temp;
}

/*
 * Najprv sa vytvori kluc pomocou hesovacej funkcie a potom sa na danom mieste ulozi prvok. Ak uz na
 * danom mieste prvok je, vytvori sa spajany zoznam prvkov s rovnakymi klucmi. Ak pocet prvkov presiahne
 * 50% celkovej kapacity pola, vykona sa operacia zvacsenia pola.
 */
void mhash_insert(int data){
    int key = H(data);
    
    if(mhash_table[key].data != -1){
        BLOCK *point = &mhash_table[key];
        while(point->next != NULL){
            if(point->data == data){
                //printf("Vkladany udaj sa uz v poli nachadza\n");
                return;
            }
            point = point->next;
        }
        if(point->data == data){
            //printf("Vkladany udaj sa uz v poli nachadza\n");
            return;
        }
        
        point->next = newBlock(data, key);
        entries++;
        if(entries > 0.5 * capacity)
            mhash_resize();
        return;
    }
    
    else{
        mhash_table[key] = *newBlock(data, key);
        entries++;
        if(entries > 0.5 * capacity)
            mhash_resize();
        return;
    }
}

/*
 * Najprv sa vytvori kluc hladaneho prvku, a potom sa na danom mieste prejde cely spajany zoznam az kym
 * sa dany prvok nenajde, inac vrati NULL.
 */
BLOCK *mhash_search(int data){
    int key = H(data);
    BLOCK *point = &mhash_table[key];
    while(point != NULL){
        if(point->data == data)
            return point;
        
        else
            point = point->next;
    }
    
    printf("Hladany udaj %d sa nenasiel\n", data);
    return NULL;
}

// vypis pola
void mhash_display(){
    printf("%d\n%d\n", capacity, entries);
    for (int i = 0; i < capacity; i++){
        if(mhash_table[i].data != -1){
            if(mhash_table[i].next != NULL) {
                BLOCK *point = &mhash_table[i];
                int pos = 1;
                while(point != NULL){
                    printf("[%d/%d]: %4d\n", point->key, pos, point->data);
                    point = point->next;
                    pos++;
                }
            }
            
            else
                printf("[%d]: %4d\n", mhash_table[i].key, mhash_table[i].data);
        }
    }
}
