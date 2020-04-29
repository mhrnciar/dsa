//
//  main.c
//  Popolvar
//  Matej Hrnciar
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mapa.h"

typedef struct cesta{
    int a;
    int b;
    int len;
    struct cesta *prev;
} CESTA;

typedef struct front{
    int len;
    struct front *next;
    MAPY *mapa;
    CESTA *cesta;
} FRONT;

CESTA *cesty;
MAPY *mapy;

void addToFront(FRONT **front, char **mapa, int a, int b){
    FRONT *akt = *front, *new = NULL;
    int move;
    
    if(mapa[a][b] == 'N')
        return;
    else if(mapa[a][b] == 'H')
        move = 2;
    else
        move = 1;
    
    new = (FRONT *) malloc(sizeof(FRONT));
    new->cesta = (CESTA *) malloc(sizeof(CESTA));
    new->cesta->a = a;
    new->cesta->b = b;
    new->cesta->prev = (*front)->cesta;
    new->len = (*front)->len + move;
    
    /*
     * Prejde sa front a zisti sa, ci do daneho bodu uz nevedie cesta. Ak ano, zisti sa ci je kratsia ako
     * ta, ktora bola prave vytvorena a podla toho sa bud predosla cesta ponecha alebo vymeni za kratsiu.
     */
    while(akt->next != NULL){
        if(akt->next->cesta->a == a && akt->next->cesta->b == b){
            if(akt->next->len > new->len){
                akt->next = akt->next->next;
                break;
            }
            else if(akt->next->len <= new->len)
                return;
        }
        akt = akt->next;
    }
    
    akt = (*front);
    
    while (akt->next != NULL) {
        if (new->len < akt->next->len) {
            break;
        }
        akt = akt->next;
    }
    
    new->next = akt->next;
    akt->next = new;
}

