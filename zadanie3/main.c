//
//  main.c
//  Projekt 3
//
//  Created by Matej Hrnciar on 20/04/2020.
//  Copyright © 2020 Matej Hrnciar. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct cesta{
    int a;
    int b;
    int len;
    struct cesta *prev;
} CESTA;

typedef struct front{
    int len;
    struct front *next;
    CESTA *cesta;
} FRONT;

CESTA *drak, *dp1, *dp2, *dp3, *p12, *p13, *p21, *p23, *p31, *p32;

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

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty){
    int a, b, pomPole[6], minValue = 650000, minPos = 0, pocetP = 0, pocetD = 0;;
    int *returnPole = NULL;
    char pomMapa[n][m];
    FRONT *front = NULL, *temp = NULL;
    CESTA *pom = NULL, *pom1 = NULL, *pom2 = NULL, *pom3 = NULL;
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(mapa[i][j] == 'P'){
                pocetP++;
            }
            if(mapa[i][j] == 'D'){
                pocetD++;
            }
        }
    }
    if(pocetD != 1){
        printf("V mape je prilis vela drakov!\n");
        return NULL;
    }
    if(pocetP > 3){
        printf("V mape je prilis vela princezien!\n");
        return NULL;
    }
    else if (pocetP < 3){
        printf("V mape sa nenachadzaju tri princezne!\n");
        return NULL;
    }
    
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < m; k++){
                pomMapa[j][k] = '0';
            }
        }
        
        switch(i){
            case 0:
                a = 0;
                b = 0;
                break;
                
            case 1:
                if(drak == NULL){
                    printf("Nie je mozne dostat sa k drakovi!\n");
                    return NULL;
                }
                a = drak->a;
                b = drak->b;
                break;
                
            case 2:
                a = dp1->a;
                b = dp1->b;
                break;
                
            case 3:
                a = dp2->a;
                b = dp2->b;
                break;
                
            case 4:
                a = dp3->a;
                b = dp3->b;
                break;
        }
        
        front = NULL;
        front = malloc(sizeof(FRONT*));
        front->cesta = (CESTA *) malloc (sizeof(CESTA));
        front->cesta->a = a;
        front->cesta->b = b;
        front->cesta->prev = NULL;
        front->len = 0;
        front->next = NULL;
        
        while(front != NULL) {
            a = front->cesta->a;
            b = front->cesta->b;
            pomMapa[a][b] = '1';
            
            if((i == 0) && (mapa[a][b] == 'D')) {
                //printf("TESTDRAK\n");
                drak = front->cesta;
                drak->len = front->len;
                break;
            }
            
            else if((i == 1) && (mapa[a][b] == 'P')) {
                //printf("TESTDDPP\n");
                if(dp1 == NULL){
                    dp1 = front->cesta;
                    dp1->len = front->len;
                    
                }
                else if(dp2 == NULL){
                    dp2 = front->cesta;
                    dp2->len = front->len;
                    
                }
                else if(dp3 == NULL){
                    dp3 = front->cesta;
                    dp3->len = front->len;
                    free(front);
                    break;
                }
            }
            
            else if((i == 2) && (mapa[a][b] == 'P') && !((a == dp1->a) && (b == dp1->b))) {
                if(p12 == NULL){
                    p12 = front->cesta;
                    p12->len = front->len;
                }
                else if(p13 == NULL){
                    p13 = front->cesta;
                    p13->len = front->len;
                    free(front);
                    front = NULL;
                    break;}
            }
            else if((i == 3) && (mapa[a][b] == 'P') && !((a == dp2->a) && (b == dp2->b))) {
                if((p23 == NULL) && (a == dp1->a) && (b == dp1->b)){
                    p21 = front->cesta;
                    p21->len = front->len;
                }
                else if((p23 != NULL) && (a == dp1->a) && (b == dp1->b)){
                    p21 = front->cesta;
                    p21->len = front->len;
                    free(front);
                    break;
                }
                else if((p21 == NULL) && (a == dp3->a) && (b == dp3->b)){
                    p23 = front->cesta;
                    p23->len = front->len;
                }
                else if((p21 != NULL) && (a == dp3->a) && (b == dp3->b)){
                    p23 = front->cesta;
                    p23->len = front->len;
                    free(front);
                    break;
                }
            }
            else if((i == 4) && (mapa[a][b] == 'P') && !((a == dp3->a) && (b == dp3->b))){
                if((p32 == NULL) && (a == dp1->a) && (b == dp1->b)){
                    p31 = front->cesta;
                    p31->len = front->len;
                }
                else if((p32 != NULL) && (a == dp1->a) && (b == dp1->b)){
                    p31 = front->cesta;
                    p31->len = front->len;
                    free(front);
                    break;
                }
                else if((p31 == NULL) && (a == dp2->a) && (b == dp2->b)){
                    p32 = front->cesta;
                    p32->len = front->len;
                }
                else if((p31 != NULL) && (a == dp2->a) && (b == dp2->b)){
                    p32 = front->cesta;
                    p32->len = front->len;
                    free(front);
                    break;
                }
            }
            
            if(a == 0){
                if(pomMapa[a+1][b] == '0')
                    addToFront(&front, mapa, a+1, b);
                
                if(b == 0){
                    // lavy horny roh
                    if(pomMapa[a][b+1] == '0')
                        addToFront(&front, mapa, a, b+1);
                }
                else if(b == (m-1)){
                    // pravy horny roh
                    if(pomMapa[a][b-1] == '0')
                        addToFront(&front, mapa, a, b-1);
                }
                else{
                    // horna hrana
                    if(pomMapa[a][b+1] == '0')
                        addToFront(&front, mapa, a, b+1);
                    if(pomMapa[a][b-1] == '0')
                        addToFront(&front, mapa, a, b-1);
                }
            }
            else if(a == (n-1)){
                if(pomMapa[a-1][b] == '0')
                    addToFront(&front, mapa, a-1, b);
                
                if(b == 0) {
                    // lavy dolny roh
                    if (pomMapa[a][b+1] == '0')
                        addToFront(&front, mapa, a, b+1);
                }
                else if(b == (m-1)) {
                    // pravy dolny roh
                    if(pomMapa[a][b-1] == '0')
                        addToFront(&front, mapa, a, b-1);
                }
                else{
                    // dolna hrana
                    if(pomMapa[a][b+1] == '0')
                        addToFront(&front, mapa, a, b+1);
                    if(pomMapa[a][b-1] == '0')
                        addToFront(&front, mapa, a, b-1);
                }
            }
            else if(b == 0){
                // lava hrana
                if (pomMapa[a][b+1] == '0')
                    addToFront(&front, mapa, a, b+1);
                if (pomMapa[a+1][b] == '0')
                    addToFront(&front, mapa, a+1, b);
                if (pomMapa[a-1][b] == '0')
                    addToFront(&front, mapa, a-1, b);
            }
            else if(b == (m-1)){
                // prava hrana
                if (pomMapa[a][b-1] == '0')
                    addToFront(&front, mapa, a, b-1);
                if (pomMapa[a+1][b] == '0')
                    addToFront(&front, mapa, a+1, b);
                if (pomMapa[a-1][b] == '0')
                    addToFront(&front, mapa, a-1, b);
            }
            else{
                if(pomMapa[a][b+1] == '0')
                    addToFront(&front, mapa, a, b+1);
                if(pomMapa[a][b-1] == '0')
                    addToFront(&front, mapa, a, b-1);
                if(pomMapa[a+1][b] == '0')
                    addToFront(&front, mapa, a+1, b);
                if(pomMapa[a-1][b] == '0')
                    addToFront(&front, mapa, a-1, b);
            }
            
            // Odstranenie prveho prvku fronty a posun na dalsi
            temp = front;
            if (front != NULL) {
                front = front->next;
                free(temp);
            }
        }
    }
    
    pomPole[0] = (drak->len + dp1->len + p12->len + p23->len);
    pomPole[1] = (drak->len + dp1->len + p13->len + p32->len);
    pomPole[2] = (drak->len + dp2->len + p21->len + p13->len);
    pomPole[3] = (drak->len + dp2->len + p23->len + p31->len);
    pomPole[4] = (drak->len + dp3->len + p31->len + p12->len);
    pomPole[5] = (drak->len + dp3->len + p32->len + p21->len);
    
    // Najdenie najkratsej cesty
    for (int i = 0; i < 6; i++) {
        if (pomPole[i] < minValue) {
            minValue = pomPole[i];
            minPos = i;
        }
    }
    
    switch (minPos) {
        case 0 :
            //printf("TEST0 %d\n", minValue);
            pom1 = dp1;
            pom2 = p12;
            pom3 = p23;
            break;
        case 1 :
            //printf("TEST1 %d\n", minValue);
            pom1 = dp1;
            pom2 = p13;
            pom3 = p32;
            break;
        case 2 :
            //printf("TEST2 %d\n", minValue);
            pom1 = dp2;
            pom2 = p21;
            pom3 = p13;
            break;
        case 3 :
            //printf("TEST3 %d\n", minValue);
            pom1 = dp2;
            pom2 = p23;
            pom3 = p31;
            break;
        case 4 :
            //printf("TEST4 %d\n", minValue);
            pom1 = dp3;
            pom2 = p31;
            pom3 = p12;
            break;
        case 5 :
            //printf("TEST5 %d\n", minValue);
            pom1 = dp3;
            pom2 = p32;
            pom3 = p21;
            break;
    }
    
    // vypocitanie dlzky cesty
    for (int i = 0; i < 4; i++) {
        switch (i) {
            case 0 :
                pom = drak;
                break;
            case 1 :
                pom = pom1;
                (*dlzka_cesty)--;
                break;
            case 2 :
                pom = pom2;
                (*dlzka_cesty)--;
                break;
            case 3 :
                pom = pom3;
                (*dlzka_cesty)--;
                break;
        }
        
        while (pom != NULL) {
            (*dlzka_cesty)++;
            pom = pom->prev;
        }
    }
    
    returnPole = malloc(2 * (*dlzka_cesty) * sizeof(int));
    
    int j = 0;
    for (int i = 0; i < 4; i++) {
        switch (i) {
            case 0 :
                pom = drak;
                break;
            case 1 :
                pom = pom1;
                break;
            case 2 :
                pom = pom2;
                break;
            case 3 :
                pom = pom3;
                break;
        }
        int g = j*2;
        // Pridavanie prvkov do returnPole
        switch (i) {
          case 0 :
              while (pom != NULL) {
              returnPole[j*2] = pom->a;
              returnPole[j*2+1] = pom->b;
              j++;
              pom = pom->prev;
            }
              break;
          case 1 :
          case 2 :
          case 3 :
              while (pom->prev != NULL) {
              returnPole[j*2] = pom->a;
              returnPole[j*2+1] = pom->b;
              j++;
              pom = pom->prev;
            }
              break;
        }
        
        // "Obratenie" pridanych prvkov
        int h = j*2-1;
        int l;
        while (g != h+1) {
          l = returnPole[g];
          returnPole[g] = returnPole[h];
          returnPole[h] = l;
          g++;
          h--;
        }
    }
    return returnPole;
}

