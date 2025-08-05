#ifndef GRAFO_H_
#define GRAFO_H_

#include <list>
#include <queue>
#include <vector>
#include "nodoVertice.hpp"
#include "nodoArco.hpp"
using namespace std;

template <typename Tipo>
class Grafo{
    protected:
        int nVertices, nArcos;
        Vertice<Tipo> *primero;

    public:
        Grafo(){
            this->primero = NULL;
            this->nVertices=0;
            this->nArcos=0;
        }
        ~Grafo(){}
        void construir(){
            this->primero = NULL;
            this->nVertices=0;
            this->nArcos = 0;
        }
        //GETTERS
        int getNVertices();
        int getNArcos();
        Vertice<Tipo>* getVertice(Tipo v) const;  // Agregar const
        Vertice<Tipo> * getPrimero();
        //SETTERS
        void setNVertices(int cant);
        void setNArcos(int cant);
        void setPrimero(Vertice<Tipo> *primero);
        void setPrimero(Tipo primero);

        Vertice<Tipo> * getVertice(Tipo v); //FUNCIONAL

        list<Tipo> sucesores(Vertice<Tipo> *v); //FUNCIONAL
        list<Tipo> sucesores(Tipo v); //FUNCIONAL

        list<Tipo> predecesores(Vertice<Tipo> *v); //FUNCIONAL
        list<Tipo> predecesores(Tipo v); //FUNCIONAL

        list<Tipo> vertices() const;

        //ARCOS
        bool existeArco(Tipo v, Tipo w); //FUNCIONAL
        float getPesoArco(Tipo v, Tipo w); //FUNCIONAL
        void modificarPesoArco(Tipo v, Tipo w, float nuevo); //FUNCIONAL
        void agregarArco(Tipo v, Tipo w, float peso = 0); //FUNCIONAL
        void eliminarArco (Tipo v, Tipo w); //FUNCIONAL
        //VERTICES
        void agregarVertice (Tipo v); //FUNCIONAL
        void eliminarVertice (Tipo v); //FUNCIONAL

        int buscarMapeo(vector<Tipo> arreglo, Tipo elem, int dim);

        void escribirGrafo();
        //RECORRIDOS (SOLO PARA GRAFOS MAPEADOS)
        void BFS(int v, vector<bool> *visitados); //FUNCIONAL
        void BFS(int v, vector<bool> *visitados, list<int> *recorrido);
        void DFS(int v, vector<bool> *visitados, list<int> *recorrido);


        list<int> caminoDijkstra(int v, int w, float *peso = NULL);
        list<int> caminoObstaculos(int v, int w, vector<bool> obstaculos, float *peso = NULL);
        void mayorCamino(int v, int w, float peso, vector<bool> *visitados, list<int> *camino, float *pesoMayor, list<int> *caminoMayor, bool *prim);
        bool esCompleto();

        //CAMINOS HAMILTONIANOS
        void hamiltonianos(int actual, vector<bool> *visitados, int *nVisitados, float *peso, list<list<int>> *caminos, list<int> *caminoActual);
        void hamiltonianoMinimo(int i, vector<bool> *visitados, int *nVisitados, float *peso, float *pesoMenor, list<int> *minimo, list<int> *caminoActual, bool *prim);
        void cHamiltonianos(int i, int inicio, vector<bool> *visitados, int *nVisitados, float *peso, list<list<int>> *caminos, list<int> *caminoActual);
        void chamiltonianoMinimo(int i, int inicio, vector<bool> *visitados, int *nVisitados, float *peso, float *pesoMenor, list<int> *minimo, list<int> *caminoActual, bool *prim);

        void contarGrados(vector<int> *in, vector<int> *out);
        bool existeEuleriano(vector<int> in, vector<int> out, int *v);
};


//GETTERS ==================================================================
template<typename Tipo>
int Grafo<Tipo>::getNVertices(){
    return this->nVertices;
}
template<typename Tipo>
int Grafo<Tipo>::getNArcos(){
    return this->nArcos;
}
template<typename Tipo>
Vertice<Tipo> * Grafo<Tipo>::getPrimero(){
    return this->primero;
}
template<typename Tipo>
Vertice<Tipo> * Grafo<Tipo>::getVertice(Tipo v){
    Vertice<Tipo> *actual = this->primero;
    while(actual != NULL){
        if(actual->getInfo() == v){
            return actual;
        }else{
            actual = actual->getSig();
        }
    }
    return NULL;
}