void dijkstra(int n, int m, int pocetP){
    char navstivene[n][m];
    int a = 0, b = 0, pomPocet = 0, poloha = 0;
    FRONT *front = NULL, *temp = NULL;
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            navstivene[i][j] = '0';
        }
    }
    
    front = NULL;
    front = malloc(sizeof(FRONT));
    front->cesta = (CESTA *) malloc(sizeof(CESTA));
    front->cesta->a = a;
    front->cesta->b = b;
    front->cesta->prev = NULL;
    front->mapa->mapa = mapy[0].mapa;
    front->len = 0;
    front->next = NULL;
    
    cesty = malloc(32 * sizeof(CESTA));
    for(int i = 0; i < 32; i++){
        cesty[i].len = -1;
    }
    pomPocet = pocetP;
    
    while(front != NULL){
        a = front->cesta->a;
        b = front->cesta->b;
        navstivene[a][b] = '1';
        
        if((pocetP == 0) && (mapy[0].mapa[a][b] == 'D')){
            cesty[0] = *front->cesta;
            cesty[0].len = front->len;
            return;
        }
        else if(pocetP == 1){
            if(front->mapa->mapa[a][b] == 'D'){
                front->mapa->mapa = mapy[1].mapa;
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = '0';
                    }
                }
            }
            else if(front->mapa->mapa[a][b] == '1'){
                cesty[0] = *front->cesta;
                cesty[0].len = front->len;
                return;
            }
        }
        
        else if(pocetP == 2){
            if(front->mapa->mapa[a][b] == 'D'){
                front->mapa->mapa = mapy[1].mapa;
            }
            else if(front->mapa->mapa[a][b] == '1'){
                pomPocet--;
                if(pomPocet == 0){
                    while(cesty[poloha].len != -1){
                        poloha++;
                    }
                    cesty[poloha] = *front->cesta;
                    cesty[poloha].len = front->len;
                    return;
                }
                front->mapa->mapa = mapy[2].mapa;
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = '0';
                    }
                }
            }
            else if(front->mapa->mapa[a][b] == '2'){
                pomPocet--;
                if(pomPocet == 0){
                    while(cesty[poloha].len != -1){
                        poloha++;
                    }
                    cesty[poloha] = *front->cesta;
                    cesty[poloha].len = front->len;
                    return;
                }
                front->mapa->mapa = mapy[3].mapa;
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = '0';
                    }
                }
            }
        }
        else if(pocetP == 3){
            if(front->mapa->mapa[a][b] == 'D'){
                front->mapa->mapa = mapy[1].mapa;
            }
        }
        else if(pocetP == 4){
            if(front->mapa->mapa[a][b] == 'D'){
                front->mapa->mapa = mapy[1].mapa;
            }
        }
        else if(pocetP == 5){
            if(front->mapa->mapa[a][b] == 'D'){
                front->mapa->mapa = mapy[1].mapa;
            }
        }
        
        /*
         * Prechadza sa mapa podla toho, ci uz dane policko bolo navstivene alebo nie. Treba pokryt velke
         * mnozstvo scenarov, napr. ked sa nachadzam na hornej hrane, nemozem prehladavat policka nad lebo
         * neexistuju, z toho isteho dovodu ak sa nachadzam na lavej hrane, nemozem prehladavat policka
         * vlavo a pod.
         */
        if(a == 0){
            // horna hrana - vzdy mozem prehladavat nizsi rad
            if(navstivene[a+1][b] == '0')
                addToFront(&front, front->mapa->mapa, a+1, b);
            
            if(b == 0){
                // lavy horny roh - mozem ist iba doprava
                if(navstivene[a][b+1] == '0')
                    addToFront(&front, front->mapa->mapa, a, b+1);
            }
            else if(b == (m-1)){
                // pravy horny roh - mozem ist iba dolava
                if(navstivene[a][b-1] == '0')
                    addToFront(&front, front->mapa->mapa, a, b-1);
            }
            else{
                // zvysok hornej hrany - mozem ist doparava aj dolava
                if(navstivene[a][b+1] == '0')
                    addToFront(&front, front->mapa->mapa, a, b+1);
                if(navstivene[a][b-1] == '0')
                    addToFront(&front, front->mapa->mapa, a, b-1);
            }
        }
        else if(a == (n-1)){
            // dolna hrana - vzdy mozem prehladavat vyssi rad
            if(navstivene[a-1][b] == '0')
                addToFront(&front, front->mapa->mapa, a-1, b);
            
            if(b == 0){
                // lavy dolny roh - mozem ist iba doprava
                if(navstivene[a][b+1] == '0')
                    addToFront(&front, front->mapa->mapa, a, b+1);
            }
            else if(b == (m-1)){
                // pravy dolny roh - mozem ist iba dolava
                if(navstivene[a][b-1] == '0')
                    addToFront(&front, front->mapa->mapa, a, b-1);
            }
            else{
                // zvysok dolnej hrany - mozem ist doprava aj dolava
                if(navstivene[a][b+1] == '0')
                    addToFront(&front, front->mapa->mapa, a, b+1);
                if(navstivene[a][b-1] == '0')
                    addToFront(&front, front->mapa->mapa, a, b-1);
            }
        }
        else if(b == 0){
            // lava hrana - mozem ist doprava, hore alebo dole
            if(navstivene[a+1][b] == '0')
                addToFront(&front, front->mapa->mapa, a+1, b);
            if(navstivene[a-1][b] == '0')
                addToFront(&front, front->mapa->mapa, a-1, b);
            if(navstivene[a][b+1] == '0')
                addToFront(&front, front->mapa->mapa, a, b+1);
        }
        else if(b == (m-1)){
            // prava hrana - mozem ist dolava, hore alebo dole
            if(navstivene[a+1][b] == '0')
                addToFront(&front, front->mapa->mapa, a+1, b);
            if(navstivene[a-1][b] == '0')
                addToFront(&front, front->mapa->mapa, a-1, b);
            if(navstivene[a][b-1] == '0')
                addToFront(&front, front->mapa->mapa, a, b-1);
        }
        else{
            // uprostred - mozem ist do vsetkych styroch smerov
            if(navstivene[a+1][b] == '0')
                addToFront(&front, front->mapa->mapa, a+1, b);
            if(navstivene[a-1][b] == '0')
                addToFront(&front, front->mapa->mapa, a-1, b);
            if(navstivene[a][b+1] == '0')
                addToFront(&front, front->mapa->mapa, a, b+1);
            if(navstivene[a][b-1] == '0')
                addToFront(&front, front->mapa->mapa, a, b-1);
        }
        
        // Pociatocne policko vynecham z fronty a pokracujem dalej
        temp = front;
        if(front != NULL){
            front = front->next;
            free(temp);
        }
    }
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty){
    int pocetP = 0;
    mapy = createMaps(mapa, n, m);
    
    if(mapy == NULL){
        return NULL;
    }
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(mapy[0].mapa[i][j] >= '1' && mapy[0].mapa[i][j] <= '5'){
                pocetP++;
            }
        }
    }
    
    dijkstra(n, m, pocetP);
    
    int *p = NULL;
    return p;
}

