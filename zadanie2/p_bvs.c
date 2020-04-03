//
//  p_bvs.c
//  Projekt 2
//  Matej Hrnčiar
//
//  https://www.programiz.com/dsa/red-black-tree
//

#include "p_bvs.h"
#include <stdio.h>
#include <stdlib.h>

static struct rbNode *root = NULL;

static struct rbNode *createNode(int data){
    struct rbNode *newnode = (struct rbNode *) malloc(sizeof(struct rbNode));
    newnode->data = data;
    newnode->color = RED;
    newnode->link[0] = newnode->link[1] = NULL;
    return newnode;
}

void pbvs_insert(int data){
    struct rbNode *stack[98], *ptr, *newnode, *xPtr, *yPtr;
    int dir[98], ht = 0, index = 0;
    ptr = root;
    
    if(!root){
        root = createNode(data);
        return;
    }
    
    stack[ht] = root;
    dir[ht++] = 0;
    while(ptr != NULL){
        if(ptr->data == data){
            //printf("Duplicates Not Allowed!!\n");
            return;
        }
        
        index = (data - ptr->data) > 0 ? 1 : 0;
        stack[ht] = ptr;
        ptr = ptr->link[index];
        dir[ht++] = index;
    }
    
    stack[ht - 1]->link[index] = newnode = createNode(data);
    
    while((ht >= 3) && (stack[ht-1]->color == RED)){
        if(dir[ht-2] == 0){
            yPtr = stack[ht-2]->link[1];
            if(yPtr != NULL && yPtr->color == RED){
                stack[ht-2]->color = RED;
                stack[ht-1]->color = yPtr->color = BLACK;
                ht = ht - 2;
            }
            
            else{
                if(dir[ht-1] == 0)
                    yPtr = stack[ht-1];
                
                else{
                    xPtr = stack[ht-1];
                    yPtr = xPtr->link[1];
                    xPtr->link[1] = yPtr->link[0];
                    yPtr->link[0] = xPtr;
                    stack[ht-2]->link[0] = yPtr;
                }
                
                xPtr = stack[ht - 2];
                xPtr->color = RED;
                yPtr->color = BLACK;
                xPtr->link[0] = yPtr->link[1];
                yPtr->link[1] = xPtr;
                
                if(xPtr == root)
                    root = yPtr;
                
                else
                    stack[ht-3]->link[dir[ht-3]] = yPtr;
                
                break;
            }
        }
        
        else{
            yPtr = stack[ht-2]->link[0];
            if((yPtr != NULL) && (yPtr->color == RED)){
                stack[ht-2]->color = RED;
                stack[ht-1]->color = yPtr->color = BLACK;
                ht = ht - 2;
            }
            
            else{
                if(dir[ht-1] == 1)
                    yPtr = stack[ht-1];
                
                else{
                    xPtr = stack[ht-1];
                    yPtr = xPtr->link[0];
                    xPtr->link[0] = yPtr->link[1];
                    yPtr->link[1] = xPtr;
                    stack[ht-2]->link[1] = yPtr;
                }
                
                xPtr = stack[ht-2];
                yPtr->color = BLACK;
                xPtr->color = RED;
                xPtr->link[1] = yPtr->link[0];
                yPtr->link[0] = xPtr;
                
                if(xPtr == root)
                    root = yPtr;
                
                else
                    stack[ht-3]->link[dir[ht-3]] = yPtr;
                
                break;
            }
        }
    }
    root->color = BLACK;
}

static struct rbNode *pbvs_searchNode(struct rbNode *node, int key){
    if(node == NULL){
        printf("Key %d has not been found\n", key);
        return NULL;
    }
    
    if(key == node->data)
        return node;
    
    else if (key < node->data)
        return pbvs_searchNode(node->link[0], key);
    
    else if(key > node->data)
        return pbvs_searchNode(node->link[1], key);
    
    return node;
}

struct rbNode *pbvs_search(int key){
    struct rbNode *node = root;
    return pbvs_searchNode(node, key);
}

static void pbvs_inOrderTraversal(struct rbNode *node){
    if(node){
        pbvs_inOrderTraversal(node->link[0]);
        printf("%d ", node->data);
        pbvs_inOrderTraversal(node->link[1]);
    }
    return;
}

void pbvs_inOrder(){
    struct rbNode *node = root;
    pbvs_inOrderTraversal(node);
    printf("\n");
}
