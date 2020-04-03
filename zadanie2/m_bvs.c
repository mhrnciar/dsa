//
//  m_bvs.c
//  Projekt 2
//  Matej Hrnčiar
//

#include "m_bvs.h"
#include <stdio.h>
#include <stdlib.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))

// vracia vysku daneho vrcholu
static int height(NODE *node){
    if (node == NULL)
        return 0;
    return node->height;
}

// vracia rozdiel vysok medzi lavou a pravou vetvou vrchola
static int getBalance(NODE *node){
    if(node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// vypis stromu
void mbvs_preOrder(NODE *node){
    if(node){
        printf("%d ", node->key);
        mbvs_preOrder(node->left);
        mbvs_preOrder(node->right);
    }
    return;
}

// rotacia dolava a nastavenie novych vysok vrchola
static NODE *rotateLeft(NODE *x){
    NODE *y = x->right;
    NODE *t = y->left;
    
    y->left = x;
    x->right = t;
    
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
    
    return y;
}

// rotacia doprava a nastavenie novych vysok vrchola
static NODE *rotateRight(NODE *y){
    NODE *x = y->left;
    NODE *t = x->right;
    
    x->right = y;
    y->left = t;
    
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;
    
    return x;
}

// tvorba noveho vrchola
static NODE *mbvs_newNode(int key){
    NODE *new = (NODE *) malloc(sizeof(NODE));
    
    new->key = key;
    new->height = 1;
    new->left = new->right = NULL;
    return new;
}

/*
 * Najprv sa rekurzivne prejde strom kym sa nenajde vhodne miesto pre novy vrchol. Potom sa nastavi nova vyska
 * vrcholov a zistuje sa ci je strom vyvazeny. Ak nie je, tak dochadza k rotacii a to tak, ze ked je vpravo viac
 * vrcholov, tak sa rotuje dolava, ak je ich menej, tak sa rotuje doprava. Zavisi aj od toho ci je vkladany kluc
 * vacsi alebo mensi ako kluc vo vrchole - vtedy treba urobit este jednu rotaciu do opacneho smeru navyse.
 */
NODE *mbvs_insert(NODE *node, int key){
    if(node == NULL)
        return mbvs_newNode(key);
    
    if(key < node->key)
        node->left = mbvs_insert(node->left, key);
    
    else if(key > node->key)
        node->right = mbvs_insert(node->right, key);
    
    else
        return node;
    
    
    node->height = 1 + max(height(node->left), height(node->right));
    
    int balance = getBalance(node);
    
    if(balance > 1 && key > node->left->key){
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    if(balance > 1 && key < node->left->key){
        return rotateRight(node);
    }
    
    if(balance < -1 && key > node->right->key){
        return rotateLeft(node);
    }
    
    if(balance < -1 && key < node->right->key){
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

/*
 * Vyhladavanie prebieha taktiez rekurzivne, ak je hladany kluc mensi ako kluc vo vrchole, tak sa pokracuje
 * lavou vetvou, ak je vacsi, tak pravou vetvou. Pokracuje sa az kym sa nenajde dany kluc alebo NULL.
 */
NODE *mbvs_search(NODE *node, int key){
    if(node == NULL){
        printf("Kľúč %d sa v strome nenachádza\n", key);
        return NULL;
    }
    
    if(key == node->key)
        return node;
    
    else if (key < node->key)
        return mbvs_search(node->left, key);
    
    else if(key > node->key)
        return mbvs_search(node->right, key);
    
    return node;
}