template <typename Tipo>
Vertice<Tipo>* Grafo<Tipo>::getVertice(Tipo v) const {
    Vertice<Tipo>* actual = primero;
    while (actual != NULL) {
        if (actual->getInfo() == v) {
            return actual;
        }
        actual = actual->getSig();
    }
    return NULL;
}



//SETTERS ===============================================================
template<typename Tipo>
void Grafo<Tipo>::setNVertices(int cant){
    this->nVertices=cant;
}
template<typename Tipo>
void Grafo<Tipo>::setNArcos(int cant){
    this->nArcos=cant;
}
template<typename Tipo>
void Grafo<Tipo>::setPrimero(Vertice<Tipo> *primero){
    this->primero = primero;
}
template<typename Tipo>
void Grafo<Tipo>::setPrimero(Tipo primero){
    Vertice<Tipo> *prim = new (Vertice<Tipo>);
    prim->setInfo(primero);
    prim->setArcos(NULL);
    prim->setSig(NULL);
    this->primero = prim;
    this->nVertices = this->nVertices +1;
}



template<typename Tipo>
list<Tipo> Grafo<Tipo>::sucesores(Vertice<Tipo> *v){
    list<Tipo> lista;

    Arco<Tipo> *arco;
    if(v!=NULL){
        arco = v->getArcos();

        while (arco!=NULL){
            lista.push_back(arco->getInfo()->getInfo());
            arco = arco->getSig();
        }
    }

    return lista;            
}
template<typename Tipo>
list<Tipo> Grafo<Tipo>::sucesores(Tipo v){
    return this->sucesores(this->getVertice(v));            
}
template<typename Tipo>
list<Tipo> Grafo<Tipo>::predecesores(Vertice<Tipo> *v){
    list<Tipo> lista;
    Vertice<Tipo> *inicio = this->primero;
    Arco<Tipo> *ady;
    if (v != NULL){
        while (inicio != NULL){
            if(inicio != v){
                ady = inicio->getArcos();
                while(ady != NULL){
                    Vertice<Tipo> *conexion = ady->getInfo();
                    //Si el vertice adyacente es el nodo buscado, se añade a la lista
                    if(conexion == v){
                        lista.push_back(inicio->getInfo());
                    }
                    ady=ady->getSig();
                }
            }
            inicio=inicio->getSig();
        }
    }

    return lista;
}

template<typename Tipo>
list<Tipo> Grafo<Tipo>::predecesores(Tipo v){
    return this->predecesores(this->getVertice(v));            
}
template<typename Tipo>
list<Tipo> Grafo<Tipo>::vertices() const{
    list<Tipo> result;
    Vertice<Tipo> *actual = this->primero;
    while(actual != NULL){
        result.push_back(actual->getInfo());
        actual = actual->getSig();
    }
    return result;
}
//OPERACIONES CON ARCOS ============================================================
template <typename Tipo>
bool Grafo<Tipo>::existeArco(Tipo v, Tipo w){
    Vertice<Tipo> *inicio = this->primero;
    Arco<Tipo> *ady;
    bool band=false;
    if(inicio != NULL){
        //ENCONTRAR VERTICE V
        while((inicio != NULL) && (inicio->getInfo() != v)){
            inicio = inicio->getSig();
        }
        if (inicio == NULL) return false;
        
        //RECORRER ARCOS DE V
        ady = inicio->getArcos();
        while (!band && ady != NULL){
            Tipo info = ady->getInfo()->getInfo();
            band = info == w;
            ady=ady->getSig();
        }
        return band;
    }
    return band;
}
template<typename Tipo>
float Grafo<Tipo>::getPesoArco(Tipo v, Tipo w){
    Vertice<Tipo> *inicio = this->primero;
    Arco<Tipo> *ady;
    if (inicio != NULL){
        //ENCONTRAR VERTICE V
        while((inicio != NULL) && (inicio->getInfo() != v)){
            inicio = inicio->getSig();
        }
        if (inicio == NULL){
            cout<<"No existe el vertice "<<v<< " (No se puede ver el peso del arco entre " << v << " y " << w<<") retornando  0"<<endl; //MENSAJE DE AVISO
            return 0;
        }
        
        //RECORRER ARCOS DE V HASTA ENCONTRAR ARCO CON W
        ady = inicio->getArcos();
        while ((ady != NULL) && (ady->getInfo()->getInfo() != w)){
            ady=ady->getSig();
        }
        if (ady == NULL){
            cout<<"No existe el arco entre "<<v<< " y "<< w << " (No se puede ver el peso del arco entre " << v << " y " << w<<") retornando 0"<<endl; // MENSAJE DE AVISO
            return 0;
        }

        return ady->getPeso();
    }
    cout<<"El grafo no tiene vertices (retornando 0)"<<endl; //MENSAJE DE AVISO
    return 0;
}

