#include <iostream>
using namespace std;


class NodeB{
    public:
        int *claves; // array de claves
        int t; // numero de grados
        NodeB **nodeB;  // arreglo de nodosB
        int n;  // clave actual
        bool hoja;  // boleano para saber si es una hoja o no
        
        NodeB(int t, bool hoja);

        void insertar(int k);
};


NodeB::NodeB(int t, bool hoja) {
    this->t = t;
    this->hoja = hoja;

    claves = new int[2 * t - 1];
    nodeB = new NodeB*[2 * t];

    n = 0;
}


class ArbolB{
    public:
        int t;
        NodeB *raiz;

        ArbolB(int t){
            this->t = t;
            raiz = nullptr;
        }

        void insertar(int k){
            if(raiz == nullptr){
                raiz = new NodeB(t, true);
                raiz->claves[0] = k;
                raiz->n = 1;
            }
            else{
                if(raiz->n == 2 * t - 1){
                    NodeB *s = new NodeB(t, false);
                    s->nodeB[0] = raiz;
                    
                }
            }
        }
};


