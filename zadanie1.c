//
//  Projekt 1
//  Matej Hrnčiar
//

#include <stdio.h>
#include <string.h>

void *memory;

void set(unsigned short pos, short value){
    *(short *) (memory + pos) = value;
}

short get(unsigned short pos){
    return *(short *) (memory + pos);
}

void print(int size){
    for(int i = 0; i < size; i += 2){
        printf("%2d: %d\n", i, get(i));
    }
    printf("\n");
}

/*
 * memory_alloc() hlada volny blok na zaklade best-fit. Najprv sa nastavim na prvy volny blok,
 * na ktory ukazuje cislo s offsetom 2, teda jeden sizeof(short) a ak je volny blok presne
 * rovnako velky, tak prejde k alokacii pola. Ak je vacsi a dostatocne velky na to, aby sa dal
 * rozdelit, zapise rozdiel velkosti a pokracuje na dalsi volny blok. Po prejdeni celeho pola
 * alokuje blok na mieste, ktory sa velkostou najblizsie rovna pozadovanej velkosti pola. Funkcia
 * nevracia smernik na hlavicku, ale priamo na prvy vyuzitelny bajt v bloku.
 */
void *memory_alloc(unsigned int size){
    int diff = get(0), min = get(0), loc = -1;
    unsigned int p_head = get(sizeof(short));
    unsigned int len = get(p_head);
    int next = get(p_head + 2 * sizeof(short));
    
    if(p_head == 0) {
        printf("Nenašiel sa žiadny voľný blok\n");
        return NULL;
    }
    
    while(1){
        // Ak sa blok presne zmesti do volneho bloku
        if((len - size) == 0){
            loc = p_head;
            break;
        }
        // Ak sa blok zmesti do volneho bloku a da sa rozdelit
        else if(len - size >= 0 && len - size > 6 * sizeof(short)){
            diff = len - size;
            if(diff < min){
                min = diff;
                loc = p_head;
            }
        }
        // Posun na dalsi volny blok
        if(next > 0){
            p_head = next;
            len = get(p_head);
            next = get(p_head + 2 * sizeof(short));
        }
        else{
            break;
        }
    }
    
    if(loc < 0){
        printf("Nenašiel sa žiadny dostatočne veľký voľný blok, ktorý by sa dal rozdeliť\n");
        return NULL;
    }
    
    p_head = loc;
    len = get(p_head);
    diff = len - size;
    unsigned int p_foot = p_head + len + sizeof(short);
    int prev = get(p_head + sizeof(short));
    next = get(p_head + 2 * sizeof(short));
    
    // Ak sa pozadovany blok presne zmesti do volneho bloku
    if(diff == 0){
        set(p_head, size + 1);
        set(p_foot, size + 1);
        set(p_head + sizeof(short), prev);
        set(p_head + 2 * sizeof(short), next);
        
        if(prev < 0)
            set(sizeof(short), 0);
        else
            set(prev + sizeof(short), p_head);
        
        if(next > 0)
            set(next + sizeof(short), prev);
        
        for(int i = p_head + 3 * sizeof(short); i < p_foot; i += sizeof(short)){
            set(i, 0);
        }
    }
    
    // Ak sa blok zmesti do volneho bloku a da sa rozdelit
    else{
        int new_len = len - (size + 2 * sizeof(short));
        unsigned int head = p_head;
        unsigned int foot = p_head + size + sizeof(short);
        
        // Ak alokovany blok nesiaha na uplny koniec pamate
        if(foot < get(0) - sizeof(short)){
            set(head, size + 1);
            set(foot, size + 1);
            set(foot + sizeof(short), new_len);
            set(foot + 2 * sizeof(short), prev);
            set(foot + 3 * sizeof(short), next);
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
            set(head, size + 1);
            set(foot, size + 1);
            
            if(prev < 0)
                set(sizeof(short), 0);
            else
                set(prev + sizeof(short), head);
            
            
            if (next > 0) {
                set(next + 2 * sizeof(short), foot + sizeof(short));
            }
        }
        
        for(int i = head + 3 * sizeof(short); i < foot; i += sizeof(short)){
            set(i, 0);
        }
    }
    return memory + p_head + sizeof(short);
}

