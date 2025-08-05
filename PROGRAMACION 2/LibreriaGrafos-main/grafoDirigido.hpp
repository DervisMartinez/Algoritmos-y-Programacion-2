#ifndef GRAFODDIRIGIDO_H_
#define GRAFODDIRIGIDO_H_

#include <iostream>
#include <list>
#include <vector>
#include "grafo.hpp"
using namespace std;

template <typename T>
class GrafoNoDirigido;

template <typename T>
class GrafoDirigido : public Grafo<T> {
public:
    void copiar(Grafo<T> *A);
    GrafoDirigido<int> mapear(vector<T> *mapeo);
    GrafoNoDirigido<T> convertirEnNoDirigido();
    
    // Características de vértices
    bool esSumidero(T v);
    bool esFuente(T v);

    // Recorridos
    list<T> listaBFS(T inicio);
    list<T> listaDFS(T inicio);
    vector<T> DFSMenor(T inicio);

    // Componentes
    int NComponentesFConexas();

    // Caminos
    list<T> caminoMenor(T v, T w, float *peso = NULL);
    list<T> caminoMenorConBloqueo(T v, T w, list<T> bloqueados, float *peso = NULL);
    list<T> caminoMenorConRequisito(T v, T w, T H, float *peso = NULL);
    list<T> caminoMayor(T v, T w);

    // Caminos y ciclos especiales
    list<list<T>> caminosHamiltonianos();
    list<T> caminoHamiltonianoMinimo();
    list<list<T>> ciclosHamiltonianos();
    list<list<int>> caminosEulerianos();
    void eulerianos(list<int> *cam, int v, list<list<int>> *caminos);
    list<list<T>> getCaminosEulerianos();
};

// Implementación de métodos

template<typename T>
void GrafoDirigido<T>::copiar(Grafo<T> *A) {
    Vertice<T> *aux = A->getPrimero();
    while(aux != NULL) {
        this->agregarVertice(aux->getInfo());
        aux = aux->getSig();
    }
    aux = A->getPrimero();
    while(aux != NULL) {
        Arco<T> *arco = aux->getArcos();
        while(arco != NULL) {
            this->agregarArco(aux->getInfo(), arco->getInfo()->getInfo(), arco->getPeso());
            arco = arco->getSig();
        }
        aux = aux->getSig();
    }
}

template<typename T>
GrafoDirigido<int> GrafoDirigido<T>::mapear(vector<T> *mapeo) {
    GrafoDirigido<int> grafo;
    grafo.construir();
    Vertice<T> *actual = this->primero;
    int i = 0;
    while(actual != NULL) {
        mapeo->emplace_back(actual->getInfo());
        grafo.agregarVertice(i);
        actual = actual->getSig();
        i++;
    }
    actual = this->primero;
    i = 0;
    while(actual != NULL) {
        Arco<T> *arco = actual->getArcos();
        while(arco != NULL) {
            int v = this->buscarMapeo(*mapeo, arco->getInfo()->getInfo(), this->getNVertices());
            grafo.agregarArco(i, v, arco->getPeso());
            arco = arco->getSig();
        }
        actual = actual->getSig();
        i++;
    }
    return grafo;
}

template<typename T>
GrafoNoDirigido<T> GrafoDirigido<T>::convertirEnNoDirigido() {
    GrafoNoDirigido<T> C;
    C.construir();
    C.copiar(this);
    return C;
}

template <typename T>
bool GrafoDirigido<T>::esSumidero(T v) {
    return this->sucesores(v).empty();
}

template <typename T>
bool GrafoDirigido<T>::esFuente(T v) {
    return this->predecesores(v).empty();
}

// =============================== RECORRIDOS ===============================