template <typename Tipo>
void Grafo<Tipo>::modificarPesoArco(Tipo v, Tipo w, float nuevo){
    Vertice<Tipo> *inicio = this->primero;
    Arco<Tipo> *ady;
    if(inicio != NULL){
        //ENCONTRAR VERTICE V
        while((inicio != NULL) && (inicio->getInfo() != v)){
            inicio = inicio->getSig();
        }
        if (inicio == NULL) return; //SI V NO EXISTE -> SALIR DEL METODO

        //RECORRER ARCOS DE V HASTA ENCONTRAR ARCO CON W
        ady = inicio->getArcos();
        while ((ady != NULL) && (ady->getInfo()->getInfo() != w)){
            ady=ady->getSig();
        }
        if (ady == NULL) return; //SI NO EXISTE EL ARCO -> SALIR DEL METODO
        ady->setPeso(nuevo);

    }
}

template <typename Tipo>
void Grafo<Tipo>::agregarArco(Tipo v, Tipo w, float peso){
    Vertice<Tipo> *inicio = this->primero, *objetivo = NULL, *verticeAux;
    Arco<Tipo> *ady, *nuevo;
    if (inicio != NULL) {
        
        //ENCONTRAR VERTICE V
        while((inicio!=NULL) && (inicio->getInfo() != v)){
            if (inicio->getInfo() == w){
                objetivo = inicio;
            }
            inicio = inicio->getSig();
        }

        if(inicio != NULL){

            verticeAux = inicio;
            ady = inicio->getArcos();

            while ((objetivo == NULL) && (inicio->getSig() != NULL)){
                if(inicio->getSig()->getInfo() == w){
                    objetivo = inicio->getSig();
                }
                inicio=inicio->getSig();
            }
            if(objetivo == NULL) return;

            //RECORRER ARCOS DE V HASTA ENCONTRAR ARCO CON W
            if (ady != NULL){
                while (ady->getSig() != NULL){
                    if(ady->getInfo()->getInfo() == w){
                        return;
                    }
                    ady=ady->getSig();
                }
                if(ady->getInfo()->getInfo() == w){
                    return;
                }
                nuevo = new Arco<Tipo>;
                nuevo->setInfo(objetivo);
                nuevo->setSig(NULL);
                nuevo->setPeso(peso);
                ady->setSig(nuevo);
                verticeAux->setGradoOut(verticeAux->getGradoOut() + 1);
                objetivo->setGradoIn(objetivo->getGradoIn() + 1);
            }else{
                nuevo = new Arco<Tipo>;
                nuevo->setInfo(objetivo);
                nuevo->setSig(NULL);
                nuevo->setPeso(peso);
                verticeAux->setArcos(nuevo);
                verticeAux->setGradoOut(verticeAux->getGradoOut() + 1);
                objetivo->setGradoIn(objetivo->getGradoIn() + 1);

            }
            this->nArcos = this-> nArcos + 1;
        }else{
            cout<< "El vertice " << v << " no existe" <<endl; 
        } 
    }
}

