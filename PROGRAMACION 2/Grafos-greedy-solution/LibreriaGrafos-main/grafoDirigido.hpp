#ifndef GRAFODDIRIGIDO_H_
#define GRAFODDIRIGIDO_H_

#include <list>
#include <vector>
#include "grafo.hpp"
using namespace std;

template <typename Tipo>
class GrafoNoDirigido;

template <typename Tipo>
class GrafoDirigido: public Grafo<Tipo>{
    public:
        void copiar(Grafo<Tipo> *A);
        GrafoDirigido<int> mapear(vector<Tipo> *mapeo);
        GrafoNoDirigido<Tipo> convertirEnNoDirigido();
        //Tipos
        bool esSumidero(Tipo v);
        bool esFuente(Tipo v);
        vector<Tipo> DFSMenor(Tipo inicio);

        //RECORRIDOS
        list<Tipo> listaBFS(Tipo inicio);
        list<Tipo> listaDFS(Tipo inicio);


        int NComponentesFConexas(); //NO LISTO
        list<Tipo> caminoMenor(Tipo v, Tipo w, float *peso = nullptr);
        list<Tipo> caminoMenorConBloqueo(Tipo v, Tipo w, list<Tipo> bloqueados, float *peso = nullptr);
        list<Tipo> caminoMenorConRequisito(Tipo v, Tipo w, Tipo H, float *peso = nullptr);
        list<Tipo> caminoMayor(Tipo v, Tipo w);
        list<list<Tipo>> caminosHamiltonianos();
        list<Tipo> caminoHamiltonianoMinimo();
        list<list<Tipo>> ciclosHamiltonianos();
        list<list<int>> caminosEulerianos();
        void eulerianos(list<int> *cam, int v, list<list<int>> *caminos);
        list<list<Tipo>> getCaminosEulerianos();

};

template<typename Tipo>
void GrafoDirigido<Tipo>::copiar(Grafo<Tipo> *A){
    Vertice<Tipo> *aux;
    Arco<Tipo> *arco;
    float pesoAux;
    aux = A->getPrimero();
    while(aux != nullptr){
        this->agregarVertice(aux->getInfo());
        aux = aux->getSig();
    }
    aux = A->getPrimero();
    while(aux != nullptr){
        arco = aux->getArcos();
        while(arco != nullptr){
            pesoAux = arco->getPeso();
            this->agregarArco(aux->getInfo(), arco->getInfo()->getInfo(), pesoAux);
            arco = arco->getSig();
        }
        aux = aux->getSig();
    }
}


template<typename Tipo>
GrafoDirigido<int> GrafoDirigido<Tipo>::mapear(vector<Tipo> *mapeo){
    Vertice<Tipo> *actual = this->primero;
    int i=0, dim=this->getNVertices();
    GrafoDirigido<int> grafo;
    Arco<Tipo> *arco;
    grafo.construir();
    //MAPEAR Y AGREGAR VERTICES AL GRAFO MAPEADO
    while(actual!=nullptr){
        mapeo->emplace_back(actual->getInfo());
        grafo.agregarVertice(i);
        actual=actual->getSig();
        i++;
    }

    //AGREGAR ARCOS AL GRAFO MAPEADO
    actual = this->primero;
    i=0;
    while(actual != nullptr){
        arco = actual->getArcos();
        while(arco != nullptr){
            int v = this->buscarMapeo(*mapeo, arco->getInfo()->getInfo(), dim);
            grafo.agregarArco(i,v, arco->getPeso());
            arco=arco->getSig();
        }
        actual=actual->getSig();
        i++;
    }

    return grafo;

}
template<typename Tipo>
GrafoNoDirigido<Tipo> GrafoDirigido<Tipo>::convertirEnNoDirigido(){
    GrafoNoDirigido<Tipo> C;
    C.construir();
    C.copiar(this); 
    return C;
}
template <typename Tipo>
bool GrafoDirigido<Tipo>::esSumidero(Tipo v){
    list<Tipo> L = this->sucesores(v);
    return L.empty();
}
template <typename Tipo>
bool GrafoDirigido<Tipo>::esFuente(Tipo v){
    list<Tipo> L = this->predecesores(v);
    return L.empty();
}