template<typename T>
list<T> GrafoDirigido<T>::listaBFS(T inicio) {
    list<T> recorrido;
    list<int> recorridoAux;
    vector<T> mapeo;
    vector<bool> visitados(this->getNVertices(), false);

    GrafoDirigido<int> aux = this->mapear(&mapeo);
    int v = this->buscarMapeo(mapeo, inicio, this->getNVertices());
    visitados[v] = true;
    aux.BFS(v, &visitados, &recorridoAux);

    for(auto id : recorridoAux)
        recorrido.push_back(mapeo[id]);
    
    return recorrido;
}

template<typename T>
list<T> GrafoDirigido<T>::listaDFS(T inicio) {
    list<T> recorrido;
    list<int> recorridoAux;
    vector<T> mapeo;
    vector<bool> visitados(this->getNVertices(), false);

    GrafoDirigido<int> aux = this->mapear(&mapeo);
    int v = this->buscarMapeo(mapeo, inicio, this->getNVertices());
    visitados[v] = true;
    aux.DFS(v, &visitados, &recorridoAux);

    for(auto id : recorridoAux)
        recorrido.push_back(mapeo[id]);
    
    return recorrido;
}

template<typename T>
vector<T> GrafoDirigido<T>::DFSMenor(T inicio) {
    vector<T> recorrido;
    list<T> sucesores;
    T v = inicio;

    recorrido.emplace_back(v);
    for (int i = 1; i < this->getNVertices(); ++i) {
        sucesores = this->sucesores(v);
        v = min(sucesores);
        recorrido.emplace_back(v);
    }
    return recorrido;
}

// =============================== COMPONENTES ===============================

template<>
int GrafoDirigido<int>::NComponentesFConexas() {
    int nComponentes = 1;
    vector<bool> visitados(this->getNVertices(), false);

    for (int j = 0; j < this->getNVertices(); j++) {
        if(j > 0)
            fill(visitados.begin(), visitados.end(), false);

        this->BFS(j, &visitados);

        for (int i = 0; i < this->getNVertices(); i++) {
            if (!visitados[i]) {
                this->BFS(i, &visitados);
                nComponentes++;
            }
        }
    }
    return nComponentes;
}

// =============================== CAMINOS ===============================

template<typename T>
list<T> GrafoDirigido<T>::caminoMenor(T v, T w, float *peso) {
    vector<T> mapeo;
    GrafoDirigido<int> aux = this->mapear(&mapeo);
    int inicio = this->buscarMapeo(mapeo, v, this->getNVertices());
    int fin = this->buscarMapeo(mapeo, w, this->getNVertices());
    list<int> camino = aux.caminoDijkstra(inicio, fin, peso);

    list<T> resultado;
    for (auto id : camino)
        resultado.push_back(mapeo[id]);
    
    return resultado;
}

template<typename T>
list<T> GrafoDirigido<T>::caminoMenorConBloqueo(T v, T w, list<T> bloqueados, float *peso) {
    vector<T> mapeo;
    GrafoDirigido<int> aux = this->mapear(&mapeo);
    int inicio = this->buscarMapeo(mapeo, v, this->getNVertices());
    int fin = this->buscarMapeo(mapeo, w, this->getNVertices());
    vector<bool> bloqueos(this->nVertices, false);

    for (auto b : bloqueados)
        bloqueos[this->buscarMapeo(mapeo, b, this->getNVertices())] = true;

    list<int> camino = aux.caminoObstaculos(inicio, fin, bloqueos, peso);

    list<T> resultado;
    for (auto id : camino)
        resultado.push_back(mapeo[id]);
    
    return resultado;
}

template<typename T>
list<T> GrafoDirigido<T>::caminoMenorConRequisito(T v, T w, T H, float *peso) {
    list<T> camino1 = this->caminoMenor(v, H, peso);
    if(camino1.empty()) return {};

    list<T> bloqueos = camino1;
    bloqueos.pop_back();
    list<T> camino2 = this->caminoMenorConBloqueo(H, w, bloqueos, peso);

    list<T> resultado;
    resultado.splice(resultado.end(), camino1);
    if (!camino2.empty()) {
        camino2.pop_front();
        resultado.splice(resultado.end(), camino2);
    }
    return resultado;
}

