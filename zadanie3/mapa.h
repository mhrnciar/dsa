//
//  mapa.h
//  Popolvar
//  Matej Hrnciar
//

#ifndef mapa_h
#define mapa_h

#include <stdio.h>
typedef struct mapy{
    char *nazov;
    char **mapa;
} MAPY;

extern MAPY *createMaps(char **mapa, int n, int m);
extern void print(MAPY *mapy, int n, int m);

#endif /* mapa_h */