template <typename Tipo>
void Grafo<Tipo>::eliminarArco (Tipo v, Tipo w){
    Vertice<Tipo> *inicio = this->primero;
    Arco<Tipo> *ady, *objetivo;

    if(this->existeArco(v,w)){
        //ENCONTRAR VERTICE V
        while((inicio!=NULL) && (inicio->getInfo() != v)){
            inicio = inicio->getSig();
        }
        
        //RECORRER ARCOS DE V HASTA ENCONTRAR ARCO CON W
        ady = inicio->getArcos();
        if(ady != NULL){
            if(ady->getInfo()->getInfo() == w){
                inicio->setArcos(ady->getSig());
                inicio->setGradoOut(inicio->getGradoOut() - 1);
                ady->getInfo()->setGradoIn(ady->getInfo()->getGradoIn() - 1);
                delete ady;
                this->nArcos = this->nArcos - 1;
            }else{
                if (ady->getSig() != NULL){
                    while (ady->getSig()->getInfo()->getInfo() != w){
                        ady=ady->getSig();
                    }
                    objetivo = ady->getSig();
                    ady->setSig(objetivo->getSig());
                    objetivo->setSig(NULL);

                    inicio->setGradoOut(inicio->getGradoOut() - 1);
                    objetivo->getInfo()->setGradoIn(objetivo->getInfo()->getGradoIn() - 1);
                    delete objetivo;
                    this->nArcos = this->nArcos - 1;

                }
            }
        }
    }    
}


//OPERACIONES CON VERTICES ================================================================
template <typename Tipo>
void Grafo<Tipo>::agregarVertice (Tipo v){
    Vertice<Tipo> *inicio = this->primero, *nuevo;
    if(inicio != NULL){
        //VERIFICAR QUE NO ESTE REPETIDO
        while(inicio != NULL){
            if(inicio->getInfo() == v) return; //Si el vertice ya se encuentra en el grafo se sale del metodo
            inicio=inicio->getSig();
        }
    }
    //INSERTAR AL INICIO
    inicio = this->primero;
    nuevo = new Vertice<Tipo>;
    nuevo->setInfo(v);
    nuevo->setSig(inicio);
    nuevo->setArcos(NULL);
    nuevo->setGradoIn(0);
    nuevo->setGradoOut(0);
    this->setPrimero(nuevo);
    this->nVertices = this->nVertices + 1;
}
template <typename Tipo>
void Grafo<Tipo>::eliminarVertice (Tipo v){
    Vertice<Tipo> *inicio = this->primero, *objetivo = NULL;
    list<Tipo> conexiones;
    if(inicio != NULL){
        conexiones = this->sucesores(v);
        while(!conexiones.empty()){
            this->eliminarArco(v,conexiones.front());
            conexiones.pop_front();
        }
        conexiones = this->predecesores(v);
        while(!conexiones.empty()){
            this->eliminarArco(conexiones.front(),v);
            conexiones.pop_front();
        }
        
        if(inicio->getInfo() == v){
            objetivo = inicio;
            this->primero = objetivo->getSig();
            objetivo->setSig(NULL);

        }else{
            //ENCONTRAR VERTICE V
            while((inicio->getSig() != NULL) && (inicio->getSig()->getInfo() != v)){
                inicio = inicio->getSig();
            }

            objetivo = inicio->getSig();

            if(objetivo == NULL) return; //SI NO EXISTE EL VERTICE SALE DEL METODO

            inicio->setSig(objetivo->getSig());
            objetivo->setSig(NULL);
        }
        delete objetivo;
        this->nVertices = this->nVertices - 1;
    }
}


template<typename Tipo>
int Grafo<Tipo>::buscarMapeo(vector<Tipo> arreglo, Tipo elem, int dim){
    int i = 0;
    for(i=0;i<dim;i++){
        if(arreglo.at(i) == elem){
            return(i);
        }
    }
    return -1;
}

template<typename Tipo>
void Grafo<Tipo>::escribirGrafo(){
    Vertice<Tipo> *v = this->primero;  
    while(v != NULL){
        cout<< v->getInfo()<<"{" << v->getGradoIn()<<" | "<<v->getGradoOut()<<"}"<<" = ";
        Arco<Tipo> *a = v->getArcos();
        while(a != NULL){
            //ESCRITURA DEL ARCO -> (origen,destino)[peso]
            cout<< "("<< v->getInfo() << ", "<< a->getInfo()->getInfo() << ")[" << this->getPesoArco(v->getInfo(),a->getInfo()->getInfo())<<"]  ";
            a=a->getSig();
        }
        v=v->getSig();
        cout<<endl;    
    }
}


