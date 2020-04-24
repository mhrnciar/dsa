//
//  main.c
//  Projekt 3
//  Matej Hrnčiar
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

/*
 * Funkcia clearAll nastavi vsetky cesty na NULL. Je to pre to, aby sa po scenari s uspesnym najdenim cesty
 * nemohol spustit scenar s chybnou mapou, ktora vrati zdanlivo uspesnu cestu.
 */
void clearAll(){
    drak = NULL;
    dp1 = NULL;
    dp2 = NULL;
    dp3 = NULL;
    p12 = NULL;
    p13 = NULL;
    p21 = NULL;
    p23 = NULL;
    p31 = NULL;
    p32 = NULL;
}

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

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty){
    int a = 0, b = 0, pocetP = 0, pocetD = 0;;
    int *returnPole = NULL;
    char navstivene[n][m];
    FRONT *front = NULL, *temp = NULL;
    
    clearAll();
    
    /*
     * Najprv sa zisti ci je mapa v poriadku, teda ci je v nej prave jeden drak a prave tri princezne.
     * Ak ano, tak sa pokracuje dalej, ak nie vypise sa chyba a vrati sa NULL.
     */
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
    
    if(pocetD == 0){
        printf("V mape sa nenachadza drak!\n\n");
        return NULL;
    }
    else if(pocetD > 1){
        printf("V mape je prilis vela drakov!\n\n");
        return NULL;
    }
    if(pocetP > 3){
        printf("V mape je prilis vela princezien!\n\n");
        return NULL;
    }
    else if(pocetP < 3){
        printf("V mape sa nenachadzaju tri princezne!\n\n");
        return NULL;
    }
    
    /*
     * Dijkstra zbehne dokopy 5-krat: najprv hlada cestu k drakovi od [0,0], potom hlada princezne od draka,
     * od prvej princeznej cestu k druhej a tretej, od druhej k prvej a tretej a od tretej k prvej a druhej.
     * 'navstivene' uchovava navstivene policka ale kedze pri hladani niecoho ineho uz mozeme ist spat, pole
     * sa vzdy vynuluje a nastavia sa suradnice zaciatku hladania od posledneho najdeneho objektu. Ak predosla
     * cesta neexistuje, napr. ked je drak obkoleseny nepriechodnymi polickami, vypise sa chyba a vrati sa NULL.
     */
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < m; k++){
                navstivene[j][k] = '0';
            }
        }
        
        switch(i){
            case 0:
                a = 0;
                b = 0;
                break;
                
            case 1:
                if(drak == NULL){
                    printf("Nie je mozne dostat sa k drakovi!\n\n");
                    return NULL;
                }
                a = drak->a;
                b = drak->b;
                break;
                
            case 2:
                if(dp1 == NULL){
                    printf("Nie je mozne dostat sa k princeznej!\n\n");
                    return NULL;
                }
                a = dp1->a;
                b = dp1->b;
                break;
                
            case 3:
                if(dp2 == NULL){
                    printf("Nie je mozne dostat sa k princeznej!\n\n");
                    return NULL;
                }
                a = dp2->a;
                b = dp2->b;
                break;
                
            case 4:
                if(dp3 == NULL){
                    printf("Nie je mozne dostat sa k princeznej!\n\n");
                    return NULL;
                }
                a = dp3->a;
                b = dp3->b;
                break;
        }
        
        front = NULL;
        front = malloc(sizeof(FRONT*));
        front->cesta = (CESTA *) malloc(sizeof(CESTA));
        front->cesta->a = a;
        front->cesta->b = b;
        front->cesta->prev = NULL;
        front->len = 0;
        front->next = NULL;
        
        /*
         * Prechadza sa mapa az kym sa nenajde hladany objekt, najprv sa hlada drak, a potom princezne
         * od draka sa najdu postupne vsetky tri a od nich sa hladaju zvysne dve.
         */
        while(front != NULL){
            a = front->cesta->a;
            b = front->cesta->b;
            navstivene[a][b] = '1';
            
            if((i == 0) && (mapa[a][b] == 'D')){
                drak = front->cesta;
                drak->len = front->len;
                break;
            }
            
            else if((i == 1) && (mapa[a][b] == 'P')){
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
            
            else if((i == 2) && (mapa[a][b] == 'P') && (a != dp1->a) && (b != dp1->b)){
                if(p12 == NULL){
                    p12 = front->cesta;
                    p12->len = front->len;
                }
                else if(p13 == NULL){
                    p13 = front->cesta;
                    p13->len = front->len;
                    free(front);
                    front = NULL;
                    break;
                }
            }
            
            else if((i == 3) && (mapa[a][b] == 'P') && (a != dp2->a) && (b != dp2->b)){
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
            
            else if((i == 4) && (mapa[a][b] == 'P') && (a != dp3->a) && (b != dp3->b)){
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
            
            /*
             * Prechadza sa mapa podla toho, ci uz dane policko bolo navstivene alebo nie. Treba pokryt velke
             * mnozstvo scenarov, napr. ked sa nachadzam na hornej hrane, nemozem prehladavat policka nad lebo
             * neexistuju, z toho isteho dovodu ak sa nachadzam na lavej hrane, nemozem prehladavat policka
             * vlavo a pod.
             */
            if(a == 0){
                // horna hrana - vzdy mozem prehladavat nizsi rad
                if(navstivene[a+1][b] == '0')
                    addToFront(&front, mapa, a+1, b);
                
                if(b == 0){
                    // lavy horny roh - mozem ist iba doprava
                    if(navstivene[a][b+1] == '0')
                        addToFront(&front, mapa, a, b+1);
                }
                else if(b == (m-1)){
                    // pravy horny roh - mozem ist iba dolava
                    if(navstivene[a][b-1] == '0')
                        addToFront(&front, mapa, a, b-1);
                }
                else{
                    // zvysok hornej hrany - mozem ist doparava aj dolava
                    if(navstivene[a][b+1] == '0')
                        addToFront(&front, mapa, a, b+1);
                    if(navstivene[a][b-1] == '0')
                        addToFront(&front, mapa, a, b-1);
                }
            }
            else if(a == (n-1)){
                // dolna hrana - vzdy mozem prehladavat vyssi rad
                if(navstivene[a-1][b] == '0')
                    addToFront(&front, mapa, a-1, b);
                
                if(b == 0){
                    // lavy dolny roh - mozem ist iba doprava
                    if(navstivene[a][b+1] == '0')
                        addToFront(&front, mapa, a, b+1);
                }
                else if(b == (m-1)){
                    // pravy dolny roh - mozem ist iba dolava
                    if(navstivene[a][b-1] == '0')
                        addToFront(&front, mapa, a, b-1);
                }
                else{
                    // zvysok dolnej hrany - mozem ist doprava aj dolava
                    if(navstivene[a][b+1] == '0')
                        addToFront(&front, mapa, a, b+1);
                    if(navstivene[a][b-1] == '0')
                        addToFront(&front, mapa, a, b-1);
                }
            }
            else if(b == 0){
                // lava hrana - mozem ist doprava, hore alebo dole
                if(navstivene[a][b+1] == '0')
                    addToFront(&front, mapa, a, b+1);
                if(navstivene[a+1][b] == '0')
                    addToFront(&front, mapa, a+1, b);
                if(navstivene[a-1][b] == '0')
                    addToFront(&front, mapa, a-1, b);
            }
            else if(b == (m-1)){
                // prava hrana - mozem ist dolava, hore alebo dole
                if(navstivene[a][b-1] == '0')
                    addToFront(&front, mapa, a, b-1);
                if(navstivene[a+1][b] == '0')
                    addToFront(&front, mapa, a+1, b);
                if(navstivene[a-1][b] == '0')
                    addToFront(&front, mapa, a-1, b);
            }
            else{
                // uprostred - mozem ist do vsetkych styroch smerov
                if(navstivene[a][b+1] == '0')
                    addToFront(&front, mapa, a, b+1);
                if(navstivene[a][b-1] == '0')
                    addToFront(&front, mapa, a, b-1);
                if(navstivene[a+1][b] == '0')
                    addToFront(&front, mapa, a+1, b);
                if(navstivene[a-1][b] == '0')
                    addToFront(&front, mapa, a-1, b);
            }
            
            temp = front;
            if(front != NULL){
                front = front->next;
                free(temp);
            }
        }
    }
    
    int kombinacie[6], ckombinacie = 0, min = 1000000;
    
    // Po najdeni draka aj princezien sa urobia kombinacie dlzok a najde sa najkratsia cesta
    kombinacie[0] = (drak->len + dp1->len + p12->len + p23->len);
    kombinacie[1] = (drak->len + dp1->len + p13->len + p32->len);
    kombinacie[2] = (drak->len + dp2->len + p21->len + p13->len);
    kombinacie[3] = (drak->len + dp2->len + p23->len + p31->len);
    kombinacie[4] = (drak->len + dp3->len + p31->len + p12->len);
    kombinacie[5] = (drak->len + dp3->len + p32->len + p21->len);
    
    for (int i = 0; i < 6; i++) {
        if (kombinacie[i] < min) {
            min = kombinacie[i];
            ckombinacie = i;
        }
    }
    
    CESTA *pomCesta = NULL, *cesta1 = NULL, *cesta2 = NULL, *cesta3 = NULL;
    
    /*
     * Zapise sa najkratsia cesta do pomocnych pointerov a vypocita sa jej dlzka. Pri kazdej ceste treba
     * odpocitat 1, lebo 2-krat pocitam to iste policko, raz ako konecne, a potom ako zaciatocne v dalsej
     * ceste. Po zisteni velkosti cesty sa vytvori pole, kam sa zapisu vsetky suradnice, no kedze idem od
     * konca cesty, musia sa vzdy po blokoch otocit, teda najprv sa otoci cesta od [0,0] po draka, potom
     * od draka k princeznej a pod.
     */
    switch(ckombinacie){
        case 0:
            cesta1 = dp1;
            cesta2 = p12;
            cesta3 = p23;
            break;
        case 1:
            cesta1 = dp1;
            cesta2 = p13;
            cesta3 = p32;
            break;
        case 2:
            cesta1 = dp2;
            cesta2 = p21;
            cesta3 = p13;
            break;
        case 3:
            cesta1 = dp2;
            cesta2 = p23;
            cesta3 = p31;
            break;
        case 4:
            cesta1 = dp3;
            cesta2 = p31;
            cesta3 = p12;
            break;
        case 5:
            cesta1 = dp3;
            cesta2 = p32;
            cesta3 = p21;
            break;
    }
    
    for(int i = 0; i < 4; i++){
        switch(i){
            case 0:
                pomCesta = drak;
                break;
            case 1:
                pomCesta = cesta1;
                (*dlzka_cesty)--;
                break;
            case 2:
                pomCesta = cesta2;
                (*dlzka_cesty)--;
                break;
            case 3:
                pomCesta = cesta3;
                (*dlzka_cesty)--;
                break;
        }
        
        while(pomCesta != NULL){
            (*dlzka_cesty)++;
            pomCesta = pomCesta->prev;
        }
    }
    
    returnPole = malloc(2 * (*dlzka_cesty) * sizeof(int));
    
    int j = 0;
    for(int i = 0; i < 4; i++){
        int koniec = j * 2;
        switch(i){
            case 0:
                while(drak != NULL){
                  returnPole[j*2] = drak->a;
                  returnPole[j*2+1] = drak->b;
                  j++;
                  drak = drak->prev;
                }
                break;
                
            case 1 :
                while(cesta1->prev != NULL){
                  returnPole[j*2] = cesta1->a;
                  returnPole[j*2+1] = cesta1->b;
                  j++;
                  cesta1 = cesta1->prev;
                }
                break;
                
            case 2 :
                while(cesta2->prev != NULL){
                  returnPole[j*2] = cesta2->a;
                  returnPole[j*2+1] = cesta2->b;
                  j++;
                  cesta2 = cesta2->prev;
                }
                break;
                
            case 3 :
                while(cesta3->prev != NULL){
                  returnPole[j*2] = cesta3->a;
                  returnPole[j*2+1] = cesta3->b;
                  j++;
                  cesta3 = cesta3->prev;
                }
                break;
        }
        
        int temp, zaciatok = j * 2 - 1;
        
        while(koniec != zaciatok + 1){
          temp = returnPole[koniec];
          returnPole[koniec] = returnPole[zaciatok];
          returnPole[zaciatok] = temp;
          koniec++;
          zaciatok--;
        }
    }
    return returnPole;
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
                n = 15;
                m = 20;
                t = 12;
                mapa = (char**) malloc(n * sizeof(char*));
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
