#ifndef NODEAVL

#define NODEAVL 1

#include <iostream>

using namespace std;

template <class T>
class NodeAVL{
    private:
        NodeAVL *right;
        NodeAVL *left;
        NodeAVL *parent;
        T* data;
        int fe; // factor de equilibrio
        int heightLeft;
        int heightRight;
    
    public:
        NodeAVL() {
            data = NULL;
            right = NULL;
            left = NULL;
            parent = NULL;
            fe = 0;
            heightLeft = 0;
            heightRight = 0;
        }

        NodeAVL(T *pData) {
            data = pData;
            right = NULL;
            left = NULL;
            parent = NULL;
            fe = 0;
            heightLeft = 0;
            heightRight = 0;
        }

        T* getData() {
            return data;
        }

        void setData(T *pData) {
            this->data = pData;
        }

        void setRight(NodeAVL *pNode){
            right = pNode;
        }

        NodeAVL* getRight(){
            return right;
        }

        void setLeft(NodeAVL *pNode){
            left = pNode;
        }

        NodeAVL* getLeft(){
            return left;
        }

        void setParent(NodeAVL *pNode){
            parent = pNode;
        }

        NodeAVL* getParent(){
            return parent;
        }

        void setFE(int pFE){
            fe = pFE;
        }

        int getFE(){
            return fe;
        }

        int decFE(){
            return --fe;
        }

        int incFE(){
            return ++fe;
        }

        void setHeightLeft(int pHeightLeft){
            heightLeft = pHeightLeft;
        }

        int getHeightLeft(){
            return heightLeft;
        }

        void setHeightRight(int pHeightRight){
            heightRight = pHeightRight;
        }

        int getHeightRight(){
            return heightRight;
        }

};

#endif