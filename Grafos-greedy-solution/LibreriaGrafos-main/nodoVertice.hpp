#ifndef NODOVERTICE_H_
#define NODOVERTICE_H_
//#include "nodoArco.hpp"


template <typename tipo>
class Arco;
template <typename tipo>
class Vertice{
    private:
        tipo info;
        Vertice<tipo> *sig;
        Arco<tipo> *listaAdy;
        int gradoIn, gradoOut;
    public:
        tipo getInfo(){
            return this->info;
        }
        Vertice<tipo> * getSig(){
            return this->sig;
        }
        Arco<tipo> * getArcos(){
            return this->listaAdy;
        }
        int getGradoIn(){
            return this->gradoIn;
        }
        int getGradoOut(){
            return this->gradoOut;
        }
        void setInfo(tipo info){
            this->info = info;
        }
        void setSig(Vertice<tipo> *apuntador){
            this->sig = apuntador;
        }
        void setArcos(Arco<tipo> *adyacente){
            this->listaAdy = adyacente;
        }
        void setGradoIn(int nuevo){
            this->gradoIn = nuevo;
        }
        void setGradoOut(int nuevo){
            this->gradoOut = nuevo;
        }
};
#endif