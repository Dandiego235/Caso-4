#ifndef AVL_TREE

#define AVL_TREE 1

#include "AVL_Node.h"
#include <iostream>
#include <string>

using namespace std;

class AVL_Tree{
    private:
        AVL_Node* raiz;
        AVL_Node* searchPointer;
        int size;

    public:
        AVL_Tree(){
            raiz = nullptr;
            searchPointer = nullptr;
            size = 0;
        }

        AVL_Node* getRaiz(){
            return raiz;
        }

        /*
            Function to get the height of the tree
        */
        int height(AVL_Node *N){
            if (N == NULL)
                return 0;
            return N->getHeight();
        }

        /*
            Function to create a new AVL_Node 
            Adding the newly created node as leaf node
        */

        AVL_Node* newAVL_Node(AVL_Node* par, IData* data){
            AVL_Node* node = new AVL_Node(par, data);
            size++;
            if (!raiz){
                raiz = node;
            }
            searchPointer = node; // ponemos a searchPointer a apuntar al nodo que acabamos de agregar.
            return(node);
        }

        int getSize(){
            return size;
        }

        /*
            Function to update the heights
        */
        void update_height(AVL_Node* root){
            if (root != NULL) {
                int val = 1;
                if (root->getLeft() != NULL)
                    val = root->getLeft()->getHeight() + 1;
                if (root->getRight() != NULL)
                    val = max(val, root->getRight()->getHeight() + 1);
                root->setHeight(val);
            }
        }

        /*
            Function for right rotation
        */
        /*   T1, T2, T3 and T4 are subtrees.
                     z                                     Z
                    / \                                  /   \
                  y   T4      Right Rotate (z)         x     T4
                / \          - - - - - - - - ->      /  \   
               x   T3                               T1  y
             / \                                       / \
            T1 T2                                     T2 T3

            y = node
            x = leftChild
            T2 = subtree
        */
        AVL_Node *rightRotate(AVL_Node *node){
            AVL_Node *leftChild = node->getLeft();
            AVL_Node *subtree = leftChild->getRight();

            if (leftChild->getRight() != NULL)
                leftChild->getRight()->setParent(node);

        /*
            Perform rotation
        */
            node->setLeft(subtree);
            leftChild->setRight(node);

            leftChild->setParent(node->getParent());
            node->setParent(leftChild);

            if (leftChild->getParent() != NULL && node->getData()->compareTo(leftChild->getParent()->getData()) == -1) {
                leftChild->getParent()->setLeft(leftChild);
            }
            else{
                if (leftChild->getParent() != NULL)
                    leftChild->getParent()->setRight(leftChild);
                else {
                    raiz = leftChild;
                }
            }

            node = leftChild;

        /*
            Update the heights
        */    
            update_height(node->getLeft());
            update_height(node->getRight());
            update_height(node);
            update_height(node->getParent());

        /*
            Return new root
        */
            return node;
        }

        /*
            Function for left rotation
        */
        /*        z                                z
                /  \                            /   \ 
               T1   x     Left Rotate(z)      T1     y
                  /  \   - - - - - - - ->           / \
                 T3   y                            x  T4
                    / \                          / \
                  T2  T4                       T3  T2
        
            x = node
            y = rightChild
            T2 = subtree
        */
        AVL_Node *leftRotate(AVL_Node *node){
            AVL_Node *rightChild = node->getRight();
            AVL_Node *subtree = rightChild->getLeft();

        /*
            Perform rotation
        */
            node->setRight(subtree);

            if (rightChild->getLeft() != NULL)
                rightChild->getLeft()->setParent(node);
            rightChild->setLeft(node);

            rightChild->setParent(node->getParent());
            node->setParent(rightChild);

            if (rightChild->getParent() != NULL && node->getData()->compareTo(rightChild->getParent()->getData()) == -1) {
                rightChild->getParent()->setLeft(rightChild);
            }
            else{
                if (rightChild->getParent() != NULL)
                    rightChild->getParent()->setRight(rightChild);
                else {
                    raiz = rightChild;
                }
            }

        /*
            Update heights
        */

            update_height(node->getLeft());
            update_height(node->getRight());
            update_height(node);
            update_height(node->getParent());

        /*
            Return new root
        */
            return rightChild;
        }

        /*
            Function to find the Balance factor of Nodes
        */
        int getBalance(AVL_Node *N){
            if (N == NULL)
                return 0;
            return height(N->getLeft()) - height(N->getRight());
        }


