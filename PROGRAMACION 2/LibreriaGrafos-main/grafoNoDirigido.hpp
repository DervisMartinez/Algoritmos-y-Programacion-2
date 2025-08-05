#ifndef GRAFONODIRIGIDO_H_
#define GRAFONODIRIGIDO_H_

#include <iostream>
#include <list>
#include <vector>
#include "grafo.hpp"
using namespace std;

template <typename T>
class GrafoNoDirigido: public Grafo<T>{
    public:
        void copiar(Grafo<T> *A);
        list<T> vecinos(T v) const;
        list<T> vecinos(Vertice<T>* v) const;  
        //ARCOS
        void modificarPesoArcoND(T v, T w, float nuevo);
        void agregarArcoND(T v, T w, float peso = 0); 
        void eliminarArcoND(T v, T w);
        

        GrafoNoDirigido<int> mapear(vector<T> *mapeo);
        //RECORRIDOS
        list<T> listaBFS(T inicio);
        list<T> listaDFS(T inicio);

        //COMPONENTES CONEXAS (SOLO GRAFOS MAPEADOS)
        int NComponentes(); 

        list<T> caminoMenor(T v, T w, float *peso = NULL);
        list<T> caminoMenorConBloqueo(T v, T w, list<T> bloqueados, float *peso = NULL);
        list<T> caminoMenorConRequisito(T v, T w, T H, float *peso = NULL); 
        list<T> caminoMayor(T v, T w);
        void arbolExpandidoMinimo(GrafoNoDirigido<int> *g, float *peso);
        void arcoMinimo(list<int> activos, int *v, int *w, float *peso, vector<bool> visitados, bool *band);
        list<list<T>> puentes();
        bool esConexo();
        list<list<T>> caminosHamiltonianos();
        list<T> caminoHamiltonianoMinimo();
        list<list<T>> ciclosHamiltonianos();
        list<T> cicloHamiltonianoMinimo();
        void eulerianos(list<int> *cam, int v, list<list<int>> *caminos);
        list<list<T>> getCaminosEulerianos();

        bool esBipartito();
        void esBipartito(int inicio, bool *resultado);
        bool esMulticoloreable(int nColores);
        void esMulticoloreable(int inicio, int nColoresGrafo, bool *resultado);

        list<T> puntosArticulacion();
        //PARA GRAFOS QUE YA ESTEN MAPEADOS
        list<list<int>> caminosEulerianos();
        bool esConexoM();
        list<list<T>> puentesM();
};



template<typename T>
void GrafoNoDirigido<T>::copiar(Grafo<T> *A){
    Vertice<T> *aux;
    Arco<T> *arco;
    float pesoAux;
    aux = A->getPrimero();
    while(aux != NULL){
        this->agregarVertice(aux->getInfo());
        aux = aux->getSig();
    }
    aux = A->getPrimero();
    while(aux != NULL){
        arco = aux->getArcos();
        while(arco != NULL){
            pesoAux = arco->getPeso();
            this->agregarArcoND(aux->getInfo(), arco->getInfo()->getInfo(), pesoAux);
            arco = arco->getSig();
        }
        aux = aux->getSig();
    }
}


template<typename T>
list<T> GrafoNoDirigido<T>::vecinos(Vertice<T> *v) const{
    list<T> lista;

    Arco<T> *arco;
    if(v!=NULL){
        arco = v->getArcos();

        while (arco!=NULL){
            lista.push_back(arco->getInfo()->getInfo());
            arco = arco->getSig();
        }
    }

    return lista;            
}
template<typename T>
list<T> GrafoNoDirigido<T>::vecinos(T v) const{
    return this->vecinos(this->getVertice(v));            
}
//OPERACIONES CON ARCOS ============================================================

template <typename T>
void GrafoNoDirigido<T>::modificarPesoArcoND(T v, T w, float nuevo){
    this->modificarPesoArco(v,w,nuevo);
    this->modificarPesoArco(w,v,nuevo);
}

template <typename T>
void GrafoNoDirigido<T>::agregarArcoND(T v, T w, float peso){
    this->agregarArco(v,w,peso);
    this->agregarArco(w,v,peso);
}

template <typename T>
void GrafoNoDirigido<T>::eliminarArcoND(T v, T w){
    this->eliminarArco(v,w);
    this->eliminarArco(w,v);
}


