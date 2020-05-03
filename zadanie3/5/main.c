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
    int mrtvyDrak;
    int pocetP;
    struct front *next;
    MAPY *mapa;
    CESTA *cesta;
} FRONT;

CESTA *cesta = NULL;
MAPY *mapy;

static int N;
static int M;

char **setmapa(char mapa[N][M]){
    char **pom = (char **) malloc(N * sizeof(char *));
    for(int i = 0; i < N; i++){
        *pom = (char *) malloc(M * sizeof(char));
    }
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            pom[i] = strdup(mapa[i]);
        }
    }
    return pom;
}

/*
 * Ak sa nasla ina cesta, zistuje sa, ci je vkladana kratsia ako tak ktora tam uz bola vlozena. Ak je kratsia,
 * tak sa ulozi namiesto nej, v opacnom pripade sa zahodi.
 */
void ulozCestu(FRONT *front){
    if(cesta == NULL){
        cesta = malloc(sizeof(CESTA));
        cesta = front->cesta;
        cesta->len = front->len;
    }
    else{
        if(front->len < cesta->len){
            cesta = front->cesta;
            cesta->len = front->len;
        }
    }
}

/*
 * Vkladanie noveho prvku do frontu - najprv sa zisti aky je teren, teda ci sa pripocita 1, 2 alebo ak ide o
 * nepriechodnu prekazku, tak sa nezapise prvok vobec. Potom sa prekopiruju informacie z predosleho prvku ako
 * napriklad ci uz v danej ceste bol najdeny drak alebo nejaka princezna, mapa ktora sa prave pouziva a taktiez
 * dlzka cesty s pripocitanou vzdialenostou na policko na ktorom prave Popolvar stoji. Nasledne sa prejde spajany
 * zoznam a zisti sa ci do daneho policka uz nevedie ina, dlhsia cesta. V tom pripade by sa nova cesta zapisala
 * namiesto nej. Nakoniec sa pole automaticky postupne zoraduje podla dlzky doposial vytvorenej cesty.
 */
