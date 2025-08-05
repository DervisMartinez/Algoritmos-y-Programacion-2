#ifndef NODOVERTICE_H_
#define NODOVERTICE_H_
//#include "nodoArco.hpp"


template <typename T>
class Arco;
template <typename T>
class Vertice{
    private:
        T info;
        Vertice<T> *sig;
        Arco<T> *listaAdy;
        int gradoIn, gradoOut;
    public:
        T getInfo(){
            return this->info;
        }
        Vertice<T> * getSig(){
            return this->sig;
        }
        Arco<T> * getArcos(){
            return this->listaAdy;
        }
        int getGradoIn(){
            return this->gradoIn;
        }
        int getGradoOut(){
            return this->gradoOut;
        }
        void setInfo(T info){
            this->info = info;
        }
        void setSig(Vertice<T> *apuntador){
            this->sig = apuntador;
        }
        void setArcos(Arco<T> *adyacente){
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