//LISTAS DE RECORRIDOS ===============================
template<typename Tipo>
list<Tipo> GrafoDirigido<Tipo>::listaBFS(Tipo inicio){
    list<Tipo> recorrido;
    list<int> recorridoAux;
    vector<Tipo> mapeo;
    vector<bool> visitados;
    GrafoDirigido<int> aux = this->mapear(&mapeo);
    int v = this->buscarMapeo(mapeo, inicio, this->getNVertices());
    int i;
    for(i=0;i<this->getNVertices();i++){
        visitados.emplace_back(false);
    }
    visitados.at(v) = true;
    aux.BFS(v, &visitados, &recorridoAux);

    while(!recorridoAux.empty()){
        recorrido.push_back(mapeo.at(recorridoAux.front()));
        recorridoAux.pop_front();
    }
    return recorrido;

}
template<typename Tipo>
list<Tipo> GrafoDirigido<Tipo>::listaDFS(Tipo inicio){
    list<Tipo> recorrido;
    list<int> recorridoAux;
    vector<Tipo> mapeo;
    vector<bool> visitados;
    GrafoDirigido<int> aux = this->mapear(&mapeo);
    int v = this->buscarMapeo(mapeo, inicio, this->getNVertices());
    int i;
    for(i=0;i<this->getNVertices();i++){
        visitados.emplace_back(false);
    }
    visitados.at(v) = true;
    aux.DFS(v, &visitados, &recorridoAux);

    while(!recorridoAux.empty()){
        recorrido.push_back(mapeo.at(recorridoAux.front()));
        recorridoAux.pop_front();
    }
    return recorrido;

}

template<>
int GrafoDirigido<int>::NComponentesFConexas(){
    int nComponentes=1, i=0,j;
    vector<bool> visitados;
    for(i=0;i<this->getNVertices();i++){
        visitados.emplace_back(false);
    }
    for (j=0;j<this->getNVertices(); j++){
        cout<<j<<endl;
        //Inicializar vector de visitados
        if(j>0){
            for(i=0;i<this->getNVertices();i++){
                visitados.at(i) = false;
            }
        }

        this->BFS(j,&visitados);

        for(i=0;i<this->getNVertices();i++){
            if (!visitados.at(i)){
                this->BFS(i,&visitados);
                cout << i << " No habia sido visitado"<<endl;
                nComponentes++;
            }
        }
    }
    return nComponentes;
}

template <typename Tipo>
list<Tipo> GrafoDirigido<Tipo>::caminoMenor(Tipo v, Tipo w, float *peso){
    vector<Tipo> mapeo;
    GrafoDirigido<int> aux = this->mapear(&mapeo);
    int inicio = this->buscarMapeo(mapeo, v, this->getNVertices()), fin = this->buscarMapeo(mapeo, w, this->getNVertices());
    list<int> camino = aux.caminoDijkstra(inicio, fin, peso);

    list<Tipo> resultado;
    while(!camino.empty()){
        resultado.push_back(mapeo[camino.front()]);
        camino.pop_front();
    }
    return resultado;
}