void addToFront(FRONT **front, char **navstivene, int a, int b){
    FRONT *akt = *front, *new = NULL;
    int move;
    
    if(navstivene[a][b] == 'N')
        return;
    else if(navstivene[a][b] == 'H')
        move = 2;
    else
        move = 1;
    
    new = (FRONT *) malloc(sizeof(FRONT));
    new->mapa = malloc(sizeof(MAPY));
    new->mapa->mapa = navstivene;
    new->mapa->nazov = akt->mapa->nazov;
    new->cesta = (CESTA *) malloc(sizeof(CESTA));
    new->cesta->a = a;
    new->cesta->b = b;
    new->cesta->prev = (*front)->cesta;
    new->len = (*front)->len + move;
    new->mrtvyDrak = (*front)->mrtvyDrak;
    new->pocetP = (*front)->pocetP;
    
    /*
     * Prejde sa front a zisti sa, ci do daneho bodu uz nevedie cesta. Ak ano, zisti sa ci je kratsia ako
     * ta, ktora bola prave vytvorena a podla toho sa bud predosla cesta ponecha alebo vymeni za kratsiu.
     */
    while(akt->next != NULL){
        if(strcmp(akt->next->mapa->nazov, new->mapa->nazov) == 0 && akt->next->cesta->a == a && akt->next->cesta->b == b){
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
    
    while(akt->next != NULL){
        if(new->len < akt->next->len){
            break;
        }
        akt = akt->next;
    }
    
    new->next = akt->next;
    akt->next = new;
}

void dijkstra(int n, int m, int pocetP){
    char navstivene[n][m], iden2[2], iden3[3], iden4[4], iden5[5], *string;
    int a = 0, b = 0;
    FRONT *front = NULL, *temp = NULL;
    
    cesta = NULL;
    
    /*
     * Vytvori sa prvy prvok vo fronte s mapou s drakom aj princeznami, zapise sa pocet princezien a
     * informacia o tom, ze drak este nie je mrtvy, ako aj cesta, ktora ukazuje na policko [0,0].
     */
    front = malloc(sizeof(FRONT));
    front->mapa = malloc(sizeof(MAPY));
    front->cesta = (CESTA *) malloc(sizeof(CESTA));
    front->cesta->a = a;
    front->cesta->b = b;
    front->cesta->prev = NULL;
    front->len = 0;
    front->next = NULL;
    front->mrtvyDrak = 0;
    front->pocetP = pocetP;
    front->mapa->mapa = mapy[0].mapa;
    if(pocetP != 0)
        front->mapa->nazov = mapy[1].nazov;
    else
        front->mapa->nazov = mapy[0].nazov;
    
    string = malloc(pocetP * sizeof(char));
    
    /*
     * Funkcia bezi az kym sa nevyprazdni cely front. Vzdy sa na zaciatku skontroluje ci cesta vo
     * fronte uz nie je dlhsia ako ta, ktora uz bola zapisana. V tom pripade uz nehrozi, ze by sa
     * mohla zapisat a rovno sa vyhodi. Potom sa stiahne mapa prvku (ine prvky mozu mat ine mapy,
     * napr. ked v jednom sa drak uz nasiel, zatial co v druhom sa este stale hlada) a miesto na
     * mape, kde prave Popolvar stoji sa zmeni na '0', co naznacuje ze dane policko uz bolo navstivene.
     */
    while(front != NULL){
        if(cesta != NULL){
            while(front != NULL && front->len > cesta->len){
                temp = front;
                front = front->next;
                free(temp);
            }
            if(front == NULL){
                return;
            }
        }
        
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                navstivene[i][j] = front->mapa->mapa[i][j];
            }
        }
        
        if(pocetP == 2){
            for(int i = 0; i < 2; i++){
                iden2[i] = front->mapa->nazov[i];
            }
        }
        if(pocetP == 3){
            for(int i = 0; i < 3; i++){
                iden3[i] = front->mapa->nazov[i];
            }
        }
        else if(pocetP == 4){
            for(int i = 0; i < 4; i++){
                iden4[i] = front->mapa->nazov[i];
            }
        }
        else if(pocetP == 5){
            for(int i = 0; i < 5; i++){
                iden5[i] = front->mapa->nazov[i];
            }
        }
        
        a = front->cesta->a;
        b = front->cesta->b;
        navstivene[a][b] = '0';
        
        /*
         * Prebehne kontrola, ci Popolvar nestoji na policku s drakom alebo s princeznou, podla
         * toho sa do prvku frontu zapise nova mapa a znizi sa pocet princezien, ktore je este
         * potrebne najst. Ak sa najde drak, tak sa v prvku oznaci ze na danej ceste bol uz drak
         * porazeny. Ak sa najdu vsetky princezne, prejde sa k zapisovaniu cesty.
         */
        if((pocetP == 0) && (mapy[0].mapa[a][b] == 'D')){
            ulozCestu(front);
            free(front);
            front = NULL;
            return;
        }
        else if(pocetP == 1){
            if(front->mapa->mapa[a][b] == 'D'){
                front->mapa->mapa = mapy[1].mapa;
                front->mrtvyDrak = 1;
                front->next = NULL;
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
            }
            else if(front->mrtvyDrak == 1 && front->mapa->mapa[a][b] == '1'){
                ulozCestu(front);
                free(front);
                front = NULL;
                return;
            }
        }
        else if(pocetP == 2){
            if(front->mapa->mapa[a][b] == 'D'){
                front->mapa->nazov = mapy[1].nazov;
                front->mapa->mapa = mapy[1].mapa;
                front->mrtvyDrak = 1;
                front->next = NULL;
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
            }
            else if(front->mrtvyDrak == 1 && front->mapa->mapa[a][b] == '1'){
                front->pocetP--;
                if(front->pocetP == 0){
                    ulozCestu(front);
                    temp = front;
                    front = front->next;
                    free(temp);
                    continue;
                }
                iden2[0] = 'p';
                for(int i = 1; i < 4; i++){
                    memcpy(string, iden2, pocetP);
                    if(strcmp(mapy[i].nazov, string) == 0){
                        front->mapa->mapa = mapy[i].mapa;
                        front->mapa->nazov = mapy[i].nazov;
                        break;
                    }
                }
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
            }
            else if(front->mrtvyDrak == 1 && front->mapa->mapa[a][b] == '2'){
                front->pocetP--;
                if(front->pocetP == 0){
                    ulozCestu(front);
                    temp = front;
                    front = front->next;
                    free(temp);
                    continue;
                }
                iden2[1] = 'p';
                for(int i = 1; i < 4; i++){
                    memcpy(string, iden2, pocetP);
                    if(strcmp(mapy[i].nazov, string) == 0){
                        front->mapa->mapa = mapy[i].mapa;
                        front->mapa->nazov = mapy[i].nazov;
                        break;
                    }
                }
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
            }
        }
        else if(pocetP == 3){
            if(front->mapa->mapa[a][b] == 'D'){
                front->mapa->nazov = mapy[1].nazov;
                front->mapa->mapa = mapy[1].mapa;
                front->mrtvyDrak = 1;
                front->next = NULL;
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
            }
            else if(front->mrtvyDrak == 1 && front->mapa->mapa[a][b] == '1'){
                front->pocetP--;
                if(front->pocetP == 0){
                    ulozCestu(front);
                    temp = front;
                    front = front->next;
                    free(temp);
                    continue;
                }
                iden3[0] = 'p';
                for(int i = 1; i < 8; i++){
                    memcpy(string, iden3, pocetP);
                    if(strcmp(mapy[i].nazov, string) == 0){
                        front->mapa->mapa = mapy[i].mapa;
                        front->mapa->nazov = mapy[i].nazov;
                        break;
                    }
                }
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
            }
            else if(front->mrtvyDrak == 1 && front->mapa->mapa[a][b] == '2'){
                front->pocetP--;
                if(front->pocetP == 0){
                    ulozCestu(front);
                    temp = front;
                    front = front->next;
                    free(temp);
                    continue;
                }
                iden3[1] = 'p';
                for(int i = 1; i < 8; i++){
                    memcpy(string, iden3, pocetP);
                    if(strcmp(mapy[i].nazov, string) == 0){
                        front->mapa->mapa = mapy[i].mapa;
                        front->mapa->nazov = mapy[i].nazov;
                        break;
                    }
                }
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
            }
            else if(front->mrtvyDrak == 1 && front->mapa->mapa[a][b] == '3'){
                front->pocetP--;
                if(front->pocetP == 0){
                    ulozCestu(front);
                    temp = front;
                    front = front->next;
                    free(temp);
                    continue;
                }
                iden3[2] = 'p';
                for(int i = 1; i < 8; i++){
                    memcpy(string, iden3, pocetP);
                    if(strcmp(mapy[i].nazov, string) == 0){
                        front->mapa->mapa = mapy[i].mapa;
                        front->mapa->nazov = mapy[i].nazov;
                        break;
                    }
                }
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
            }
        }
        else if(pocetP == 4){
            if(front->mapa->mapa[a][b] == 'D'){
                front->mapa->nazov = mapy[1].nazov;
                front->mapa->mapa = mapy[1].mapa;
                front->mrtvyDrak = 1;
                front->next = NULL;
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
            }
            else if(front->mrtvyDrak == 1 && front->mapa->mapa[a][b] == '1'){
                front->pocetP--;
                if(front->pocetP == 0){
                    ulozCestu(front);
                    temp = front;
                    front = front->next;
                    free(temp);
                    continue;
                }
                iden4[0] = 'p';
                for(int i = 1; i < 16; i++){
                    memcpy(string, iden4, pocetP);
                    if(strcmp(mapy[i].nazov, string) == 0){
                        front->mapa->mapa = mapy[i].mapa;
                        front->mapa->nazov = mapy[i].nazov;
                        break;
                    }
                }
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
            }
            else if(front->mrtvyDrak == 1 && front->mapa->mapa[a][b] == '2'){
                front->pocetP--;
                if(front->pocetP == 0){
                    ulozCestu(front);
                    temp = front;
                    front = front->next;
                    free(temp);
                    continue;
                }
                iden4[1] = 'p';
                for(int i = 1; i < 16; i++){
                    memcpy(string, iden4, pocetP);
                    if(strcmp(mapy[i].nazov, string) == 0){
                        front->mapa->mapa = mapy[i].mapa;
                        front->mapa->nazov = mapy[i].nazov;
                        break;
                    }
                }
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
            }
            else if(front->mrtvyDrak == 1 && front->mapa->mapa[a][b] == '3'){
                front->pocetP--;
                if(front->pocetP == 0){
                    ulozCestu(front);
                    temp = front;
                    front = front->next;
                    free(temp);
                    continue;
                }
                iden4[2] = 'p';
                for(int i = 1; i < 16; i++){
                    memcpy(string, iden4, pocetP);
                    if(strcmp(mapy[i].nazov, string) == 0){
                        front->mapa->mapa = mapy[i].mapa;
                        front->mapa->nazov = mapy[i].nazov;
                        break;
                    }
                }
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
            }
            else if(front->mrtvyDrak == 1 && front->mapa->mapa[a][b] == '4'){
                front->pocetP--;
                if(front->pocetP == 0){
                    ulozCestu(front);
                    temp = front;
                    front = front->next;
                    free(temp);
                    continue;
                }
                iden4[3] = 'p';
                for(int i = 1; i < 16; i++){
                    memcpy(string, iden4, pocetP);
                    if(strcmp(mapy[i].nazov, string) == 0){
                        front->mapa->mapa = mapy[i].mapa;
                        front->mapa->nazov = mapy[i].nazov;
                        break;
                    }
                }
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
            }
        }
        else if(pocetP == 5){
            if(front->mapa->mapa[a][b] == 'D'){
                front->mapa->nazov = mapy[1].nazov;
                front->mapa->mapa = mapy[1].mapa;
                front->mrtvyDrak = 1;
                front->next = NULL;
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
            }
            else if(front->mrtvyDrak == 1 && front->mapa->mapa[a][b] == '1'){
                front->pocetP--;
                if(front->pocetP == 0){
                    ulozCestu(front);
                    temp = front;
                    front = front->next;
                    free(temp);
                    continue;
                }
                iden5[0] = 'p';
                for(int i = 1; i < 32; i++){
                    memcpy(string, iden5, pocetP);
                    if(strcmp(mapy[i].nazov, string) == 0){
                        front->mapa->mapa = mapy[i].mapa;
                        front->mapa->nazov = mapy[i].nazov;
                        break;
                    }
                }
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
            }
            else if(front->mrtvyDrak == 1 && front->mapa->mapa[a][b] == '2'){
                front->pocetP--;
                if(front->pocetP == 0){
                    ulozCestu(front);
                    temp = front;
                    front = front->next;
                    free(temp);
                    continue;
                }
                iden5[1] = 'p';
                for(int i = 1; i < 32; i++){
                    memcpy(string, iden5, pocetP);
                    if(strcmp(mapy[i].nazov, string) == 0){
                        front->mapa->mapa = mapy[i].mapa;
                        front->mapa->nazov = mapy[i].nazov;
                        break;
                    }
                }
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
            }
            else if(front->mrtvyDrak == 1 && front->mapa->mapa[a][b] == '3'){
                front->pocetP--;
                if(front->pocetP == 0){
                    ulozCestu(front);
                    temp = front;
                    front = front->next;
                    free(temp);
                    continue;
                }
                iden5[2] = 'p';
                for(int i = 1; i < 32; i++){
                    memcpy(string, iden5, pocetP);
                    if(strcmp(mapy[i].nazov, string) == 0){
                        front->mapa->mapa = mapy[i].mapa;
                        front->mapa->nazov = mapy[i].nazov;
                        break;
                    }
                }
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
            }
            else if(front->mrtvyDrak == 1 && front->mapa->mapa[a][b] == '4'){
                front->pocetP--;
                if(front->pocetP == 0){
                    ulozCestu(front);
                    temp = front;
                    front = front->next;
                    free(temp);
                    continue;
                }
                iden5[3] = 'p';
                for(int i = 1; i < 32; i++){
                    memcpy(string, iden5, pocetP);
                    if(strcmp(mapy[i].nazov, string) == 0){
                        front->mapa->mapa = mapy[i].mapa;
                        front->mapa->nazov = mapy[i].nazov;
                        break;
                    }
                }
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
            }
            else if(front->mrtvyDrak == 1 && front->mapa->mapa[a][b] == '5'){
                front->pocetP--;
                if(front->pocetP == 0){
                    ulozCestu(front);
                    temp = front;
                    front = front->next;
                    free(temp);
                    continue;
                }
                iden5[4] = 'p';
                for(int i = 1; i < 32; i++){
                    memcpy(string, iden5, pocetP);
                    if(strcmp(mapy[i].nazov, string) == 0){
                        front->mapa->mapa = mapy[i].mapa;
                        front->mapa->nazov = mapy[i].nazov;
                        break;
                    }
                }
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < m; j++){
                        navstivene[i][j] = front->mapa->mapa[i][j];
                    }
                }
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
            if(navstivene[a+1][b] != '0')
                addToFront(&front, setmapa(navstivene), a+1, b);
            
            if(b == 0){
                // lavy horny roh - mozem ist iba doprava
                if(navstivene[a][b+1] != '0')
                    addToFront(&front, setmapa(navstivene), a, b+1);
            }
            else if(b == (m-1)){
                // pravy horny roh - mozem ist iba dolava
                if(navstivene[a][b-1] != '0')
                    addToFront(&front, setmapa(navstivene), a, b-1);
            }
            else{
                // zvysok hornej hrany - mozem ist doparava aj dolava
                if(navstivene[a][b+1] != '0')
                    addToFront(&front, setmapa(navstivene), a, b+1);
                if(navstivene[a][b-1] != '0')
                    addToFront(&front, setmapa(navstivene), a, b-1);
            }
        }
        else if(a == (n-1)){
            // dolna hrana - vzdy mozem prehladavat vyssi rad
            if(navstivene[a-1][b] != '0')
                addToFront(&front, setmapa(navstivene), a-1, b);
            
            if(b == 0){
                // lavy dolny roh - mozem ist iba doprava
                if(navstivene[a][b+1] != '0')
                    addToFront(&front, setmapa(navstivene), a, b+1);
            }
            else if(b == (m-1)){
                // pravy dolny roh - mozem ist iba dolava
                if(navstivene[a][b-1] != '0')
                    addToFront(&front, setmapa(navstivene), a, b-1);
            }
            else{
                // zvysok dolnej hrany - mozem ist doprava aj dolava
                if(navstivene[a][b+1] != '0')
                    addToFront(&front, setmapa(navstivene), a, b+1);
                if(navstivene[a][b-1] != '0')
                    addToFront(&front, setmapa(navstivene), a, b-1);
            }
        }
        else if(b == 0){
            // lava hrana - mozem ist doprava, hore alebo dole
            if(navstivene[a+1][b] != '0')
                addToFront(&front, setmapa(navstivene), a+1, b);
            if(navstivene[a-1][b] != '0')
                addToFront(&front, setmapa(navstivene), a-1, b);
            if(navstivene[a][b+1] != '0')
                addToFront(&front, setmapa(navstivene), a, b+1);
        }
        else if(b == (m-1)){
            // prava hrana - mozem ist dolava, hore alebo dole
            if(navstivene[a+1][b] != '0')
                addToFront(&front, setmapa(navstivene), a+1, b);
            if(navstivene[a-1][b] != '0')
                addToFront(&front, setmapa(navstivene), a-1, b);
            if(navstivene[a][b-1] != '0')
                addToFront(&front, setmapa(navstivene), a, b-1);
        }
        else{
            // uprostred - mozem ist do vsetkych styroch smerov
            if(navstivene[a+1][b] != '0')
                addToFront(&front, setmapa(navstivene), a+1, b);
            if(navstivene[a-1][b] != '0')
                addToFront(&front, setmapa(navstivene), a-1, b);
            if(navstivene[a][b+1] != '0')
                addToFront(&front, setmapa(navstivene), a, b+1);
            if(navstivene[a][b-1] != '0')
                addToFront(&front, setmapa(navstivene), a, b-1);
        }
        
        // Pouzity prvok vyhodim z frontu a pokracujem dalej
        temp = front;
        if(front != NULL){
            front = front->next;
            free(temp);
        }
    }
}
/*
 * Najprv sa vytvoria mapy podla poctu princezien, a potom sa zavola funkcia dijkstra, ktora najde
 * najkratsiu cestu. Ak sa cesta z nejakeho dovodu nenajde (napr. ked je drak obkoleseny nepriechodnymi
 * prekazkami), tak funkcia vrati NULL. V opacnom pripade sa zisti dlzka cesty a podla nej sa vytvori
 * pole na suradnice cesty. Tie sa do pola postupne zapisu, ale kedze sa cesta prechadza odzadu, je
 * potrebne na konci pole este otocit. Vysledna cesta je vratena do mainu, kde sa vypise.
 */