template <>
void Grafo<int>::BFS(int v, vector<bool> *visitados){
    queue<int> cola;
    list<int> vecinos;
    int w;
    cola.push(v);
    if(!visitados->empty()){
        visitados->at(v) = true;
 
        while(!cola.empty()){
            v=cola.front();
            vecinos = this->sucesores(v);
            while(!vecinos.empty()){
                w = vecinos.front();
                if(!visitados->at(w)){
                    visitados->at(w) = true;
                    cola.push(w);
                }
                vecinos.pop_front();
            }
            cola.pop();
        }
    }
    return;
}
template <>
void Grafo<int>::BFS(int v, vector<bool> *visitados, list<int> *recorrido){
    queue<int> cola;
    list<int> vecinos;
    int w;
    cola.push(v);
    if(!visitados->empty()){
        visitados->at(v) = true;
 
        while(!cola.empty()){
            v=cola.front();
            recorrido->push_back(v);
            vecinos = this->sucesores(v);
            while(!vecinos.empty()){
                w = vecinos.front();
                if(!visitados->at(w)){
                    visitados->at(w) = true;
                    cola.push(w);
                }
                vecinos.pop_front();
            }
            cola.pop();
        }
    }
    return;
}
template <>
void Grafo<int>::DFS(int v, vector<bool> *visitados, list<int> *recorrido){
    list<int> vecinos;
    int w;
    vecinos = this->sucesores(v);
    //cout << v << endl;
    recorrido->push_back(v);
    while(!vecinos.empty()){
        w = vecinos.front();
        if(!visitados->at(w)){
            visitados->at(w) = true;
            this->DFS(w, visitados, recorrido);
        }
        vecinos.pop_front();
    }
}

template <>
list<int> Grafo<int>::caminoDijkstra(int v, int w, float *peso){
    vector<float> costos;
    vector<int> camino;
    list<int> resultado, vecinos;
    queue<int> cola;
    int i, actual, destino;
    float costo; 

    for(i=0;i<this->getNVertices();i++){
        costos.push_back(-1);
        camino.push_back(-1);
    }
    if((v<this->getNVertices()) && (w<this->getNVertices())){
        costos.at(v) = 0;
        cola.push(v);
        while(!cola.empty()){
            actual=cola.front();
            vecinos = this->sucesores(actual);
            while(!vecinos.empty()){
                destino = vecinos.front();
                if(camino.at(actual) == -1){
                    costo=costos.at(actual) + this->getPesoArco(actual,destino);
                }
                else{
                    costo=costos.at(camino.at(actual)) + this->getPesoArco(camino.at(actual), actual) + this->getPesoArco(actual,destino);
                }
                if((costo<costos.at(destino)) || (costos.at(destino) == -1)){
                    costos.at(destino) = costo;
                    camino.at(destino) = actual;
                    cola.push(destino);
                }
                vecinos.pop_front();
            }
            cola.pop();
        }
        if(camino.at(w) != -1){
            if (peso != NULL){
                *peso = costos.at(w);
            }
            resultado.push_back(w);
            actual = w;
            while((actual != v) && (actual != -1)){
                actual = camino.at(actual);
                resultado.push_front(actual);
            }
        }
    }
    return resultado;
}