int main() {
    char **mapa;
    int i, test, dlzka_cesty, cas, *cesta;
    int n = 0, m = 0, t = 0;
    FILE *f;
    
    while(1){
        printf("Zadajte cislo testu (0 ukonci program):\n");
        scanf("%d", &test);
        dlzka_cesty = 0;
        n = m = t = 0;
        
        switch(test){
            case 0://ukonci program
                return 0;
                
            case 1://nacitanie mapy zo suboru
                f = fopen("test.txt","r");
                if(f)
                    fscanf(f, "%d %d %d", &n, &m, &t);
                else
                    continue;
                
                mapa = (char**) malloc(n * sizeof(char*));
                for(i = 0; i < n; i++){
                    mapa[i] = (char*) malloc(m * sizeof(char));
                    for (int j = 0; j < m; j++){
                        char policko = fgetc(f);
                        if(policko == '\n') policko = fgetc(f);
                        mapa[i][j] = policko;
                    }
                }
                
                fclose(f);
                cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
                
            case 2://nacitanie preddefinovanej mapy n = 10;
                n = 10;
                m = 10;
                t = 12;
                mapa = (char**) malloc(n * sizeof(char*));
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
                
            case 3: //pridajte vlastne testovacie vzorky
                
            default:
                continue;
        }
         printf("DC: %d\n", dlzka_cesty);
         for (i = 0; i < dlzka_cesty; i++) {
           printf("%d %d\n", cesta[i*2], cesta[i*2+1]);
         }
    }
    return 0;
}