int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty){
    int pocetP = 0;
    N = n;
    M = m;
    
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
    
    if(cesta == NULL){
        return NULL;
    }
    if(pocetP == 0){
        printf("V mape sa nenachadza ziadna princezna - cesta vedie iba k drakovi:\n");
    }
    
    CESTA *akt = cesta;
    
    while(akt != NULL){
        (*dlzka_cesty)++;
        akt = akt->prev;
    }
    
    int *returnPole = malloc(2 * (*dlzka_cesty) * sizeof(int));
    int j = 0, koniec = 0;
    akt = cesta;
    
    while(akt != NULL){
        returnPole[j*2] = akt->a;
        returnPole[j*2+1] = akt->b;
        j++;
        akt = akt->prev;
    }
    
    int temp, zaciatok = j * 2 - 1;
    while(koniec != zaciatok + 1){
        temp = returnPole[koniec];
        returnPole[koniec] = returnPole[zaciatok];
        returnPole[zaciatok] = temp;
        koniec++;
        zaciatok--;
    }
    
    return returnPole;
}

int main() {
    char **mapa = NULL;
    int i, test, dlzka_cesty, cas, *najdenaCesta = NULL;
    int n = 0, m = 0, t = 0;
    FILE *f;
    
    while(1){
        printf("Zadajte cislo testu:\n0: Ukoncenie programu\n");
        printf("1: Nacitanie testu zo suboru \n2-6: Validne vstupy \n7-12: Chybne vstupy\n");
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
                
                najdenaCesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
                
            case 2:
                n = 10;
                m = n;
                t = 12;
                mapa = (char **) malloc(n * sizeof(char *));
                mapa[0]="CCHCNHCCHN";
                mapa[1]="NNCCCHHCCC";
                mapa[2]="DNCCNNHHHC";
                mapa[3]="CHHHCCCCCC";
                mapa[4]="CCCCCNHHHH";
                mapa[5]="CCHCCCNNNN";
                mapa[6]="NNNNNHCCCC";
                mapa[7]="CCCCCCCCCC";
                mapa[8]="CCCNNHHHHH";
                mapa[9]="HHHPCCCCCC";
                
                printf("Test s jednou princeznou\n");
                najdenaCesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
                
            case 3:
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
                najdenaCesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
                
            case 4:
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
                
                printf("Test s tromi princeznami\n");
                najdenaCesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
                
            case 5:
                n = 6;
                m = 8;
                t = 10;
                mapa = (char**) malloc(n * sizeof(char*));
                mapa[0]="CHCCHCCD";
                mapa[1]="NNNNCNNH";
                mapa[2]="CCPCCNHH";
                mapa[3]="PNNNNNNH";
                mapa[4]="CNNHHPHC";
                mapa[5]="HPCCHHNN";
                
                printf("Test so styrmi princeznami\n");
                najdenaCesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
                
            case 6:
                n = 4;
                m = 8;
                t = 2;
                mapa = (char**) malloc(n * sizeof(char*));
                mapa[0]="CNNNNNNN";
                mapa[1]="DPCPCPHP";
                mapa[2]="CNNNNNNN";
                mapa[3]="PNNNNNNN";
                
                printf("Test s piatimi princeznami\n");
                najdenaCesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
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
                mapa[5]="NNHCCCNNNN";
                mapa[6]="NNNNNHCCCC";
                mapa[7]="CCCCCHCCCC";
                mapa[8]="CCCNNHHHHH";
                mapa[9]="HHHCCCCCCC";
                
                printf("Test bez princezien\n");
                najdenaCesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
                
            case 8:
                n = 6;
                m = 8;
                t = 10;
                mapa = (char**) malloc(n * sizeof(char*));
                mapa[0]="CHCCHCND";
                mapa[1]="NNNNCNNN";
                mapa[2]="CCPCCNHH";
                mapa[3]="PNNNNNNH";
                mapa[4]="CNNHHPHC";
                mapa[5]="HPCCHHNN";
                
                printf("Test s nedostupnym drakom\n");
                najdenaCesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
                
            case 9:
                n = 6;
                m = 8;
                t = 10;
                mapa = (char**) malloc(n * sizeof(char*));
                mapa[0]="CHCCHCCD";
                mapa[1]="NNNNCNNN";
                mapa[2]="CCPCCNHH";
                mapa[3]="PNNNNNNH";
                mapa[4]="CNNHNPNC";
                mapa[5]="HPCCHNNN";
                
                printf("Test s nedostupnou princeznou\n");
                najdenaCesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
            
            case 10:
                n = 6;
                m = 8;
                t = 10;
                mapa = (char**) malloc(n * sizeof(char*));
                mapa[0]="CHCCHCCC";
                mapa[1]="NNNNCNNN";
                mapa[2]="CCPCCNHH";
                mapa[3]="PNNNNNNH";
                mapa[4]="CNNHHPCC";
                mapa[5]="HPCCHNNN";
                
                printf("Test bez draka\n");
                najdenaCesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
                
            case 11:
                n = 6;
                m = 8;
                t = 10;
                mapa = (char**) malloc(n * sizeof(char*));
                mapa[0]="CHCCHCCD";
                mapa[1]="NNNNCNNN";
                mapa[2]="CCPDCNHH";
                mapa[3]="PNNNNNNH";
                mapa[4]="CNNHHPCC";
                mapa[5]="HPCCHNNN";
                
                printf("Test s dvomi drakmi\n");
                najdenaCesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
                
            case 12:
                n = 6;
                m = 8;
                t = 10;
                mapa = (char**) malloc(n * sizeof(char*));
                mapa[0]="CHCCHCCD";
                mapa[1]="NNNNCNNN";
                mapa[2]="CCPPCNHH";
                mapa[3]="PNNNNNNH";
                mapa[4]="CNNHHPCP";
                mapa[5]="HPCCHNNN";
                
                printf("Test so siestimi princeznami\n");
                najdenaCesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
                
            default:
                continue;
        }
        
        if(najdenaCesta != NULL){
            cas = 0;
            for(i = 0; i < dlzka_cesty; i++){
                printf("%d %d\n", najdenaCesta[i*2], najdenaCesta[i*2+1]);
                
                if(mapa[najdenaCesta[i*2+1]][najdenaCesta[i*2]] == 'H')
                    cas += 2;
                else
                    cas += 1;
                
                if(mapa[najdenaCesta[i*2+1]][najdenaCesta[i*2]] == 'D' && cas > t)
                    printf("Nestihol si zabit draka!\n");
                
                if(mapa[najdenaCesta[i*2+1]][najdenaCesta[i*2]] == 'N')
                    printf("Prechod cez nepriechodnu prekazku!\n");
                
                if(i > 0 && abs(najdenaCesta[i*2+1] - najdenaCesta[(i-1)*2+1]) + abs(najdenaCesta[i*2] - najdenaCesta[(i-1)*2]) > 1)
                    printf("Neplatny posun Popolvara!\n");
            }
            
            printf("Dlzka cesty: %d\n\n", cas);
        }
        else{
            printf("Chybna mapa!\n\n");
        }
        free(najdenaCesta);
    }
    return 0;
}