template<typename T>
list<T> GrafoDirigido<T>::caminoMayor(T v, T w) {
    vector<T> mapeo;
    GrafoDirigido<int> aux = this->mapear(&mapeo);
    int inicio = this->buscarMapeo(mapeo, v, this->getNVertices());
    int fin = this->buscarMapeo(mapeo, w, this->getNVertices());
    list<int> camino, caminoMayor;
    vector<bool> visitados(this->getNVertices(), false);
    float peso = 0, pesoMayor = -1;
    bool primero = true;

    camino.push_back(inicio);
    aux.mayorCamino(inicio, fin, peso, &visitados, &camino, &pesoMayor, &caminoMayor, &primero);

    list<T> resultado;
    for (auto id : caminoMayor)
        resultado.push_back(mapeo[id]);
    
    return resultado;
}
template <typename T>
list<list<T>> GrafoDirigido<T>::caminosHamiltonianos(){
    int i,j, nVisitados = 1;
    list<list<int>> result;
    list<list<T>> hamiltonianos;
    list<int> camAux;
    list<T> cam;
    vector<bool> visitados;
    float peso=0;
    vector<T> m;
    GrafoDirigido<int> aux = this->mapear(&m); 
    for(i=0;i<this->nVertices;i++){
        visitados.emplace_back(false);
    }

    for(i=0;i<this->nVertices;i++){
        peso = 0;
        nVisitados=1;
        for(j=0;j<this->nVertices;j++){
            visitados.at(j) = false;
        }
        visitados.at(i) = true;
        camAux.clear();
        camAux.push_back(i);
        aux.hamiltonianos(i,&visitados, &nVisitados, &peso, &result, &camAux);

    }
    //DESMAPEAR CAMINOS
    while(!result.empty()){
        cam.clear();
        camAux = result.front();
        while(!camAux.empty()){
            cam.push_back(m.at(camAux.front()));
            camAux.pop_front();
        }
        //AGREGAR CAMINO DESMAPEADO
        hamiltonianos.push_back(cam);
        result.pop_front();
    }
    return hamiltonianos;
}
template <typename T>
list<T> GrafoDirigido<T>::caminoHamiltonianoMinimo(){
    int i,j, nVisitados = 1;
    list<int> resultAux;
    list<T> result;
    list<int> camAux;
    list<T> cam;
    vector<bool> visitados;
    float peso=0, menor=0;
    bool prim = true;
    vector<T> m;
    GrafoDirigido<int> aux = this->mapear(&m); 
    for(i=0;i<this->nVertices;i++){
        visitados.emplace_back(false);
    }

    for(i=0;i<this->nVertices;i++){
        peso = 0;
        nVisitados=1;
        for(j=0;j<this->nVertices;j++){
            visitados.at(j) = false;
        }
        visitados.at(i) = true;
        camAux.clear();
        camAux.push_back(i);
        aux.hamiltonianoMinimo(i,&visitados, &nVisitados, &peso,&menor, &resultAux, &camAux, &prim);

    }
    //DESMAPEAR CAMINO
    while(!resultAux.empty()){
        result.push_back(m.at(resultAux.front()));
        resultAux.pop_front();
    }
    return result;
}
template<typename T>
list<list<T>> GrafoDirigido<T>::ciclosHamiltonianos(){
    int i,j, nVisitados = 1;
    list<list<int>> result;
    list<list<T>> hamiltonianos;
    list<int> camAux;
    list<T> cam;
    vector<bool> visitados;
    float peso=0;
    vector<T> m;
    GrafoDirigido<int> aux = this->mapear(&m); 
    for(i=0;i<this->nVertices;i++){
        visitados.emplace_back(false);
    }

    for(i=0;i<this->nVertices;i++){
        peso = 0;
        nVisitados=1;
        for(j=0;j<this->nVertices;j++){
            visitados.at(j) = false;
        }
        camAux.clear();
        cam.clear();
        camAux.push_back(i);
        aux.cHamiltonianos(i, i, &visitados, &nVisitados, &peso, &result, &camAux);

    }
    //DESMAPEAR CAMINOS
    while(!result.empty()){
        cam.clear();
        camAux = result.front();
        while(!camAux.empty()){
            cam.push_back(m.at(camAux.front()));
            camAux.pop_front();
        }
        //AGREGAR CAMINO DESMAPEADO
        hamiltonianos.push_back(cam);
        result.pop_front();
    }
    return hamiltonianos;
}
template<>
void GrafoDirigido<int>::eulerianos(list<int> *cam, int v, list<list<int>> *caminos){
    list<int> sucesores = this->sucesores(v);
    float peso;
    int w;
    while(!sucesores.empty()){
        w = sucesores.front();
        cam->push_back(w);
        peso = this->getPesoArco(v,w);
        this->eliminarArco(v,w);
        if(this->nArcos == 0){
            caminos->push_back(*cam);

        }   
        this->eulerianos(cam, w, caminos);

        this->agregarArco(v,w,peso);
        cam->pop_back();
        sucesores.pop_front();
    }
}