template <typename Tipo>
list<Tipo> GrafoDirigido<Tipo>::caminoMenorConBloqueo(Tipo v, Tipo w, list<Tipo> bloqueados, float *peso){ 
    vector<Tipo> mapeo;
    GrafoDirigido<int> aux = this->mapear(&mapeo);
    int inicio = this->buscarMapeo(mapeo, v, this->getNVertices()), fin = this->buscarMapeo(mapeo, w, this->getNVertices()), i;
    list<int> camino;
    vector<bool> bloqueos;
    for (i=0;i<this->nVertices;i++){
        bloqueos.emplace_back(false);
    }
    while(!bloqueados.empty()){
        bloqueos.at(this->buscarMapeo(mapeo, bloqueados.front(), this->getNVertices())) = true;
        bloqueados.pop_front();
    }
    camino = aux.caminoObstaculos(inicio, fin, bloqueos, peso);

    list<Tipo> resultado;
    while(!camino.empty()){
        resultado.push_back(mapeo[camino.front()]);
        camino.pop_front();
    }
    return resultado;
}
template <typename Tipo>
list<Tipo> GrafoDirigido<Tipo>::caminoMenorConRequisito(Tipo v, Tipo w, Tipo H, float *peso){ 
    list<Tipo> camino1, camino2;
    list<Tipo> bloqueos, bloqueosCam1;
    list<Tipo> resultado;
    float p1,p2;
    camino1 = this->caminoMenor(v, H, &p1);
    if(!camino1.empty()){
        //HACER CAMINO DEL REQUISITO AL DESTINO PERO SIN PASAR POR LOS VERTICES YA UTILIZADOS POR EL CAMINO DEL INICIO AL REQUISITO
        bloqueos = camino1;
        bloqueos.pop_back();
        camino2 = this->caminoMenorConBloqueo(H,w,bloqueos, &p2);
        //SI NO SE ENCUENTRA UN POSIBLE CAMINO, SE PONE COMO RESTRICCION DEL PRIMER CAMINO EL ULTIMO VERTICE ANTES DE LLEGAR AL REQUISITO
        while(!camino1.empty() && camino2.empty()){
            bloqueosCam1.push_back(bloqueos.back());
            if(bloqueos.back() != v){
                camino1 = this->caminoMenorConBloqueo(v, H, bloqueosCam1, &p1); //REBUSCAR CAMINO DE INICIO A REQUISITO AHORA CON VERTICES BLOQUEADOS
                if(!camino1.empty()){
                    bloqueos = camino1;
                    bloqueos.pop_back();
                    //REBUSCAR CAMINO DE REQUISITO AL VERTICE FINAL CON LOS VERTICES DEL CAMINO 1 BLOQUEADOS
                    camino2 = this->caminoMenorConBloqueo(H,w,bloqueos, &p2); 
                }
            }else{ 
                camino1.clear();
            }
        }
    }
    //SI EL CAMINO 1 NO ESTA VACIO, QUERE DECIR QUE SE ENCONTRO UN CAMINO VALIDO
    if(!camino1.empty()){
        *peso = p1 + p2;
        while(!camino1.empty()){
            resultado.push_back(camino1.front());
            camino1.pop_front();
        }
        camino2.pop_front(); //Eliminar requisito del camino2 para evitar repetirlo en la lista del camino resultante
        while(!camino2.empty()){
            resultado.push_back(camino2.front());
            camino2.pop_front();
        }
    }
    return resultado;
}
template <typename Tipo>
list<Tipo> GrafoDirigido<Tipo>::caminoMayor(Tipo v, Tipo w){
    vector<Tipo> mapeo;
    GrafoDirigido<int> aux = this->mapear(&mapeo);
    int inicio = this->buscarMapeo(mapeo, v, this->getNVertices()), fin = this->buscarMapeo(mapeo, w, this->getNVertices());
    list<int> camino, caminoMayor;
    vector<bool> visitados;
    float peso=0, pesoMayor = -1;
    bool primero = true;

    int i;
    for(i = 0; i<this->getNVertices(); i++){
        visitados.emplace_back(false);
    }

    camino.push_back(inicio);
    aux.mayorCamino(inicio, fin, peso, &visitados, &camino, &pesoMayor, &caminoMayor, &primero);

    list<Tipo> resultado;
    while(!caminoMayor.empty()){
        resultado.push_back(mapeo[caminoMayor.front()]);
        caminoMayor.pop_front();
    }
    return resultado;
}
template <typename Tipo>
list<list<Tipo>> GrafoDirigido<Tipo>::caminosHamiltonianos(){
    int i,j, nVisitados = 1;
    list<list<int>> result;
    list<list<Tipo>> hamiltonianos;
    list<int> camAux;
    list<Tipo> cam;
    vector<bool> visitados;
    float peso=0;
    vector<Tipo> m;
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
template <typename Tipo>
list<Tipo> GrafoDirigido<Tipo>::caminoHamiltonianoMinimo(){
    int i,j, nVisitados = 1;
    list<int> resultAux;
    list<Tipo> result;
    list<int> camAux;
    list<Tipo> cam;
    vector<bool> visitados;
    float peso=0, menor=0;
    bool prim = true;
    vector<Tipo> m;
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
template<typename Tipo>
list<list<Tipo>> GrafoDirigido<Tipo>::ciclosHamiltonianos(){
    int i,j, nVisitados = 1;
    list<list<int>> result;
    list<list<Tipo>> hamiltonianos;
    list<int> camAux;
    list<Tipo> cam;
    vector<bool> visitados;
    float peso=0;
    vector<Tipo> m;
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


template<typename Tipo>
list<list<Tipo>> GrafoDirigido<Tipo>::getCaminosEulerianos(){
    vector<Tipo> m;
    GrafoDirigido<int> grafoM = this->mapear(&m);
    list<list<int>> resultM = grafoM.caminosEulerianos();
    list<list<Tipo>> result;
    list<int> aux;
    list<Tipo> cam;
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

template<typename Tipo>
Tipo min(list<Tipo> L){
    int i;
    Tipo menor = L.front();
    while(!L.empty()){
        if(L.front() < menor){
            menor = L.front();
        }
        L.pop_front();
    }
    return menor;
}

template<typename Tipo>
vector<Tipo> GrafoDirigido<Tipo>::DFSMenor(Tipo inicio){
    vector<Tipo> recorrido;
    list<Tipo> sucesores;
    Tipo v = inicio;
    int i;
    recorrido.emplace_back(v);
    for (i=1;i<this->getNVertices();i++){
        sucesores = this->sucesores(v);
        v = min(sucesores);
        recorrido.emplace_back(v);
    }

    return recorrido;

}
template<typename Tipo>
vector<Tipo> ordenar(vector<Tipo> v, int dim){
    GrafoDirigido<Tipo> g;
    g.construir();
    int i,j, indiceMenor;
    Tipo menor;
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
    Tipo inicio = v.at(indiceMenor);
    vector<Tipo> L = g.DFSMenor(inicio);
    return L;
}
#endif