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
        int mineral;
    
    public:
        SubCamara(SubCamara* par, int pDistancia, int pMineral){
            left = nullptr;
            right = nullptr;
            height = 1;
            parent = par;
            distancia = pDistancia;
            mineral = pMineral;
            potencial = pMineral * pDistancia;
            if (par){
                distanciaTotal = par->getDistanciaTotal() + distancia;
            } else {
                distanciaTotal = 0;
            }
            
        }

        int getPotencial() {
            return distancia * mineral;
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

        int getMineral(){
            return mineral;
        }

        void setMineral(int pMineral){
            mineral = pMineral;
        }

        int getHeight(){
            return height;
        }

        void setHeight(int pHeight){
            height = pHeight;
        }

        int getDistanciaTotal(){
            if (parent == nullptr){
                return 0;
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