/*
 * memory_free() dostane smernik na prvy vyuzitelny bajt v bloku, takze hlavicka sa nachadza o
 * jeden sizeof(short) dozadu. Najprv premeni neparne hodnoty v hlavicke a paticke na parne
 * cim da najavo ze blok bol dealokovany, a potom spaja bloky podla styroch moznych scenarov:
 *  1. dealokovany blok je obkoleseny volnymi blokmi: vtedy sa vsetky tri bloky spoja, scitaju
 *  sa ich velkosti a upravia sa ukazovatele na dalsie volne bloky
 *  2. za dealokovanym blokom nasleduje volny blok
 *  3. dealokovanemu bloku predchadza volny blok
 *  4. okolo nie su ziadne volne bloky
 * Pri kazdom pripade (okrem 4) sa velkosti blokov scitaju a nova dlzka sa zapise do hlavicky a
 * paticky noveho velkeho bloku a upravia sa smerniky (okrem pripadu 3), ktore ukazuju na predosle a nasledujuce
 * volne bloky. Funkcia vrati 0, ak sa dealokacia podarila, 1 ak sa nepodarila.
 */
int memory_free(void *valid_ptr){
    if(valid_ptr == NULL){
        printf("Neplatný smerník\n");
        return 1;
    }
    
    int head = valid_ptr - memory - sizeof(short);
    int size = get(head);
    
    if(size % 2 == 0){
        printf("Blok nebol alokovaný\n");
        return 1;
    }
    
    size -= 1;
    int foot = head + size + sizeof(short);
    
    set(head, size);
    set(foot, size);
    
    if(get(sizeof(short)) == 0){
        set(sizeof(short), head);
        set(head + sizeof(short), -1);
        set(head + 2 * sizeof(short), -1);
        
        return 0;
    }
    
    int next_head = foot + sizeof(short);
    int prev_foot = head - sizeof(short);
    
    // Ak je uvolnovany blok obkoleseny volnymi blokmi
    if(next_head < get(0) && prev_foot > sizeof(short) && get(prev_foot) % 2 == 0 && get(next_head) % 2 == 0){
        int prev_size = get(prev_foot);
        int next_size = get(next_head);
        int new_size = prev_size + size + next_size + 4 * sizeof(short);
        int next_foot = next_head + next_size + sizeof(short);
        int prev_head = prev_foot - prev_size - sizeof(short);
        int next_next = get(next_head + 2 * sizeof(short));
        
        set(prev_head, new_size);
        set(next_foot, new_size);
        set(prev_head + 2 * sizeof(short), next_next);
        
        if (next_next > 0) {
            set(next_next + sizeof(short), prev_head);
        }
        
        for(int i = prev_head + 3 * sizeof(short); i < next_foot; i += sizeof(short)) {
            set(i, 0);
        }
        
        return 0;
    }
    
    // Ak za uvolnovanym blokom nasleduje volny blok
    else if(next_head < get(0) && get(next_head) % 2 == 0){
        int next_size = get(next_head);
        int new_size = size + next_size + 2 * sizeof(short);
        int next_foot = next_head + next_size + sizeof(short);
        int next_prev = get(next_head + sizeof(short));
        int next_next = get(next_head + 2 * sizeof(short));
        
        set(head + sizeof(short), next_prev);
        set(head + 2 * sizeof(short), next_next);
        
        set(head, new_size);
        set(next_foot, new_size);
        
        if (next_next > 0)
            set(next_next + sizeof(short), head);
            
        if (next_prev > 0)
            set(next_prev + 2 * sizeof(short), head);

        if (get(sizeof(short)) == next_head)
            set(sizeof(short), head);
        
        for(int i = head + 3 * sizeof(short); i < next_foot; i += sizeof(short)) {
            set(i, 0);
        }
        
        return 0;
    }
    
    // Ak uvolnovanemu bloku predchadza volny blok
    else if(prev_foot > sizeof(short) && get(prev_foot) % 2 == 0){
        int prev_size = get(prev_foot);
        int new_size = size + prev_size + 2 * sizeof(short);
        int prev_head = prev_foot - prev_size - sizeof(short);
        
        set(prev_head, new_size);
        set(head + size + sizeof(short), new_size);
        
        for(int i = prev_head + 3 * sizeof(short); i < head + size; i += sizeof(short)) {
            set(i, 0);
        }
        
        return 0;
    }
    
    // Ak je uvolneny blok prvy v poradi
    if(head < get(sizeof(short))){
        set(get(sizeof(short)) + sizeof(short), head);
        set(head + sizeof(short), -1);
        set(head + 2 * sizeof(short), get(sizeof(short)));
        set(sizeof(short), head);
    }
    
    else{   //!!!!!!!!!!!!!!!!!
        int akt = get(sizeof(short));
        
        while(1){
            int pom = get(akt + 2 * sizeof(short));
            
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
                set(akt + 2 * sizeof(short), head);
                set(head + sizeof(short), akt);
                set(head + 2 * sizeof(short), -1);
                break;
            }
        }
    }
    
    return 1;
}

