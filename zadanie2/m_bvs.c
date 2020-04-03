//
//  m_bvs.c
//  Projekt 2
//  Matej Hrnčiar
//

#include "m_bvs.h"
#include <stdio.h>
#include <stdlib.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))

static int height(NODE *node){
    if (node == NULL)
        return 0;
    return node->height;
}

static int getBalance(NODE *node){
    if(node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

void mbvs_preOrder(NODE *node){
    if(node){
        printf("%d ", node->key);
        mbvs_preOrder(node->left);
        mbvs_preOrder(node->right);
    }
    return;
}

static NODE *rotateLeft(NODE *x){
    NODE *y = x->right;
    NODE *t = y->left;
    
    y->left = x;
    x->right = t;
    
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
    
    return y;
}

static NODE *rotateRight(NODE *y){
    NODE *x = y->left;
    NODE *t = x->right;
    
    x->right = y;
    y->left = t;
    
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;
    
    return x;
}

static NODE *mbvs_newNode(int key){
    NODE *new = (NODE *) malloc(sizeof(NODE));
    
    new->key = key;
    new->height = 1;
    new->left = new->right = NULL;
    return new;
}

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