template <>
list<int> Grafo<int>::caminoObstaculos(int v, int w, vector<bool> obstaculos, float *peso){
    vector<float> costos;
    vector<int> camino;
    list<int> resultado, vecinos;
    queue<int> cola;
    int i, actual, destino;
    float costo; 

    for(i=0;i<this->getNVertices();i++){
        costos.push_back(-1);
        camino.push_back(-1);
    }
    if((v<this->getNVertices()) && (w<this->getNVertices())){
        costos.at(v) = 0;
        cola.push(v);
        while(!cola.empty()){
            actual=cola.front();
            vecinos = this->sucesores(actual);
            while(!vecinos.empty()){
                destino = vecinos.front();
                if(!obstaculos.at(destino)){
                    if(camino.at(actual) == -1){
                        costo=costos.at(actual) + this->getPesoArco(actual,destino);
                    }
                    else{
                        costo=costos.at(camino.at(actual)) + this->getPesoArco(camino.at(actual), actual) + this->getPesoArco(actual,destino);
                    }
                    if((costo<costos.at(destino)) || (costos.at(destino) == -1)){
                        costos.at(destino) = costo;
                        camino.at(destino) = actual;
                        cola.push(destino);
                    }
                }
                vecinos.pop_front();
            }
            cola.pop();
        }
        if(camino.at(w) != -1){
            if (peso != NULL){
                *peso = costos.at(w);
            }
            resultado.push_back(w);
            actual = w;
            while((actual != v) && (actual != -1)){
                actual = camino.at(actual);
                resultado.push_front(actual);
            }
        }
    }
    return resultado;
}


template<>
void Grafo<int>::mayorCamino(int v, int w, float peso, vector<bool> *visitados, list<int> *camino, float *pesoMayor, list<int> *caminoMayor, bool *prim){
    list<int> vecinos = this->sucesores(v);
    int sig;
    while(!vecinos.empty()){
        sig = vecinos.front();
        if(!visitados->at(sig)){
            visitados->at(sig) = true;
            peso = peso + this->getPesoArco(v,sig);
            camino->push_back(sig);
            if ((sig == w) && ((peso > *pesoMayor) || *prim)){
                *caminoMayor = *camino;
                *pesoMayor = peso;
                *prim = false;
            }
            this->mayorCamino(sig,w,peso,visitados, camino, pesoMayor, caminoMayor, prim);
            camino->pop_back();
            peso = peso - this->getPesoArco(v, sig);
            visitados->at(sig) = false;
        }
        vecinos.pop_front();
    }
}
template<typename Tipo>
bool Grafo<Tipo>::esCompleto(){
    return (this->nArcos == (this->nVertices * (this->nVertices - 1)));
}


template <>
void Grafo<int>::hamiltonianos(int i, vector<bool> *visitados, int *nVisitados, float *peso, list<list<int>> *caminos, list<int> *caminoActual){
    int w;
    //INICIALIZAR ALTERNATIVAS
    list<int>vecinos = this->sucesores(i);
    while(!vecinos.empty()){
        //INICIALIZAR PASO
        w = vecinos.front();
        //VERIFICAR SI EL PASO ES VALIDO
        if(!visitados->at(w)){
            //PROCESAR PASO
            visitados->at(w) = true;
            *peso = *peso + this->getPesoArco(i,w);
            caminoActual->push_back(w);
            *nVisitados = *nVisitados + 1;
            //VERIFICAR SI ES SOLUCION
            if(*nVisitados == this->nVertices){
                caminos->push_back(*caminoActual);
            }else{
                //SIGUIENTE PASO
                this->hamiltonianos(w, visitados, nVisitados, peso, caminos, caminoActual);
            }
            //BORRAR PASO
            visitados->at(w) = false;
            *peso = *peso - this->getPesoArco(i,w);
            caminoActual->pop_back();
            *nVisitados = *nVisitados - 1;
        }
        vecinos.pop_front();
    }
}
template <>
void Grafo<int>::hamiltonianoMinimo(int i, vector<bool> *visitados, int *nVisitados, float *peso, float *pesoMenor, list<int> *minimo, list<int> *caminoActual, bool *prim){
    int w;
    //INICIALIZAR ALTERNATIVAS
    list<int>vecinos = this->sucesores(i);
    while(!vecinos.empty()){
        //INICIALIZAR PASO
        w = vecinos.front();
        //VERIFICAR SI EL PASO ES VALIDO
        if(!visitados->at(w)){
            //PROCESAR PASO
            visitados->at(w) = true;
            *peso = *peso + this->getPesoArco(i,w);
            caminoActual->push_back(w);
            *nVisitados = *nVisitados + 1;
            //VERIFICAR SI ES SOLUCION
            if((*nVisitados == this->nVertices) && ((*peso<*pesoMenor)||*prim)){
                *minimo = *caminoActual;
                *pesoMenor = *peso;
                *prim = false;
            }else{
                //SIGUIENTE PASO
                this->hamiltonianoMinimo(w, visitados, nVisitados, peso, pesoMenor, minimo, caminoActual, prim);
            }
            //BORRAR PASO
            visitados->at(w) = false;
            *peso = *peso - this->getPesoArco(i,w);
            caminoActual->pop_back();
            *nVisitados = *nVisitados - 1;
        }
        vecinos.pop_front();
    }
}