template<>
list<list<int>> GrafoDirigido<int>::caminosEulerianos(){
    list<int> caminoAux;
    list<list<int>> caminos;
    vector<int> in, out;
    int v=-1;
    this->contarGrados(&in, &out);
    if(this->existeEuleriano(in, out, &v)){
        for(int i=0; i<this->nVertices;i++){
            caminoAux.push_back(i);
            this->eulerianos(&caminoAux,i, &caminos);
            caminoAux.clear();
        }
    }
    return caminos;
}


template<typename T>
list<list<T>> GrafoDirigido<T>::getCaminosEulerianos(){
    vector<T> m;
    GrafoDirigido<int> grafoM = this->mapear(&m);
    list<list<int>> resultM = grafoM.caminosEulerianos();
    list<list<T>> result;
    list<int> aux;
    list<T> cam;
    //DESMAPEAR CAMINOS EULERIANOS
    while(!resultM.empty()){
        aux=resultM.front();
        while(!aux.empty()){
            cam.push_back(m.at(aux.front()));
            aux.pop_front();
        }
        result.push_back(cam);
        cam.clear();
        resultM.pop_front();
    }
    return result;
}

template<typename T>
T min(list<T> L){
    int i;
    T menor = L.front();
    while(!L.empty()){
        if(L.front() < menor){
            menor = L.front();
        }
        L.pop_front();
    }
    return menor;
}

template<typename T>
vector<T> GrafoDirigido<T>::DFSMenor(T inicio){
    vector<T> recorrido;
    list<T> sucesores;
    T v = inicio;
    int i;
    recorrido.emplace_back(v);
    for (i=1;i<this->getNVertices();i++){
        sucesores = this->sucesores(v);
        v = min(sucesores);
        recorrido.emplace_back(v);
    }

    return recorrido;

}
template<typename T>
vector<T> ordenar(vector<T> v, int dim){
    GrafoDirigido<T> g;
    g.construir();
    int i,j, indiceMenor;
    T menor;
    for (i=0;i<dim;i++){
        g.agregarVertice(v.at(i));
    }
    for(i=0;i<dim;i++){
        if((i==0) || (v.at(i)<menor)){
            indiceMenor=i;
            menor = v.at(i);
        }
        for(j=0;j<dim;j++){
            if(v.at(i)<=v.at(j)){
                g.agregarArco(v.at(i),v.at(j));
            }
        }
    }
    T inicio = v.at(indiceMenor);
    vector<T> L = g.DFSMenor(inicio);
    return L;
}
#endif