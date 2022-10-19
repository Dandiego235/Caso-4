#ifndef SUBCAMARA

#define SUBCAMARA 1

using namespace std;

class SubCamara{
    private:
        SubCamara *left;
        SubCamara *right;
        int height;
        SubCamara *parent;

        int distanciaTotal;
        int potencial;
        int distancia;
        int mineralAzul;
        int mineralRojo;

    public:
        SubCamara(SubCamara* par, int pDistancia, int pMineral){
            left = nullptr;
            right = nullptr;
            height = 1;
            parent = par;
            distancia = pDistancia;
            mineralAzul = pMineral;
            mineralRojo = pMineral;
            potencial = pMineral * pDistancia;
            if (par){
                distanciaTotal = par->getDistanciaTotal() + distancia;
            } else {
                distanciaTotal = 0;
            }
            bool empty = false;
        }

        int getPotencial() {
            return potencial;
        }

        void setPotencial(int pPotencial) {
            this->potencial = pPotencial;
        }

        int getDistancia(){
            return distancia;
        }

        void setDistancia(int pDistancia){
            distancia = pDistancia;
        }

        int getMineralAzul(){
            return mineralAzul;
        }

        void setMineralAzul(int pMineral){
            mineralAzul = pMineral;
        }

        void decMineralAzul(int number){
            mineralAzul -= number;
        }

        bool isEmptyAzul(){
            return !mineralAzul;
        }

        int getMineralRojo(){
            return mineralRojo;
        }

        void setMineralRojo(int pMineral){
            mineralRojo = pMineral;
        }

        void decMineralRojo(int number){
            mineralRojo -= number;
        }

        bool isEmptyRojo(){
            return !mineralRojo;
        }

        int getHeight(){
            return height;
        }

        void setHeight(int pHeight){
            height = pHeight;
        }

        int getDistanciaTotal(){
            if (parent == nullptr){ // si es la raiz
                return distancia;
            }
            return distancia + parent->getDistanciaTotal();
        }

        void setDistanciaTotal(int pDistanciaTotal){
            distanciaTotal = pDistanciaTotal;
        }

        void setRight(SubCamara *pNode){
            right = pNode;
        }

        SubCamara* getRight(){
            return right;
        }

        void setLeft(SubCamara *pNode){
            left = pNode;
        }

        SubCamara* getLeft(){
            return left;
        }

        void setParent(SubCamara *pNode){
            parent = pNode;
        }

        SubCamara* getParent(){
            return parent;
        }
};

#endif