//OPERACIONES CON VERTICES ================================================================



template<typename T>
GrafoNoDirigido<int> GrafoNoDirigido<T>::mapear(vector<T> *mapeo){
    Vertice<T> *actual = this->primero;
    int i=0, dim=this->getNVertices();
    GrafoNoDirigido<int> grafo;
    Arco<T> *arco;
    grafo.construir();
    //MAPEAR Y AGREGAR VERTICES AL GRAFO MAPEADO
    while(actual!=NULL){
        mapeo->emplace_back(actual->getInfo());
        grafo.agregarVertice(i);
        actual=actual->getSig();
        i++;
    }

    //AGREGAR ARCOS AL GRAFO MAPEADO
    actual = this->primero;
    i=0;
    while(actual != NULL){
        arco = actual->getArcos();
        while(arco != NULL){
            int v = this->buscarMapeo(*mapeo, arco->getInfo()->getInfo(), dim);
            grafo.agregarArcoND(i,v, arco->getPeso());
            arco=arco->getSig();
        }
        actual=actual->getSig();
        i++;
    }

    return grafo;

}

//LISTAS DE RECORRIDOS ===============================
template<typename T>
list<T> GrafoNoDirigido<T>::listaBFS(T inicio){
    list<T> recorrido;
    list<int> recorridoAux;
    vector<T> mapeo;
    vector<bool> visitados;
    GrafoNoDirigido<int> aux = this->mapear(&mapeo);
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
template<typename T>
list<T> GrafoNoDirigido<T>::listaDFS(T inicio){
    list<T> recorrido;
    list<int> recorridoAux;
    vector<T> mapeo;
    vector<bool> visitados;
    GrafoNoDirigido<int> aux = this->mapear(&mapeo);
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
//COMPONENTES CONEXAS
template<>
int GrafoNoDirigido<int>::NComponentes(){
    int nComponentes=1, i=0;
    vector<bool> visitados;
    //Inicializar vector de visitados
    for(i=0;i<this->getNVertices();i++){
        visitados.emplace_back(false);
    }

    this->BFS(0,&visitados);

    for(i=0;i<this->getNVertices();i++){
        if (!visitados.at(i)){
            this->BFS(i,&visitados);
            cout << i << " No habia sido visitado"<<endl;
            nComponentes++;
        }
    }
    return nComponentes;
}
template <typename T>
list<T> GrafoNoDirigido<T>::caminoMenor(T v, T w, float *peso){
    vector<T> mapeo;
    GrafoNoDirigido<int> aux = this->mapear(&mapeo);
    int inicio = this->buscarMapeo(mapeo, v, this->getNVertices()), fin = this->buscarMapeo(mapeo, w, this->getNVertices());
    list<int> camino = aux.caminoDijkstra(inicio, fin, peso);

    list<T> resultado;
    while(!camino.empty()){
        resultado.push_back(mapeo[camino.front()]);
        camino.pop_front();
    }
    return resultado;
}
template <typename T>
list<T> GrafoNoDirigido<T>::caminoMenorConBloqueo(T v, T w, list<T> bloqueados, float *peso){ 
    vector<T> mapeo;
    GrafoNoDirigido<int> aux = this->mapear(&mapeo);
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

    list<T> resultado;
    while(!camino.empty()){
        resultado.push_back(mapeo[camino.front()]);
        camino.pop_front();
    }
    return resultado;
}
template <typename T>
list<T> GrafoNoDirigido<T>::caminoMenorConRequisito(T v, T w, T H, float *peso){ 
    list<T> camino1, camino2;
    list<T> bloqueos, bloqueosCam1;
    list<T> resultado;
    float p1,p2;
    camino1 = this->caminoMenor(v, H, &p1);
    if(!camino1.empty()){
        bloqueos = camino1;
        bloqueos.pop_back();
        camino2 = this->caminoMenorConBloqueo(H,w,bloqueos, &p2);
        while(!camino1.empty() && camino2.empty()){
            bloqueosCam1.push_back(bloqueos.back());
            if (bloqueos.back() != v){
                camino1 = this->caminoMenorConBloqueo(v, H, bloqueosCam1, &p1); 
                if(!camino1.empty()){
                    bloqueos = camino1;
                    bloqueos.pop_back();
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
template <typename T>
list<T> GrafoNoDirigido<T>::caminoMayor(T v, T w){
    vector<T> mapeo;
    GrafoNoDirigido<int> aux = this->mapear(&mapeo);
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

    list<T> resultado;
    while(!caminoMayor.empty()){
        resultado.push_back(mapeo[caminoMayor.front()]);
        caminoMayor.pop_front();
    }
    return resultado;
}

template<typename T>
void GrafoNoDirigido<T>::arbolExpandidoMinimo(GrafoNoDirigido<int> *g, float *peso){
    vector<bool> visitados;
    int i,v,w;
    bool fin, band=false;
    list<int> vertices, activos;
    float p;

    for(i=0;i<this->getNVertices();i++){
        g->agregarVertice(i);
        visitados.emplace_back(false);
    }

    activos.push_back(0);
    visitados.at(0) = true;
    fin = false;
    p=0;
    *peso=0;
    while(!fin){
        this->arcoMinimo(activos,&v,&w,&p,visitados, &band);
        g->agregarArcoND(v,w, p);
        *peso = *peso + p;
        activos.push_back(w);
        visitados.at(w) = true;

        fin = true;
        if(!band){
            for ( i = 0; i< this->getNVertices(); i++){
                fin = fin && visitados.at(i);
            }
        }else{
            *peso = 0;
        }
    }

}
template<typename T>
void GrafoNoDirigido<T>::arcoMinimo(list<int> activos, int *v, int *w, float *peso, vector<bool> visitados, bool *band){
    int actual, act;
    list<int> vecinos;
    float pesoArco;
    bool prim=false;

    while(!activos.empty()){
        actual = activos.front();
        vecinos = this->vecinos(actual);
        while(!vecinos.empty()){
            act = vecinos.front();
            if (!visitados.at(act)){
                pesoArco = this->getPesoArco(actual, act);
                if((pesoArco<*peso) || (!prim)){
                    *peso = pesoArco;
                    *v = actual;
                    *w = act;
                    prim=true;
                }
            }
            vecinos.pop_front();
        }
        activos.pop_front();
    }
    //Si no se pudo procesar ni un solo arco, se activa la bandera band para finalizar el proceso
    if(!prim){
        *band = true;
    }
}
template <typename T>
bool GrafoNoDirigido<T>::esConexo(){ //METODO PARA GRAFOS YA MAPEADOS
    vector<T> mapeo;
    vector<bool> visitados;
    int i;
    //MAPEAR GRAFO
    GrafoNoDirigido<int> g = this->mapear(&mapeo);
    //INICIALIZAR VECTOR DE VISITADOS
    for(i=0;i<this->nVertices;i++){
        visitados.emplace_back(false);
    }
    //REALIZAR RECORRIDO BFS
    g.BFS(0, &visitados);
    //VERIFICAR QUE SE HAYAN VISITADOS TODOS LOS VERTICES
    for(i=0;i<this->nVertices;i++){
        if(!visitados.at(i)) return false;
    }

    return true;
}
template <typename T>
bool GrafoNoDirigido<T>::esConexoM(){
    vector<bool> visitados;
    int i;
    //INICIALIZAR VECTOR DE VISITADOS
    for(i=0;i<this->nVertices;i++){
        visitados.emplace_back(false);
    }
    //REALIZAR RECORRIDO BFS
    this->BFS(0, &visitados);
    //VERIFICAR QUE SE HAYAN VISITADOS TODOS LOS VERTICES
    for(i=0;i<this->nVertices;i++){
        if(!visitados.at(i)) return false;
    }

    return true;
}
template<typename T>
list<list<T>> GrafoNoDirigido<T>::puentes(){
    list<list<T>> arcosPuente;
    list<T> arco;
    list<int> vecinos;
    float peso;
    int i,w;
    vector<T> m;
    GrafoNoDirigido<int> aux = this->mapear(&m);//MAPEAR GRAFO O(N+M)

    for (i=0; i<this->nVertices; i++){ //RECORRER VERTICES 
        vecinos = aux.vecinos(i);
        //RECORRER ARCOS
        while(!vecinos.empty()){
            w = vecinos.front();
            peso = aux.getPesoArco(i,w);
            //ELIMINAR ARCO
            aux.eliminarArcoND(i,w);
            //VERIFICAR QUE EL GRAFO SIGA CONEXO (Si no es conexo, el arco es un arco puente)
            if((!aux.esConexoM()) && (i<w)){
                //Agregar arco a la lista de arcos puente
                arco.clear();
                arco.push_back(m.at(i));
                arco.push_back(m.at(w));
                arcosPuente.push_back(arco);
            }
            //VOLVER A AGREGAR EL ARCO ELIMINADO
            aux.agregarArcoND(i,w,peso);
            vecinos.pop_front();
        }

    }
    return arcosPuente;
}
template<typename T>
list<list<T>> GrafoNoDirigido<T>::puentesM(){
    list<list<T>> arcosPuente;
    list<T> arco;
    list<int> vecinos;
    float peso;
    int i,w;

    for (i=0; i<this->nVertices; i++){ 
        vecinos = this->vecinos(i);
     
        while(!vecinos.empty()){
            w = vecinos.front();
            peso = this->getPesoArco(i,w);
       
            this->eliminarArcoND(i,w);
          
            if((!this->esConexoM()) && (i<w)){
        
                arco.clear();
                arco.push_back(i);
                arco.push_back(w);
                arcosPuente.push_back(arco);
            }
            this->agregarArcoND(i,w, peso);
            vecinos.pop_front();
        }

    }
    return arcosPuente;
}



template<typename T>
list<T> GrafoNoDirigido<T>::puntosArticulacion(){
    list<T> puntos, vertices = this->vertices();
    list<T> vecinos;
    list<float> pesos;
    T actual;

    //RECORRER VERTICE A VERTICE
    while(!vertices.empty()){
        actual = vertices.front();
        vecinos = this->vecinos(actual); //GUARDAR ARCOS
        while(!vecinos.empty()){ //GUARDAR PESOS DE CADA ARCO
            pesos.push_back(this->getPesoArco(actual, vecinos.front()));
            vecinos.pop_front();
        }
        vecinos = this->vecinos(actual); //GUARDAR ARCOS
        
        this->eliminarVertice(actual); //ELIMINAR VERTICE
        if(!this->esConexo()){
            puntos.push_back(actual);
        }
        this->agregarVertice(actual); //VOLVER A AGREGAR EL VERTICE
        //VOLVER A CREAR TODOS SUS ARCOS
        while(!vecinos.empty()){
            this->agregarArcoND(actual, vecinos.front(), pesos.front());
            pesos.pop_front();
            vecinos.pop_front();
        }
        vertices.pop_front();
    }

    return puntos;
}

template <typename T>
list<list<T>> GrafoNoDirigido<T>::caminosHamiltonianos(){
    int i,j, nVisitados = 1;
    list<list<int>> result;
    list<list<T>> hamiltonianos;
    list<int> camAux;
    list<T> cam;
    vector<bool> visitados;
    float peso=0;
    vector<T> m;
    GrafoNoDirigido<int> aux = this->mapear(&m); 
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
        cam.clear();
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
list<T> GrafoNoDirigido<T>::caminoHamiltonianoMinimo(){
    int i,j, nVisitados = 1;
    list<int> resultAux;
    list<T> result;
    list<int> camAux;
    list<T> cam;
    vector<bool> visitados;
    float peso=0, menor=0;
    bool prim = true;
    vector<T> m;
    GrafoNoDirigido<int> aux = this->mapear(&m); 
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

template <typename T>
list<list<T>> GrafoNoDirigido<T>::ciclosHamiltonianos(){
    int i,j, nVisitados = 1;
    list<list<int>> result;
    list<list<T>> hamiltonianos;
    list<int> camAux;
    list<T> cam;
    vector<bool> visitados;
    float peso=0;
    vector<T> m;
    GrafoNoDirigido<int> aux = this->mapear(&m); 
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

template <typename T>
list<T> GrafoNoDirigido<T>::cicloHamiltonianoMinimo(){
    int i,j, nVisitados = 1;
    list<int> resultAux;
    list<T> result;
    list<int> camAux;
    list<T> cam;
    vector<bool> visitados;
    float peso=0, menor=0;
    bool prim = true;
    vector<T> m;
    GrafoNoDirigido<int> aux = this->mapear(&m); 
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
        camAux.push_back(i);
        aux.chamiltonianoMinimo(i, i, &visitados, &nVisitados, &peso,&menor, &resultAux, &camAux, &prim);

    }
    //DESMAPEAR CAMINO
    while(!resultAux.empty()){
        result.push_back(m.at(resultAux.front()));
        resultAux.pop_front();
    }
    return result;
}

template<>
void GrafoNoDirigido<int>::eulerianos(list<int> *cam, int v, list<list<int>> *caminos){
    list<int> vecinos = this->vecinos(v);
    float peso;
    int w;
    while(!vecinos.empty()){
        w = vecinos.front();
        cam->push_back(w);
        peso = this->getPesoArco(v,w);
        this->eliminarArcoND(v,w);
        if(this->nArcos == 0){
            caminos->push_back(*cam);
        }   
        this->eulerianos(cam, w, caminos);

        this->agregarArcoND(v,w,peso);
        cam->pop_back();
        vecinos.pop_front();
    }
}
template<>
list<list<int>> GrafoNoDirigido<int>::caminosEulerianos(){
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
list<list<T>> GrafoNoDirigido<T>::getCaminosEulerianos(){
    vector<T> m;
    GrafoNoDirigido<int> grafoM = this->mapear(&m);
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
bool GrafoNoDirigido<T>::esBipartito(){
    vector<T> mapeo;
    bool bicoloreable = true;
    GrafoNoDirigido<int> aux = this->mapear(&mapeo);
    int fuente = this->buscarMapeo(mapeo, this->primero->getInfo(), this->getNVertices());
    aux.esBipartito(fuente,&bicoloreable);
    return bicoloreable;
}

template<typename T>
void GrafoNoDirigido<T>::esBipartito(int inicio, bool *resultado){
    vector<int> colores;
    int i;
    queue<int> c;
    list<int> sucesores;
    int v,w;

    for(i=0;i<this->getNVertices();i++){
        colores.emplace_back(-1);
    }
    *resultado = true;
    colores.at(inicio) = 0;
    c.push(inicio);
    while(!c.empty() && *resultado){
        v = c.front();
        c.pop();

        sucesores = this->vecinos(v);
        while (!sucesores.empty() && *resultado){
        w = sucesores.front();
        if(colores[w] == -1){
            colores[w] = (colores[v] + 1) % 2;
            c.push(w);
        }
        if(colores[w] == colores[v]){
            *resultado = false;
            return;
        }
        sucesores.pop_front();
        }
    } 
    
    return;
}

template<typename T>
bool GrafoNoDirigido<T>::esMulticoloreable(int nColores){
    vector<T> map;
    bool multicoloreable = true;
    GrafoNoDirigido<int> aux = this->mapear(&map);
    int fuente = this->buscarMapeo(map,this->primero->getInfo(), this->getNVertices());
    aux.esMulticoloreable(fuente,nColores,&multicoloreable);
    return multicoloreable;
}

template<typename T>
void GrafoNoDirigido<T>::esMulticoloreable(int inicio, int nColoresGrafo, bool *resultado){
  vector<int> colores;
  vector<bool> coloresDisponibles, visitados;
  queue<int> c;
  list<int> sucesores;
  int v,w,i;

    for(i=0;i<this->getNVertices();i++){
        colores.emplace_back(-1);
        coloresDisponibles.emplace_back(true);
        visitados.emplace_back(false);
    }

    bool encontrado = false;
    *resultado = true;
    colores.at(inicio) = 0;
    c.push(inicio);
    while(!c.empty() && *resultado){
        v = c.front();
        c.pop();

        sucesores = this->vecinos(v);
        while (!sucesores.empty() && *resultado)
        {
        w = sucesores.front();
        visitados[w] = true;
        if(colores[w] != -1){
            coloresDisponibles[colores[w]] = false;
        }
        if(!visitados[v]){
            c.push(w);
        }   
        sucesores.pop_front();
        }
        i = 0;
        encontrado = false;
        while (i < nColoresGrafo && !encontrado){
        if(coloresDisponibles[i]){
            colores[w] = i;
            encontrado = true;
        }
        ++i;
        }
        visitados[v] = true;
        if(!encontrado){
        *resultado = false;
        return;
        }
    }
    return;
}


#endif