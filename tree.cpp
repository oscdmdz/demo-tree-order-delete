#include <stdio.h>
#include <stdlib.h>
#include <cstddef>
#include <stack>
#include <iostream>
#include <fstream>
typedef struct Tree{
    int key;
    struct Tree *left,*right;
    int height;
}tree;

int height(tree *t)
{
    if (t == NULL)
        return 0;
    return t->height;
}

int getBalance(tree *t)
{
    if(t==NULL)
        return 0;
    return height(t->left)-height(t->right);
}

int max(int a, int b)
{
    return (a > b)? a : b;
}

tree* newNode(int key)
{
    tree* node = (tree*)malloc(sizeof(tree));
    node->key   = key;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;  // new node is initially added at leaf
    return(node);
}
tree* insert(tree* node, int key)
{
    /* 1.  Perform the normal BST rotation */
    if (node == NULL)
        return(newNode(key));
    
    if (key < node->key)
        node->left  = insert(node->left, key);
    else
        node->right = insert(node->right, key);
    
    /* 2. Update height of this ancestor node */
    node->height = max(height(node->left), height(node->right)) + 1;
    return node;
}

void print_preorder(tree * tTree,std::ofstream &myfile)
{

    if(tTree)
    {
        printf("%d ",tTree->key);
        
        if(tTree->left){
             myfile << tTree->key<< "->" << tTree->left->key<<";"<<"\n";
        }
        
        if(tTree->right){
           myfile << tTree->key << "->" << tTree->right->key<<";"<<"\n";
        }
       
//        myfile << tTree->key << "->" << tTree->right->key<<";"<<"\n";
        print_preorder(tTree->left, myfile);
        print_preorder(tTree->right, myfile);
    }
   
}

void deleteTree(tree * tTree)
{
    if (tTree)
    {
        deleteTree(tTree->left);
        deleteTree(tTree->right);
        free(tTree);
    }
}

tree* del(tree *tTree, int delItem) /* delete( main body) */
{
    if(!(tTree))
        return tTree;
    if(delItem < tTree->key)
        tTree->left = del(tTree->left, delItem);
    else
        if(delItem > tTree->key)
            tTree->right = del(tTree->right, delItem);
        else
        {
            tree *oTree = tTree;
            if((tTree->left) && (tTree->right))
            {
                tree *parent = tTree->right;
                tTree = parent->left;
                if (tTree)
                {
                    while(tTree->left)
                    {
                        parent = tTree;
                        tTree = tTree->left;
                    }
                    parent->left = tTree->right;
                    tTree->right = oTree->right;
                }
                else
                    tTree = parent;
                tTree->left = oTree->left;
            }
            else
                if(tTree->left)
                    tTree = tTree->left;
                else
                    tTree = tTree->right;
            free(oTree);
        }
    return tTree;
}

tree* rotateLeft(tree* t)
{
    tree *y = t->right;
    tree *T2 = y->left;
    
    // Perform rotation
    y->left = t;
    t->right = T2;
    
    
    // Update heights
    t->height = max(height(t->left), height(t->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
    
    // Return new root
    return y;
}

tree* rotateRight(tree* t)
{
    tree *x = t->left;
    tree *T2 = x->right;
    
    // Perform rotation
    x->right = t;
    t->left = T2;
    
    // Update heights
    t->height = max(height(t->left), height(t->right))+1;
    x->height = max(height(x->left), height(x->right))+1;
    
    // Return new root
    return x;
}

tree *balanceNode(tree *t)
{
    if (!t) return t;
    int balance = getBalance(t);
    if (balance > 1 && getBalance(t->left) >= 0)   /* rotateReight */
        return rotateRight(t);
    if (balance < -1 && getBalance(t->right) <= 0) /* rotateLeft */
        return rotateLeft(t);
    if (balance > 1 && getBalance(t->left) < 0)    /* rotateLeftReight */
    {
        t->left = rotateLeft(t->left);
        return rotateRight(t);
    }
    if (balance < -1 && getBalance(t->right) > 0)  /* rotateReightLeft */
    {
        t->right = rotateRight(t->right);
        return rotateLeft(t);
    }
    return t;
}


int main(void)
{
    tree *Tree;
    Tree = NULL;
    int i, a[]={7,3,9,10,8,1,4,2,6,5};
    for(i=0;i<10;i++) /* construct tree */
        Tree = insert(Tree,a[i]);
    /* Printing nodes of tree */
    printf("Pre Order Display\n");
//    print_preorder(Tree);
    
    printf("\nBalanced Tree\n");
    Tree=balanceNode(Tree);
    std::ofstream myfile;
    myfile.open ("/Users/gladysmaquera/Documents/maestria/example.dot");
    myfile << "digraph G{\n node [shape=box, color=blue]\n";
    
    print_preorder(Tree,myfile);
    
    myfile << "}\n";
    myfile.close();
    
    system("/usr/local/bin/dot -Tpdf /Users/gladysmaquera/Documents/maestria/example.dot -o /Users/gladysmaquera/Documents/maestria/example.pdf");
    
    
    
    while(1)
    {
        printf("\nEnter an Item that You Want to Delete: ");
        scanf("%d",&i);
        printf("\n");
        Tree = del(Tree,i);
        printf("After Delete\n");
        /*  Pre-order displays root node, left node and then right node. */
//        print_preorder(Tree);
        Tree = balanceNode(Tree);
        printf("\nAfter Balance\n");
//        print_preorder(Tree);
        
        std::ofstream myfile;
        myfile.open ("/Users/gladysmaquera/Documents/maestria/example1.dot");
        myfile << "digraph G{\n node [shape=box, color=blue]\n";
        
        print_preorder(Tree,myfile);
        
        myfile << "}\n";
        myfile.close();
        
        system("/usr/local/bin/dot -Tpdf /Users/gladysmaquera/Documents/maestria/example1.dot -o /Users/gladysmaquera/Documents/maestria/example1.pdf");
    }
    
    printf("\n");
    
    deleteTree(Tree);
    printf("\n");
    return 0;
}