int main() {
    char **mapa = NULL;
    int i, test, dlzka_cesty, cas, *cesta = NULL;
    int n = 0, m = 0, t = 0;
    FILE *f;
    
    while(1){
        printf("Zadajte cislo testu:\n0: Ukoncenie programu\n");
        printf("1: Nacitanie testu zo suboru \n2: Mapa 10 x 10 \n3: Mapa 20 x 15 \n4-7: Testy s neriesitelnym problemom\n");
        scanf("%d", &test);
        dlzka_cesty = 0;
        n = m = t = 0;
        
        switch(test){
            case 0://ukonci program
                return 0;
                break;
                
            case 1://nacitanie mapy zo suboru
                f = fopen("test.txt","r");
                if(f)
                    fscanf(f, "%d %d %d", &n, &m, &t);
                else
                    continue;
                
                mapa = (char **) malloc(n * sizeof(char*));
                for(i = 0; i < n; i++){
                    mapa[i] = (char *) malloc(m * sizeof(char));
                    for(int j = 0; j < m; j++){
                        char policko = fgetc(f);
                        if(policko == '\n')
                            policko = fgetc(f);
                        
                        mapa[i][j] = policko;
                    }
                }
                fclose(f);
                
                cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
                
            case 2://nacitanie preddefinovanej mapy n = 10;
                n = 10;
                m = n;
                t = 12;
                mapa = (char **) malloc(n * sizeof(char *));
                mapa[0]="CCHCNHCCHN";
                mapa[1]="NNCCCHHCCC";
                mapa[2]="DNCCNNHHHC";
                mapa[3]="CHHHCCCCCC";
                mapa[4]="CCCCCNHHHH";
                mapa[5]="PCHCCCNNNN";
                mapa[6]="NNNNNHCCCC";
                mapa[7]="CCCCCPCCCC";
                mapa[8]="CCCNNHHHHH";
                mapa[9]="HHHPCCCCCC";
                
                cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
                
            case 3:
                n = 10;
                m = 10;
                t = 12;
                mapa = (char**) malloc(n * sizeof(char*));
                mapa[0]="CCHCNHCCHN";
                mapa[1]="NNCCCHHCCC";
                mapa[2]="DNCCNNHHHC";
                mapa[3]="NHHHCCCCPC";
                mapa[4]="CCCCCNHHHH";
                mapa[5]="PCHCCCNNNN";
                mapa[6]="NNNNNHCCCC";
                mapa[7]="CCCCCPCCCC";
                mapa[8]="CCCNNHHHHH";
                mapa[9]="HHHPCCCCCC";
                
                printf("Test s drakom obkolesenym nepriechodnymi polickami\n");
                cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
                
            case 4:
                n = 10;
                m = n;
                t = 12;
                mapa = (char **) malloc(n * sizeof(char *));
                mapa[0]="CCHCNHCCHN";
                mapa[1]="NNCCCHHCCC";
                mapa[2]="DNCCNNHHHC";
                mapa[3]="NHHHCCCCCC";
                mapa[4]="CCCCCNHHHH";
                mapa[5]="PCHCCCNNNN";
                mapa[6]="NNNNNHCCCC";
                mapa[7]="CCCCCPCCCC";
                mapa[8]="CCCNNHHHHH";
                mapa[9]="HHHPCCCCCC";
                
                printf("Test s drakom obkolesenym nepriechodnymi polickami\n");
                cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
                
            case 5:
                n = 10;
                m = n;
                t = 12;
                mapa = (char **) malloc(n * sizeof(char *));
                mapa[0]="CCHCNHCCHN";
                mapa[1]="NNCCCHHCCC";
                mapa[2]="NNCCNNHHHC";
                mapa[3]="NHHHCCCCCC";
                mapa[4]="CCCCCNHHHH";
                mapa[5]="PCHCCCNNNN";
                mapa[6]="NNNNNHCCCC";
                mapa[7]="CCCCCPCCCC";
                mapa[8]="CCCNNHHHHH";
                mapa[9]="HHHPCCCCCC";
                
                printf("Test bez draka\n");
                cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
                
            case 6:
                n = 10;
                m = n;
                t = 12;
                mapa = (char **) malloc(n * sizeof(char *));
                mapa[0]="CCHCNHCCHN";
                mapa[1]="NNCCCHHCCC";
                mapa[2]="DNCCNNHHHC";
                mapa[3]="CHHHCCCCCC";
                mapa[4]="CCCCCNHHHH";
                mapa[5]="PCHCCCNNNN";
                mapa[6]="NNNNNHCCCC";
                mapa[7]="CCCCCCCCCC";
                mapa[8]="CCCNNHHHHH";
                mapa[9]="HHHPCCCCCC";
                
                printf("Test s dvomi princeznami\n");
                cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
                
            case 7:
                n = 10;
                m = n;
                t = 12;
                mapa = (char **) malloc(n * sizeof(char *));
                mapa[0]="CCHCNHCCHN";
                mapa[1]="NNCCCHHCCC";
                mapa[2]="DNCCNNHHHC";
                mapa[3]="CHHHCCCCCC";
                mapa[4]="NCCCCNHHHH";
                mapa[5]="PNHCCCNNNN";
                mapa[6]="NNNNNHCCCC";
                mapa[7]="CCCCCPCCCC";
                mapa[8]="CCCNNHHHHH";
                mapa[9]="HHHPCCCCCC";
                
                printf("Test s princeznou obkolesenou nepriechodnymi polickami\n");
                cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
                
            default:
                continue;
        }
        
        if(cesta != NULL){
            cas = 0;
            for(i = 0; i < dlzka_cesty; i++){
                printf("%d %d\n", cesta[i*2], cesta[i*2+1]);
                
                if(mapa[cesta[i*2+1]][cesta[i*2]] == 'H')
                    cas += 2;
                else
                    cas += 1;
                
                if(mapa[cesta[i*2+1]][cesta[i*2]] == 'D' && cas > t)
                    printf("Nestihol si zabit draka!\n");
                
                if(mapa[cesta[i*2+1]][cesta[i*2]] == 'N')
                    printf("Prechod cez nepriechodnu prekazku!\n");
                
                if(i > 0 && abs(cesta[i*2+1] - cesta[(i-1)*2+1]) + abs(cesta[i*2] - cesta[(i-1)*2]) > 1)
                    printf("Neplatny posun Popolvara!\n");
            }
            
            printf("Dlzka cesty: %d\n\n", cas);
        }
        free(cesta);
    }
    return 0;
}

