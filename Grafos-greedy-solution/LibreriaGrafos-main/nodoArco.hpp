#ifndef NODOARCO_H_
#define NODOARCO_H_
#include "nodoVertice.hpp"

template <typename tipo>
class Vertice;
template <typename Tipo>
class Arco{
    private:
        float peso;
        Vertice<Tipo> *info;
        Arco<Tipo> *sig;
    public:
        Vertice<Tipo>* getInfo(){
            return this->info;
        }
        Arco<Tipo> * getSig(){
            return this->sig;
        }
        float getPeso(){
            return this->peso;
        }
        void setInfo(Vertice<Tipo> *info){
            this->info = info;
        }
        void setSig(Arco<Tipo> *apuntador){
            this->sig = apuntador;
        }
        void setPeso(float peso){
            this->peso = peso;
        }
};
#endif