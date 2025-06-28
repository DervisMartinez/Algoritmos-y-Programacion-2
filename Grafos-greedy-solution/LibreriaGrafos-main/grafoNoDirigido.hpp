#ifndef GRAFONODIRIGIDO_H_
#define GRAFONODIRIGIDO_H_

#include <iostream>
#include <list>
#include <vector>
#include "grafo.hpp"
using namespace std;

template <typename Tipo>
class GrafoNoDirigido: public Grafo<Tipo>{
    public:
        void copiar(Grafo<Tipo> *A);
        list<Tipo> vecinos(Vertice<Tipo> *v); //FUNCIONAL
        list<Tipo> vecinos(Tipo v); //FUNCIONAL

        //ARCOS
        void modificarPesoArcoND(Tipo v, Tipo w, float nuevo); //FUNCIONAL
        void agregarArcoND(Tipo v, Tipo w, float peso = 0); //FUNCIONAL
        void eliminarArcoND(Tipo v, Tipo w); //FUNCIONAL
        //VERTICES

        GrafoNoDirigido<int> mapear(vector<Tipo> *mapeo);
        //RECORRIDOS
        list<Tipo> listaBFS(Tipo inicio);
        list<Tipo> listaDFS(Tipo inicio);

        //COMPONENTES CONEXAS (SOLO GRAFOS MAPEADOS)
        int NComponentes(); //FUNCIONAL

        list<Tipo> caminoMenor(Tipo v, Tipo w, float *peso = nullptr);
        list<Tipo> caminoMenorConBloqueo(Tipo v, Tipo w, list<Tipo> bloqueados, float *peso = nullptr);
        list<Tipo> caminoMenorConRequisito(Tipo v, Tipo w, Tipo H, float *peso = nullptr); 
        list<Tipo> caminoMayor(Tipo v, Tipo w);
        void arbolExpandidoMinimo(GrafoNoDirigido<int> *g, float *peso);
        void arcoMinimo(list<int> activos, int *v, int *w, float *peso, vector<bool> visitados, bool *band);
        list<list<Tipo>> puentes();
        bool esConexo();
        list<list<Tipo>> caminosHamiltonianos();
        list<Tipo> caminoHamiltonianoMinimo();
        list<list<Tipo>> ciclosHamiltonianos();
        list<Tipo> cicloHamiltonianoMinimo();
        void eulerianos(list<int> *cam, int v, list<list<int>> *caminos);
        list<list<Tipo>> getCaminosEulerianos();

        bool esBipartito();
        void esBipartito(int inicio, bool *resultado);
        bool esMulticoloreable(int nColores);
        void esMulticoloreable(int inicio, int nColoresGrafo, bool *resultado);

        list<Tipo> puntosArticulacion();
        //PARA GRAFOS QUE YA ESTEN MAPEADOS
        list<list<int>> caminosEulerianos();
        bool esConexoM();
        list<list<Tipo>> puentesM();
};



template<typename Tipo>
void GrafoNoDirigido<Tipo>::copiar(Grafo<Tipo> *A){
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
            this->agregarArcoND(aux->getInfo(), arco->getInfo()->getInfo(), pesoAux);
            arco = arco->getSig();
        }
        aux = aux->getSig();
    }
}


template<typename Tipo>
list<Tipo> GrafoNoDirigido<Tipo>::vecinos(Vertice<Tipo> *v){
    list<Tipo> lista;

    Arco<Tipo> *arco;
    if(v!=nullptr){
        arco = v->getArcos();

        while (arco!=nullptr){
            lista.push_back(arco->getInfo()->getInfo());
            arco = arco->getSig();
        }
    }

    return lista;            
}
template<typename Tipo>
list<Tipo> GrafoNoDirigido<Tipo>::vecinos(Tipo v){
    return this->vecinos(this->getVertice(v));            
}
//OPERACIONES CON ARCOS ============================================================

template <typename Tipo>
void GrafoNoDirigido<Tipo>::modificarPesoArcoND(Tipo v, Tipo w, float nuevo){
    this->modificarPesoArco(v,w,nuevo);
    this->modificarPesoArco(w,v,nuevo);
}