template <>
void Grafo<int>::cHamiltonianos(int i, int inicio, vector<bool> *visitados, int *nVisitados, float *peso, list<list<int>> *caminos, list<int> *caminoActual){
    int w;
    //INICIALIZAR ALTERNATIVAS
    list<int>vecinos = this->sucesores(i);
    while(!vecinos.empty()){
        //INICIALIZAR PASO
        w = vecinos.front();
        //VERIFICAR SI EL PASO ES VALIDO
        if(!visitados->at(w)){
            //PROCESAR PASO
            visitados->at(w) = true;
            *peso = *peso + this->getPesoArco(i,w);
            caminoActual->push_back(w);
            *nVisitados = *nVisitados + 1;
            //VERIFICAR SI ES SOLUCION
            if((*nVisitados == this->nVertices + 1) && (w == inicio)){
                caminos->push_back(*caminoActual);
            }else{
                //SIGUIENTE PASO
                this->cHamiltonianos(w,inicio, visitados, nVisitados, peso, caminos, caminoActual);
            }
            //BORRAR PASO
            visitados->at(w) = false;
            *peso = *peso - this->getPesoArco(i,w);
            caminoActual->pop_back();
            *nVisitados = *nVisitados - 1;
        }
        vecinos.pop_front();
    }
}

template <>
void Grafo<int>::chamiltonianoMinimo(int i, int inicio, vector<bool> *visitados, int *nVisitados, float *peso, float *pesoMenor, list<int> *minimo, list<int> *caminoActual, bool *prim){
    int w;
    //INICIALIZAR ALTERNATIVAS
    list<int>vecinos = this->sucesores(i);
    while(!vecinos.empty()){
        //INICIALIZAR PASO
        w = vecinos.front();
        //VERIFICAR SI EL PASO ES VALIDO
        if(!visitados->at(w)){
            //PROCESAR PASO
            visitados->at(w) = true;
            *peso = *peso + this->getPesoArco(i,w);
            caminoActual->push_back(w);
            *nVisitados = *nVisitados + 1;
            //VERIFICAR SI ES SOLUCION
            if((*nVisitados == this->nVertices + 1) && (w == inicio) && ((*peso<*pesoMenor)||*prim)){
                *minimo = *caminoActual;
                *pesoMenor = *peso;
                *prim = false;
            }else{
                //SIGUIENTE PASO
                this->chamiltonianoMinimo(w, inicio, visitados, nVisitados, peso, pesoMenor, minimo, caminoActual, prim);
            }
            //BORRAR PASO
            visitados->at(w) = false;
            *peso = *peso - this->getPesoArco(i,w);
            caminoActual->pop_back();
            *nVisitados = *nVisitados - 1;
        }
        vecinos.pop_front();
    }
}

template<>
void Grafo<int>::contarGrados(vector<int> *in, vector<int> *out){
    int i;
    Vertice<int> *v;
    for(i=0;i<this->getNVertices(); i++){
        v = this->getVertice(i);
        in->emplace_back(v->getGradoIn());
        out->emplace_back(v->getGradoOut());
    }
}
template<>
bool Grafo<int>::existeEuleriano(vector<int> in, vector<int> out, int *v){
    int i, vInicio=0, vFinal=0;
    for(i=0;i<this->getNVertices();i++){
        if((out.at(i) - in.at(i) > 1) || (in.at(i) - out.at(i))>1) return false;

        if(out.at(i) - in.at(i) == 1){
            *v=i;
            vInicio++;
        }else{
            if(in.at(i) - out.at(i) == 1){
                vFinal++;
            }
        }
    }
    return ((vInicio == 0) && (vFinal == 0)) || ((vInicio == 1) && (vFinal==1));
}


#endif