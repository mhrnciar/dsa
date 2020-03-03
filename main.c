//
//  Projekt 1
//  Matej Hrnčiar
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
    set(pos, - get(pos));
}

void print(int size){
    for(int i = 0; i < size; i += 2){
        printf("%2d: %d\n", i, get(i));
    }
    printf("\n");
}

void *memory_alloc(unsigned int size){
    int diff = get(0), min = get(0), loc = -1;
    unsigned int p_head = get(sizeof(short));
    int len = - get(p_head);
    int next = get(p_head + 2 * sizeof(short));
    
    if(p_head == 0) {
        printf("Nenašiel sa žiadny voľný blok\n");
        return NULL;
    }
    
    while(1){
        if((len - size) == 0){
            loc = p_head;
            break;
        }
        else if(len - size >= 0 && len - size > 6 * sizeof(short)){
            diff = len - size;
            if(diff < min){
                min = diff;
                loc = p_head;
            }
        }
        
        if(next > 0){
            p_head = next;
            len = - get(p_head);
            next = get(p_head + 2 * sizeof(short));
        }
        else{
            break;
        }
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
    
    if(diff == 0){
        flip(p_head);
        flip(p_foot);
        
        set(sizeof(short), next);
        
        if(next > 0)
            set(next + sizeof(short), prev);
    }
    
    else{
        int new_len = - (len - (size + 2 * sizeof(short)));
        unsigned int head = p_head;
        unsigned int foot = p_head + size + sizeof(short);
        
        if(foot < get(0) - sizeof(short)){
            set(head, size);
            set(foot, size);
            set(foot + sizeof(short), new_len);
            set(p_foot, new_len);
            
            if(prev < 0)
                set(sizeof(short), foot + sizeof(short));
            else
                set(prev + sizeof(short), foot + sizeof(short));
            
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
    }
    
    return memory + p_head + sizeof(short);
}

int memory_free(void *valid_ptr){
    if(valid_ptr == NULL){
        return 1;
    }
    
    int head = valid_ptr - memory - sizeof(short);
    int size = get(head);
    int foot = head + size + sizeof(short);
    
    if(size < 0){
        printf("Blok nebol alokovaný\n");
        return 1;
    }
    
    flip(head);
    flip(foot);
    
    if(sizeof(short) == 0){
        set(sizeof(short), head);
        set(valid_ptr - memory, -1);
        set(valid_ptr - memory + sizeof(short), -1);
        
        return 0;
    }
    
    int next_head = foot + sizeof(short);
    int prev_foot = head - sizeof(short);
    
    if(next_head < get(0) && prev_foot > sizeof(short) && get(prev_foot) < 0 && get(next_head) < 0){
        int prev_size = - get(prev_foot);
        int next_size = - get(next_head);
        int new_size = prev_size + size + next_size + 4 * sizeof(short);
        int next_foot = next_head + next_size + sizeof(short);
        int prev_head = prev_foot - prev_size - sizeof(short);
        int next_next = get(next_head + 2 * sizeof(short));
        
        set(prev_head, - new_size);
        set(next_foot, - new_size);
        set(prev_head + 2 * sizeof(short), next_next);
        
        if (next_next > 0) {
            set(next_next - sizeof(short), prev_head);
        }
        for(int i = prev_head + 3 * sizeof(short); i < next_foot - sizeof(short); i += 2) {
            set(i, 0);
        }
        
        return 0;
    }
    
    else if(next_head < get(0) && get(next_head) < 0){
        int next_size = - get(next_head);
        int new_size = size + next_size + 2 * sizeof(short);
        int next_foot = next_head + next_size + sizeof(short);
        int next_prev = get(next_head + sizeof(short));
        int next_next = get(next_head + 2 * sizeof(short));
        
        set(head + sizeof(short), next_prev);
        set(head + 2 * sizeof(short), next_next);
        
        set(head, - new_size);
        set(next_foot, - new_size);
        
        if (next_next > 0)
            set(next_next + sizeof(short), head);
            
        if (next_prev > 0)
            set(next_prev + 2 * sizeof(short), head);

        if (get(sizeof(short)) == next_head)
            set(sizeof(short), head);
        
        for(int i = head + 3 * sizeof(short); i < next_foot - sizeof(short); i += 2) {
            set(i, 0);
        }
        
        return 0;
    }
    
    else if(prev_foot > sizeof(short) && get(prev_foot) < 0){
        int prev_size = - get(prev_foot);
        int new_size = size + prev_size + 2 * sizeof(short);
        int prev_head = prev_foot - prev_size - sizeof(short);
        
        set(prev_head, - new_size);
        set(head + size + sizeof(short), - new_size);
        
        for(int i = prev_head + 3 * sizeof(short); i < head + size; i += 2) {
            set(i, 0);
        }
        
        return 0;
    }
    
    set(head + sizeof(short), -1);
    set(head + 2 * sizeof(short), -1);
    
    if(head < get(sizeof(short))){
        set(get(sizeof(short)) + sizeof(short), head);
        set(head + sizeof(short), -1);
        set(head + 2 * sizeof(short), get(sizeof(short)));
        set(sizeof(short), head);
    }
    
    else{
        int akt = get(sizeof(short));
        
        while(1){
            int pom = get(akt + sizeof(short));
            
            if(pom > 0){
                if(pom < head){
                    akt = pom;
                    continue;
                }
                
                else{
                    set(akt + 2 * sizeof(short), head);
                    set(head + sizeof(short), akt);
                    set(head + 2 * sizeof(short), pom);
                    set(pom + sizeof(short), head);
                    break;
                }
            }
            
            else{
                set(akt + sizeof(short), head);
                set(head + sizeof(short), akt);
                set(head + 2 * sizeof(short), -1);
                break;
            }
        }
    }
    
    return 1;
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
    
    print(size);
}

int main() {
    char region[100];
    memory_init(region, 100);
    
    int size = sizeof(region) / sizeof(region[0]);
    
    char *pointer = (char*) memory_alloc(10);
    print(size);
    char *p = (char *) memory_alloc(20);
    print(size);
    char *p2 = (char *) memory_alloc(20);
    print(size);
    char *p3 = (char *) memory_alloc(20);
    print(size);
    
    memory_free(p);
    print(size);
    memory_free(p3);
    print(size);
    char *p4 = (char *) memory_alloc(20);
    print(size);
    
    if(pointer)
        memset(pointer, 0, 10);
    
    if(pointer)
        memory_free(pointer);
    
    return 0;
}