        /*
            Function to construct a tree.
        */
        AVL_Node* AVL_insert(IData *pData){
            searchPointer = nullptr; // ponemos a searchPointer a apuntar para indicar cuando no se agrega un nodo.
            AVL_insert(NULL, raiz, pData);
            return searchPointer; // retornamos el nodo insertado.
        }

        AVL_Node* AVL_insert(AVL_Node *par, AVL_Node* root, IData *pData){
        /*
            Perform the normal BST insertion
        */
            if (root == NULL)
                return(newAVL_Node(par, pData));

            if (pData->compareTo(root->getData()) == -1){ // si el data es menor que el valor del nodo.
                root->setLeft(AVL_insert(root, root->getLeft(), pData));

            } else if (pData->compareTo(root->getData()) == 1) { // si el data es mayor que el valor del nodo.
                root->setRight(AVL_insert(root, root->getRight(), pData));
            } else { 
                return root;
            }

        /*
            Step 1: Find the balance factor of parent
        */
            int balance = getBalance(root);
        /*
            If this Node becomes unbalanced, all four cases are:
        */
        /*
            1. Left Left Case
        */
            if (balance > 1 && pData->compareTo(root->getLeft()->getData()) == -1)
                return rightRotate(root);

        /*
            2. Right Right Case
        */
            if (balance < -1 && pData->compareTo(root->getRight()->getData()) == 1)
                return leftRotate(root);

        /*
            3. Left Right Case
        */
            if (balance > 1 && pData->compareTo(root->getLeft()->getData()) == 1){
                root->setLeft(leftRotate(root->getLeft()));
                return rightRotate(root);
            }

        /* 
            4. Right Left Case
        */
            if (balance < -1 && pData->compareTo(root->getRight()->getData()) == -1){
                root->setRight(rightRotate(root->getRight()));
                return leftRotate(root);
            }

        /*
            Update the heights
        */
            update_height(root);

        /*
            Return the (unchanged) Node pointer
        */
            return root;
        }

        /*
            AVL Tree Traversal 
        
        void PREORDER(AVL_Node *root){
            cout << "Node: " << root->key << ", Parent Node: ";

            if (root->getParent() != NULL)
                cout << root->getParent()->key << endl;
            else
                cout << "NULL" << endl;
            /* 
                Recur to the left subtree
            
            if (root->getLeft() != NULL) {
                PREORDER(root->getLeft());
            }

            /*
                Recur to the right subtree
            
            if (root->getRight() != NULL) {
                PREORDER(root->getRight());
            }
        }*/

        /*
            Function to find the AVL_Node with minimum key value
        */
        AVL_Node * minValueAVL_Node(AVL_Node* node){
            AVL_Node* current = node;
            /*
                Going to the left most side
            */
            while (current->getLeft() != NULL)
                current = current->getLeft();
            return current;
        }