template <typename Tipo>
void GrafoNoDirigido<Tipo>::agregarArcoND(Tipo v, Tipo w, float peso){
    this->agregarArco(v,w,peso);
    this->agregarArco(w,v,peso);
}

template <typename Tipo>
void GrafoNoDirigido<Tipo>::eliminarArcoND(Tipo v, Tipo w){
    this->eliminarArco(v,w);
    this->eliminarArco(w,v);
}


//OPERACIONES CON VERTICES ================================================================



template<typename Tipo>
GrafoNoDirigido<int> GrafoNoDirigido<Tipo>::mapear(vector<Tipo> *mapeo){
    Vertice<Tipo> *actual = this->primero;
    int i=0, dim=this->getNVertices();
    GrafoNoDirigido<int> grafo;
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
            grafo.agregarArcoND(i,v, arco->getPeso());
            arco=arco->getSig();
        }
        actual=actual->getSig();
        i++;
    }

    return grafo;

}

//LISTAS DE RECORRIDOS ===============================
template<typename Tipo>
list<Tipo> GrafoNoDirigido<Tipo>::listaBFS(Tipo inicio){
    list<Tipo> recorrido;
    list<int> recorridoAux;
    vector<Tipo> mapeo;
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
template<typename Tipo>
list<Tipo> GrafoNoDirigido<Tipo>::listaDFS(Tipo inicio){
    list<Tipo> recorrido;
    list<int> recorridoAux;
    vector<Tipo> mapeo;
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
template <typename Tipo>
list<Tipo> GrafoNoDirigido<Tipo>::caminoMenor(Tipo v, Tipo w, float *peso){
    vector<Tipo> mapeo;
    GrafoNoDirigido<int> aux = this->mapear(&mapeo);
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
list<Tipo> GrafoNoDirigido<Tipo>::caminoMenorConBloqueo(Tipo v, Tipo w, list<Tipo> bloqueados, float *peso){ 
    vector<Tipo> mapeo;
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

    list<Tipo> resultado;
    while(!camino.empty()){
        resultado.push_back(mapeo[camino.front()]);
        camino.pop_front();
    }
    return resultado;
}
template <typename Tipo>
list<Tipo> GrafoNoDirigido<Tipo>::caminoMenorConRequisito(Tipo v, Tipo w, Tipo H, float *peso){ 
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
            if (bloqueos.back() != v){
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
list<Tipo> GrafoNoDirigido<Tipo>::caminoMayor(Tipo v, Tipo w){
    vector<Tipo> mapeo;
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

    list<Tipo> resultado;
    while(!caminoMayor.empty()){
        resultado.push_back(mapeo[caminoMayor.front()]);
        caminoMayor.pop_front();
    }
    return resultado;
}

template<typename Tipo>
void GrafoNoDirigido<Tipo>::arbolExpandidoMinimo(GrafoNoDirigido<int> *g, float *peso){
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
template<typename Tipo>
void GrafoNoDirigido<Tipo>::arcoMinimo(list<int> activos, int *v, int *w, float *peso, vector<bool> visitados, bool *band){
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
template <typename Tipo>
bool GrafoNoDirigido<Tipo>::esConexo(){ //METODO PARA GRAFOS YA MAPEADOS
    vector<Tipo> mapeo;
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
template <typename Tipo>
bool GrafoNoDirigido<Tipo>::esConexoM(){
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
template<typename Tipo>
list<list<Tipo>> GrafoNoDirigido<Tipo>::puentes(){
    list<list<Tipo>> arcosPuente;
    list<Tipo> arco;
    list<int> vecinos;
    float peso;
    int i,w;
    vector<Tipo> m;
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
template<typename Tipo>
list<list<Tipo>> GrafoNoDirigido<Tipo>::puentesM(){
    list<list<Tipo>> arcosPuente;
    list<Tipo> arco;
    list<int> vecinos;
    float peso;
    int i,w;

    for (i=0; i<this->nVertices; i++){ //RECORRER VERTICES 
        vecinos = this->vecinos(i);
        //RECORRER ARCOS
        while(!vecinos.empty()){
            w = vecinos.front();
            peso = this->getPesoArco(i,w);
            //ELIMINAR ARCO
            this->eliminarArcoND(i,w);
            //VERIFICAR QUE EL GRAFO SIGA CONEXO (Si no es conexo, el arco es un arco puente)
            if((!this->esConexoM()) && (i<w)){
                //Agregar arco a la lista de arcos puente
                arco.clear();
                arco.push_back(i);
                arco.push_back(w);
                arcosPuente.push_back(arco);
            }
            //VOLVER A AGREGAR EL ARCO ELIMINADO
            this->agregarArcoND(i,w, peso);
            vecinos.pop_front();
        }

    }
    return arcosPuente;
}



template<typename Tipo>
list<Tipo> GrafoNoDirigido<Tipo>::puntosArticulacion(){
    list<Tipo> puntos, vertices = this->vertices();
    list<Tipo> vecinos;
    list<float> pesos;
    Tipo actual;

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

template <typename Tipo>
list<list<Tipo>> GrafoNoDirigido<Tipo>::caminosHamiltonianos(){
    int i,j, nVisitados = 1;
    list<list<int>> result;
    list<list<Tipo>> hamiltonianos;
    list<int> camAux;
    list<Tipo> cam;
    vector<bool> visitados;
    float peso=0;
    vector<Tipo> m;
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

template <typename Tipo>
list<Tipo> GrafoNoDirigido<Tipo>::caminoHamiltonianoMinimo(){
    int i,j, nVisitados = 1;
    list<int> resultAux;
    list<Tipo> result;
    list<int> camAux;
    list<Tipo> cam;
    vector<bool> visitados;
    float peso=0, menor=0;
    bool prim = true;
    vector<Tipo> m;
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

template <typename Tipo>
list<list<Tipo>> GrafoNoDirigido<Tipo>::ciclosHamiltonianos(){
    int i,j, nVisitados = 1;
    list<list<int>> result;
    list<list<Tipo>> hamiltonianos;
    list<int> camAux;
    list<Tipo> cam;
    vector<bool> visitados;
    float peso=0;
    vector<Tipo> m;
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

template <typename Tipo>
list<Tipo> GrafoNoDirigido<Tipo>::cicloHamiltonianoMinimo(){
    int i,j, nVisitados = 1;
    list<int> resultAux;
    list<Tipo> result;
    list<int> camAux;
    list<Tipo> cam;
    vector<bool> visitados;
    float peso=0, menor=0;
    bool prim = true;
    vector<Tipo> m;
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
template<typename Tipo>
list<list<Tipo>> GrafoNoDirigido<Tipo>::getCaminosEulerianos(){
    vector<Tipo> m;
    GrafoNoDirigido<int> grafoM = this->mapear(&m);
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
bool GrafoNoDirigido<Tipo>::esBipartito(){
    vector<Tipo> mapeo;
    bool bicoloreable = true;
    GrafoNoDirigido<int> aux = this->mapear(&mapeo);
    int fuente = this->buscarMapeo(mapeo, this->primero->getInfo(), this->getNVertices());
    aux.esBipartito(fuente,&bicoloreable);
    return bicoloreable;
}

template<typename Tipo>
void GrafoNoDirigido<Tipo>::esBipartito(int inicio, bool *resultado){
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

template<typename Tipo>
bool GrafoNoDirigido<Tipo>::esMulticoloreable(int nColores){
    vector<Tipo> map;
    bool multicoloreable = true;
    GrafoNoDirigido<int> aux = this->mapear(&map);
    int fuente = this->buscarMapeo(map,this->primero->getInfo(), this->getNVertices());
    aux.esMulticoloreable(fuente,nColores,&multicoloreable);
    return multicoloreable;
}

template<typename Tipo>
void GrafoNoDirigido<Tipo>::esMulticoloreable(int inicio, int nColoresGrafo, bool *resultado){
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