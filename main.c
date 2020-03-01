//
//  main.c
//  Projekt 1
//
//  Created by Matej Hrnciar on 25/02/2020.
//  Copyright © 2020 Matej Hrnciar. All rights reserved.
//

#include <stdio.h>
#include <string.h>

void *memory;

void set(unsigned int pos, signed int value){
    *(short *) (memory + pos) = value;
}

short get(unsigned int pos){
    return *(short *) (memory + pos);
}

void flip(unsigned int pos){
    set(pos, -get(pos));
}

void print(){
    for(int i = 0; i < 100; i += 2){
        printf("%2d: %d\n", i, get(i));
    }
    printf("\n");
}

void *memory_alloc(unsigned int size){
    int diff, loc = -1, min = 1000;
    unsigned int p_head = get(sizeof(short));
    int len = - get(p_head);
    int next = get(p_head + 2 * sizeof(short));
    
    if(p_head == 0) {
        printf("Nenašiel sa žiadny voľný blok\n");
        return NULL;
    }
    
    while(1){
        if((len - size) >= 0){
            diff = len - (size + 2 * sizeof(short));
            
            if(diff < min){
                loc = p_head;
            }
        }
        
        if(next > 0){
            p_head = next;
            len = - get(p_head);
            next = get(p_head + 2 * sizeof(short));
        }
        
        else
            break;
    }
    
    if(loc < 0){
        printf("Nenašiel sa žiadny dostatočne veľký voľný blok\n");
        return NULL;
    }
    
    p_head = loc;
    len = - get(p_head);
    unsigned int p_foot = p_head + len + sizeof(short);
    int prev = get(p_head + sizeof(short));
    next = get(p_head + 2 * sizeof(short));
    
    int new_len = - (len - (size + 2 * sizeof(short)));
    unsigned int head = p_head;
    unsigned int foot = p_head + size + sizeof(short);
    
    if(foot < 98){
        set(head, size);
        set(foot, size);
        set(foot + sizeof(short), new_len);
        set(p_foot, new_len);
        
        if(prev < 0)
            set(sizeof(short), foot + sizeof(short));
        else
            set(prev + sizeof(short), foot + sizeof(short));
        
        set(foot + 2 * sizeof(short), prev);
        set(foot + 3 * sizeof(short), next); //treba ošetriť keď som na konci poľa
        
        if (next > 0) {
            set(next + 2 * sizeof(short), foot + sizeof(short));
        }
    }
    
    else{
        set(head, size);
        set(foot, size);
        
        if(prev < 0)
            set(sizeof(short), 0);
        else
            set(prev + sizeof(short), 0);
        
        
        if (next > 0) {
            set(next + 2 * sizeof(short), foot + sizeof(short));
        }
    }
    
    return memory + p_head + sizeof(short);
}

int memory_free(void *valid_ptr){
    if(valid_ptr - sizeof(short) < 0){
        printf("Blok nebol alokovaný\n");
        return 1;
    }
    
    return 0;
}

int memory_check(void *ptr){
    if (ptr > (memory + *(short *) memory)){
        return 0;
    }
    int head = get(ptr - memory - sizeof(short));
    
    if (head < 0){
        return 0;
    }
    int foot = get(ptr - memory + head);
    
    if (foot < 0 || head != foot){
        return 0;
    }
    
    return 1;
}

void memory_init(void *ptr, unsigned int size){
    memory = ptr;
    
    size = size - (size % 2);
    
    set(0, size);
    set(sizeof(short), 2 * sizeof(short));
    set(2 * sizeof(short), -(size - 4 * sizeof(short)));
    set(3 * sizeof(short), -1);
    set(4 * sizeof(short), -1);
    set(get(0) - sizeof(short), -(size - 4 * sizeof(short)));
    
    for(int i = 5 * sizeof(short); i < size - sizeof(short); i += sizeof(short)){
        set(i, 0);
    }
    
    print();
}

int main() {
    char region[100];
    memory_init(region, 100);
    
    char *pointer = (char*) memory_alloc(10);
    print();
    char *p = (char *) memory_alloc(20);
    print();
    char *p2 = (char *) memory_alloc(30);
    print();
    char *p3 = (char *) memory_alloc(20);
    print();
    char *p4 = (char *) memory_alloc(20);
    
    if(pointer)
        memset(pointer, 0, 10);
    
    if(pointer)
        memory_free(pointer);
    
    return 0;
}
