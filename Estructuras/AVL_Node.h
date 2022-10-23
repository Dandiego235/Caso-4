#ifndef AVL_NODE

#define AVL_NODE 1

#include "IData.h"

using namespace std;

class AVL_Node{
    private:
        IData* data;
        AVL_Node *left;
        AVL_Node *right;
        int height;
        AVL_Node *parent;
        
    public:
        AVL_Node(AVL_Node* par, IData *pData){
            data = pData;
            left = nullptr;
            right = nullptr;
            height = 1;
            parent = par;
        }

        IData* getData(){
            return data;
        }

        void setData(IData *pData){
            data = pData;
        }

        void setRight(AVL_Node *pNode){
            right = pNode;
        }

        AVL_Node* getRight(){
            return right;
        }

        void setLeft(AVL_Node *pNode){
            left = pNode;
        }

        AVL_Node* getLeft(){
            return left;
        }

        void setParent(AVL_Node *pNode){
            parent = pNode;
        }

        AVL_Node* getParent(){
            return parent;
        }

        int getHeight(){
            return height;
        }

        void setHeight(int pHeight){
            height = pHeight;
        }
};

#endif