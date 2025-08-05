#ifndef NODOARCO_H_
#define NODOARCO_H_

#include "nodoVertice.hpp"

template <typename T>
class Vertice; // Forward declaration

template <typename T>
class Arco {
private:
    float peso = 0.0f;
    Vertice<T>* info = nullptr;
    Arco<T>* sig = nullptr;

public:
    // Constructor por defecto
    Arco() = default;

    // Constructor con parámetros
    Arco(Vertice<T>* info, float peso = 0.0f, Arco<T>* sig = nullptr)
        : peso(peso), info(info), sig(sig) {}

    // Getters const
    Vertice<T>* getInfo() const { return info; }
    Arco<T>* getSig() const { return sig; }
    float getPeso() const { return peso; }

    // Setters
    void setInfo(Vertice<T>* info) { this->info = info; }
    void setSig(Arco<T>* sig) { this->sig = sig; }
    void setPeso(float peso) { this->peso = peso; }
};

#endif // NODOARCO_H_