        AVL_Node* AVL_delete(IData *pData){
            return AVL_delete(raiz, pData);
        }
        /*
            Function to delete an AVL_Node with the given key from the subtree 
        */
        AVL_Node* AVL_delete(AVL_Node* root, IData *pData){
        /*
            Perform normal BST deletion
        */
            if (root == NULL)
                return root;
        /*
            Find the node to be deleted
        */
        /*
            Left Side
        */
            if (pData->compareTo(root->getData()) == -1) {
                root->setLeft(AVL_delete(root->getLeft(), pData));

        /*
            Right Side
        */
            } else if(pData->compareTo(root->getData()) == 1) {
                root->setRight(AVL_delete(root->getRight(), pData));

        /*
            Root Node: EL que voy a borrar.
        */
            } else {
                if( (root->getLeft() != NULL) && (root->getRight() == NULL) ){ // si solo tiene hijo izquierdo.
                    AVL_Node *temp = root->getLeft();

                    if (root->getParent() != NULL) { // si no es la raiz del arbol
                        if (root->getParent()->getData()->compareTo(root->getData()) == -1)
                            // si estamos borrando el hijo derecho, enlazamos el padre con el hijo izquierdo del borrado.
                            root->getParent()->setRight(root->getLeft());
                        else
                            // si estamos borrando el hijo izquierdo, enlazamos el padre con el hijo izquierdo del borrado.
                            root->getParent()->setLeft(root->getLeft());

                        /*
                            Update the height of root's parent
                        */
                        update_height(root->getParent());
                    }
                    root->getLeft()->setParent(root->getParent());
                /*
                    One child case
                */
                *root = *temp; /*
                    Copy the contents of the non-empty child
                */
                delete(temp);

            } else if((root->getLeft() == NULL) && (root->getRight() != NULL)){ // si solo tiene un hijo derecho.
                AVL_Node *temp = root->getRight();
                
                if (root->getParent() != NULL) {
                    if (root->getParent()->getData()->compareTo(root->getData()) == -1)
                        // si estamos borrando el hijo derecho, enlazamos el padre con el hijo izquierdo del borrado.
                        root->getParent()->setRight(root->getRight());
                    else
                        root->getParent()->setLeft(root->getRight());
                        /*
                            Update the height of root's parent
                        */
                    update_height(root->getParent());
                }
                root->getRight()->setParent(root->getParent());
                /*
                    One child case
                */
                *root = *temp; /*
                    Copy the contents of the non-empty child
                */
                delete(temp);

            } else if ((root->getLeft() == NULL) && (root->getRight() == NULL)){ // case with no children

                if (root->getParent()->getData()->compareTo(root->getData()) == -1) { // si estamos borrando uno de la derecha
                    root->getParent()->setRight(NULL);

                } else if (root->getParent()->getData()->compareTo(root->getData()) == 1) { // si estamos borrando uno de la izquierda
                    root->getParent()->setLeft(NULL);

                } else { //if (root->getParent()->getData() == root->getData()){
                    if (root->getParent()->getLeft() == root){
                        root->getParent()->setLeft(NULL);
                    } else {
                        root->getParent()->setRight(NULL);
                    }
                }

                if (root->getParent() != NULL)
                    update_height(root->getParent());
                AVL_Node *temp = root;
                root = NULL;
            } else{
                /*
                    AVL_Node with two children: Get the inorder successor (smallest in the right subtree)
                */
                AVL_Node* temp = minValueAVL_Node(root->getRight());
                /*
                    Copy the inorder successor's data to this AVL_Node
                */
                root->setData(temp->getData());
                /*
                    Delete the inorder successor
                */
                root->setRight(AVL_delete(root->getRight(),temp->getData()));
            }
        }

        /*
            If the tree had only one AVL_Node then return
        */
        if (root == NULL)
            return root;

        /*
            UPDATE HEIGHT OF THE CURRENT AVL_Node
        */
        update_height(root);

        /*
            GET THE BALANCE FACTOR OF THIS AVL_Node (to check whether this AVL_Node became unbalanced)
        */
        int balance = getBalance(root);

        /*
            If this AVL_Node becomes unbalanced, then there are 4 cases
        */

        /*
            Left Left Case
        */
        if (balance > 1 && getBalance(root->getLeft()) >= 0)
            return rightRotate(root);

        /*
            Left Right Case
        */
        if (balance > 1 && getBalance(root->getLeft()) < 0){
            root->setLeft(leftRotate(root->getLeft()));
            return rightRotate(root);
        }

        /*
            Right Right Case
        */
        if (balance < -1 && getBalance(root->getRight()) <= 0)
            return leftRotate(root);

        /*
            Right Left Case
        */
        if (balance < -1 && getBalance(root->getRight()) > 0){
            root->setRight(rightRotate(root->getRight()));
            return leftRotate(root);
        }

        return root;
        }

        // Print the tree
        void printTree(){
            printTree(raiz, "", true);
        }

        void printTree(AVL_Node *pRoot, string indent, bool last) {
            if (pRoot != nullptr) {
                cout << indent;
                if (last) {
                cout << "R----";
                indent += "   ";
                } else {
                cout << "L----";
                indent += "|  ";
                }
                cout << pRoot->getData()->toString() << endl;
                printTree(pRoot->getLeft(), indent, false);
                printTree(pRoot->getRight(), indent, true);
            }
        }

        AVL_Node* search(IData *pData){
            AVL_Node* nodePtr = raiz;
            searchPointer = raiz;
            while(nodePtr){
                searchPointer = nodePtr; // el searchPointer apunta al padre del nodo que vayamos a insertar.
                if (!pData->compareTo(nodePtr->getData())){ // si es 0.
                    // si son iguales, no debemos realizar nada, entonces salimos.
                    return nodePtr;
                } else if (pData->compareTo(nodePtr->getData())){ // si es menor, vamos al subárbol izquierdo
                    nodePtr = nodePtr->getLeft(); // nos movemos el subarbol izquierdo;
                } else { 
                    nodePtr = nodePtr->getRight(); // nos movemos el subarbol derecho;
                }
            }
            return NULL; // si no lo encontró, retorna NULL.
        }
};

#endif