/*
 * memory_check() skontroluje, ci smernik ukazuje na alokovany blok v pamati.Zistuje to
 * pomocou hodnoty zapisanej v hlavicke, ak je cislo neparne, blok je alokovany, a teda
 * funkcia vrati 1. Ak je parne, tak ide o volny blok a funkcia vrati 0.
 */
int memory_check(void *ptr){
    int head = get(ptr - memory - sizeof(short));
    if(head % 2 == 1){
        printf("Smerník ukazuje na alokovaný blok\n");
        return 1;
    }
    printf("Smerník neukazuje na alokovaný blok\n");
    return 0;
}

/*
 * Pri memory_init() si nasmerujem smernik na pole a urcim si hlavicku, paticku a prvy volny
 * blok. Na 0-tom mieste je zapisana velkost celeho pola, na offsete 2 je ukazovatel na prvy
 * volny blok (na zaciatku je to 4). Na offsete 4 je hlavicka volneho bloku, ktora ukazuje
 * velkost volneho bloku a na offsete size - sizeof(short) je paticka, ktora taktiez ukazuje
 * velkost volneho bloku. Volne bloky su vyjadrene v parnych cislach a alokovane v nepanych,
 * konkretne ak alokujem pole velkosti 20, tak v hlavicke bude zapisana hodnota 21.
 */
void memory_init(void *ptr, unsigned int size){
    memory = ptr;
    
    size = size - (size % 2);
    
    set(0, size);
    set(sizeof(short), 2 * sizeof(short));
    set(2 * sizeof(short), (size - 4 * sizeof(short)));
    set(3 * sizeof(short), -1);
    set(4 * sizeof(short), -1);
    set(get(0) - sizeof(short), (size - 4 * sizeof(short)));
    
    for(int i = 5 * sizeof(short); i < size - sizeof(short); i += sizeof(short)){
        set(i, 0);
    }
    
    print(size);
}

int main() {
    char region[500];
    int size = sizeof(region) / sizeof(region[0]);
    
    memory_init(region, 500);
    
    char *pointer = (char*) memory_alloc(50);
    print(size);
    char *pointer2 = (char*) memory_alloc(50);
    print(size);
    char *pointer3 = (char*) memory_alloc(50);
    print(size);
    char *pointer4 = (char*) memory_alloc(50);
    print(size);
    char *pointer5 = (char*) memory_alloc(50);
    print(size);
    char *pointer6 = (char*) memory_alloc(50);
    print(size);
    
    memory_check(pointer3);
    
    memory_free(pointer4);
    print(size);
    memory_free(pointer3);
    print(size);
    memory_free(pointer);
    print(size);
    
    return 0;
}
