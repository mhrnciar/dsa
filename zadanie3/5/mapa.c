//
//  mapa.c
//  Popolvar
//  Matej Hrnciar
//

#include "mapa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int N;
static int M;
static int size;

char **set(char mapa[N][M]){
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

MAPY *createMaps(char **mapa, int n, int m){
    N = n;
    M = m;
    int pocetP = 0, pocetD = 0, polohy[6][2];
    MAPY *mapy = NULL;
    char povodna[n][m];
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            povodna[i][j] = mapa[i][j];
            if(mapa[i][j] == 'D'){
                pocetD++;
                polohy[0][0] = i;
                polohy[0][1] = j;
               }
            if(mapa[i][j] == 'P'){
                pocetP++;
                switch(pocetP){
                    case 1:
                        povodna[i][j] = '1';
                        break;
                        
                    case 2:
                        povodna[i][j] = '2';
                        break;
                        
                    case 3:
                        povodna[i][j] = '3';
                        break;
                        
                    case 4:
                        povodna[i][j] = '4';
                        break;
                        
                    case 5:
                        povodna[i][j] = '5';
                        break;
                }
                polohy[pocetP][0] = i;
                polohy[pocetP][1] = j;
            }
        }
    }
    
    if(pocetD == 0){
        printf("V mape sa nenachadza ziadny drak!\n");
        return NULL;
    } else if(pocetD > 1){
        printf("V mape sa nachadza viac ako jeden drak!\n");
        return NULL;
    }
    if(pocetP > 5){
        printf("V mape sa nachadza privela princezien!\n");
        return NULL;
    }
    
    if(pocetP == 0){
        size = 1;
        mapy = (MAPY *) malloc(sizeof(MAPY));
        mapy[0].nazov = "drak";
        mapy[0].mapa = set(povodna);
    }
    else if(pocetP == 1){
        size = 2;
        mapy = (MAPY *) malloc(size * sizeof(MAPY));
        mapy[0].nazov = "drak";
        mapy[0].mapa = set(povodna);
        povodna[polohy[0][0]][polohy[0][1]] = 'C';
        mapy[1].nazov = "n";
        mapy[1].mapa = set(povodna);
    }
    else if(pocetP == 2){
        size = 4;
        mapy = (MAPY *) malloc(size * sizeof(MAPY));
        mapy[0].nazov = "drak";
        mapy[0].mapa = set(povodna);
        povodna[polohy[0][0]][polohy[0][1]] = 'C';
        mapy[1].nazov = "nn";
        mapy[1].mapa = set(povodna);
        povodna[polohy[1][0]][polohy[1][1]] = 'C';
        mapy[2].nazov = "pn";
        mapy[2].mapa = set(povodna);
        povodna[polohy[1][0]][polohy[1][1]] = '1';
        povodna[polohy[2][0]][polohy[2][1]] = 'C';
        mapy[3].nazov = "np";
        mapy[3].mapa = set(povodna);
    }
    else if(pocetP == 3){
        size = 8;
        mapy = (MAPY *) malloc(size * sizeof(MAPY));
        mapy[0].nazov = "drak";
        mapy[0].mapa = set(povodna);
        povodna[polohy[0][0]][polohy[0][1]] = 'C';
        mapy[1].nazov = "nnn";
        mapy[1].mapa = set(povodna);
        povodna[polohy[1][0]][polohy[1][1]] = 'C';
        mapy[2].nazov = "pnn";
        mapy[2].mapa = set(povodna);
        povodna[polohy[2][0]][polohy[2][1]] = 'C';
        mapy[3].nazov = "ppn";
        mapy[3].mapa = set(povodna);
        povodna[polohy[1][0]][polohy[1][1]] = '1';
        mapy[4].nazov = "npn";
        mapy[4].mapa = set(povodna);
        povodna[polohy[3][0]][polohy[3][1]] = 'C';
        mapy[5].nazov = "npp";
        mapy[5].mapa = set(povodna);
        povodna[polohy[2][0]][polohy[2][1]] = '2';
        mapy[6].nazov = "nnp";
        mapy[6].mapa = set(povodna);
        povodna[polohy[1][0]][polohy[1][1]] = 'C';
        mapy[7].nazov = "pnp";
        mapy[7].mapa = set(povodna);
    }
    else if(pocetP == 4){
        size = 16;
        mapy = (MAPY *) malloc(size * sizeof(MAPY));
        mapy[0].nazov = "drak";
        mapy[0].mapa = set(povodna);
        povodna[polohy[0][0]][polohy[0][1]] = 'C';
        mapy[1].nazov = "nnnn";
        mapy[1].mapa = set(povodna);
        povodna[polohy[1][0]][polohy[1][1]] = 'C';
        mapy[2].nazov = "pnnn";
        mapy[2].mapa = set(povodna);
        povodna[polohy[2][0]][polohy[2][1]] = 'C';
        mapy[3].nazov = "ppnn";
        mapy[3].mapa = set(povodna);
        povodna[polohy[3][0]][polohy[3][1]] = 'C';
        mapy[4].nazov = "pppn";
        mapy[4].mapa = set(povodna);
        povodna[polohy[1][0]][polohy[1][1]] = '1';
        mapy[5].nazov = "nppn";
        mapy[5].mapa = set(povodna);
        povodna[polohy[2][0]][polohy[2][1]] = '2';
        mapy[6].nazov = "nnpn";
        mapy[6].mapa = set(povodna);
        povodna[polohy[4][0]][polohy[4][1]] = 'C';
        mapy[7].nazov = "nnpp";
        mapy[7].mapa = set(povodna);
        povodna[polohy[2][0]][polohy[2][1]] = 'C';
        mapy[8].nazov = "nppp";
        mapy[8].mapa = set(povodna);
        povodna[polohy[3][0]][polohy[3][1]] = '3';
        mapy[9].nazov = "npnp";
        mapy[9].mapa = set(povodna);
        povodna[polohy[1][0]][polohy[1][1]] = 'C';
        mapy[10].nazov = "ppnp";
        mapy[10].mapa = set(povodna);
        povodna[polohy[2][0]][polohy[2][1]] = '2';
        mapy[11].nazov = "pnnp";
        mapy[11].mapa = set(povodna);
        povodna[polohy[3][0]][polohy[3][1]] = 'C';
        mapy[12].nazov = "pnpp";
        mapy[12].mapa = set(povodna);
        povodna[polohy[4][0]][polohy[4][1]] = '4';
        mapy[13].nazov = "pnpn";
        mapy[13].mapa = set(povodna);
        povodna[polohy[1][0]][polohy[1][1]] = '1';
        povodna[polohy[3][0]][polohy[3][1]] = '3';
        povodna[polohy[4][0]][polohy[4][1]] = 'C';
        mapy[14].nazov = "nnnp";
        mapy[14].mapa = set(povodna);
        povodna[polohy[4][0]][polohy[4][1]] = '4';
        povodna[polohy[2][0]][polohy[2][1]] = 'C';
        mapy[15].nazov = "npnn";
        mapy[15].mapa = set(povodna);
    }
    else if(pocetP == 5){
        size = 32;
        mapy = (MAPY *) malloc(size * sizeof(MAPY));
        mapy[0].nazov = "drak";
        mapy[0].mapa = set(povodna);
        povodna[polohy[0][0]][polohy[0][1]] = 'C';
        mapy[1].nazov = "nnnnn";
        mapy[1].mapa = set(povodna);
        povodna[polohy[1][0]][polohy[1][1]] = 'C';
        mapy[2].nazov = "pnnnn";
        mapy[2].mapa = set(povodna);
        povodna[polohy[2][0]][polohy[2][1]] = 'C';
        mapy[3].nazov = "ppnnn";
        mapy[3].mapa = set(povodna);
        povodna[polohy[2][0]][polohy[2][1]] = '2';
        povodna[polohy[3][0]][polohy[3][1]] = 'C';
        mapy[4].nazov = "pnpnn";
        mapy[4].mapa = set(povodna);
        povodna[polohy[3][0]][polohy[3][1]] = '3';
        povodna[polohy[4][0]][polohy[4][1]] = 'C';
        mapy[5].nazov = "pnnpn";
        mapy[5].mapa = set(povodna);
        povodna[polohy[4][0]][polohy[4][1]] = '4';
        povodna[polohy[5][0]][polohy[5][1]] = 'C';
        mapy[6].nazov = "pnnnp";
        mapy[6].mapa = set(povodna);
        povodna[polohy[5][0]][polohy[5][1]] = '5';
        povodna[polohy[2][0]][polohy[2][1]] = 'C';
        povodna[polohy[3][0]][polohy[3][1]] = 'C';
        mapy[7].nazov = "pppnn";
        mapy[7].mapa = set(povodna);
        povodna[polohy[3][0]][polohy[3][1]] = '3';
        povodna[polohy[4][0]][polohy[4][1]] = 'C';
        mapy[8].nazov = "ppnpn";
        mapy[8].mapa = set(povodna);
        povodna[polohy[4][0]][polohy[4][1]] = '4';
        povodna[polohy[5][0]][polohy[5][1]] = 'C';
        mapy[9].nazov = "ppnnp";
        mapy[9].mapa = set(povodna);
        povodna[polohy[2][0]][polohy[2][1]] = '2';
        povodna[polohy[3][0]][polohy[3][1]] = 'C';
        povodna[polohy[4][0]][polohy[4][1]] = 'C';
        povodna[polohy[5][0]][polohy[5][1]] = '5';
        mapy[10].nazov = "pnppn";
        mapy[10].mapa = set(povodna);
        povodna[polohy[4][0]][polohy[4][1]] = '4';
        povodna[polohy[5][0]][polohy[5][1]] = 'C';
        mapy[11].nazov = "pnpnp";
        mapy[11].mapa = set(povodna);
        povodna[polohy[3][0]][polohy[3][1]] = '3';
        povodna[polohy[4][0]][polohy[4][1]] = 'C';
        mapy[12].nazov = "pnnpp";
        mapy[12].mapa = set(povodna);
        povodna[polohy[5][0]][polohy[5][1]] = '5';
        povodna[polohy[2][0]][polohy[2][1]] = 'C';
        povodna[polohy[3][0]][polohy[3][1]] = 'C';
        mapy[13].nazov = "ppppn";
        mapy[13].mapa = set(povodna);
        povodna[polohy[4][0]][polohy[4][1]] = '4';
        povodna[polohy[5][0]][polohy[5][1]] = 'C';
        mapy[14].nazov = "pppnp";
        mapy[14].mapa = set(povodna);
        povodna[polohy[3][0]][polohy[3][1]] = '3';
        povodna[polohy[4][0]][polohy[4][1]] = 'C';
        mapy[15].nazov = "ppnpp";
        mapy[15].mapa = set(povodna);
        povodna[polohy[2][0]][polohy[2][1]] = '2';
        povodna[polohy[3][0]][polohy[3][1]] = 'C';
        mapy[16].nazov = "pnppp";
        mapy[16].mapa = set(povodna);
        povodna[polohy[1][0]][polohy[1][1]] = '1';
        povodna[polohy[2][0]][polohy[2][1]] = 'C';
        povodna[polohy[3][0]][polohy[3][1]] = '3';
        povodna[polohy[4][0]][polohy[4][1]] = '4';
        povodna[polohy[5][0]][polohy[5][1]] = '5';
        mapy[17].nazov = "npnnn";
        mapy[17].mapa = set(povodna);
        povodna[polohy[3][0]][polohy[3][1]] = 'C';
        mapy[18].nazov = "nppnn";
        mapy[18].mapa = set(povodna);
        povodna[polohy[4][0]][polohy[4][1]] = 'C';
        mapy[19].nazov = "npppn";
        mapy[19].mapa = set(povodna);
        povodna[polohy[3][0]][polohy[3][1]] = '3';
        mapy[20].nazov = "npnpn";
        mapy[20].mapa = set(povodna);
        povodna[polohy[2][0]][polohy[2][1]] = '2';
        povodna[polohy[3][0]][polohy[3][1]] = 'C';
        mapy[21].nazov = "nnppn";
        mapy[21].mapa = set(povodna);
        povodna[polohy[4][0]][polohy[4][1]] = '4';
        mapy[22].nazov = "nnpnn";
        mapy[22].mapa = set(povodna);
        povodna[polohy[3][0]][polohy[3][1]] = '3';
        povodna[polohy[4][0]][polohy[4][1]] = 'C';
        mapy[23].nazov = "nnnpn";
        mapy[23].mapa = set(povodna);
        povodna[polohy[5][0]][polohy[5][1]] = 'C';
        mapy[24].nazov = "nnnpp";
        mapy[24].mapa = set(povodna);
        povodna[polohy[4][0]][polohy[4][1]] = '4';
        mapy[25].nazov = "nnnnp";
        mapy[25].mapa = set(povodna);
        povodna[polohy[3][0]][polohy[3][1]] = 'C';
        mapy[26].nazov = "nnpnp";
        mapy[26].mapa = set(povodna);
        povodna[polohy[3][0]][polohy[3][1]] = '3';
        povodna[polohy[2][0]][polohy[2][1]] = 'C';
        mapy[27].nazov = "npnnp";
        mapy[27].mapa = set(povodna);
        povodna[polohy[2][0]][polohy[2][1]] = '2';
        povodna[polohy[3][0]][polohy[3][1]] = 'C';
        povodna[polohy[4][0]][polohy[4][1]] = 'C';
        mapy[28].nazov = "nnppp";
        mapy[28].mapa = set(povodna);
        povodna[polohy[4][0]][polohy[4][1]] = '4';
        povodna[polohy[2][0]][polohy[2][1]] = 'C';
        mapy[29].nazov = "nppnp";
        mapy[29].mapa = set(povodna);
        povodna[polohy[4][0]][polohy[4][1]] = 'C';
        povodna[polohy[3][0]][polohy[3][1]] = '3';
        mapy[30].nazov = "npnpp";
        mapy[30].mapa = set(povodna);
        povodna[polohy[3][0]][polohy[3][1]] = 'C';
        mapy[31].nazov = "npppp";
        mapy[31].mapa = set(povodna);
    }
    return mapy;
}

void print(MAPY *mapy, int n, int m){
    for(int i = 0; i < size; i++){
        printf("%s\n", mapy[i].nazov);
        for(int j = 0; j < n; j++){
            for(int k = 0; k < m; k++){
                printf("%c ", mapy[i].mapa[j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}
