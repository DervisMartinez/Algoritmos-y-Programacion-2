#ifndef ARBOL_BIN
#define ARBOL_BIN

#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include "../node/node.hpp"

using namespace std;

/*La función utiliza una plantilla (template) para trabajar con cualquier tipo de dato T.*/
template <typename T>
class ArbolBinario {
private:
    Nodo<T>* raiz;

    // Métodos auxiliares privados
    void eliminarArbol(Nodo<T>* nodo);
    Nodo<T>* reconstruirDesdePreIn(const vector<T>& inorden, const vector<T>& preorden, int inStart, int inEnd, int& preIndex, unordered_map<T, int>& inMap);
    Nodo<T>* reconstruirDesdePostIn(const vector<T>& inorden, const vector<T>& postorden, int inStart, int inEnd, int& postIndex, unordered_map<T, int>& inMap);
    Nodo<T>* buscarNodo(Nodo<T>* nodo, const T& dato) const;
    void preorden(Nodo<T>* nodo, list<T>& resultado) const;
    void inorden(Nodo<T>* nodo, list<T>& resultado) const;
    void postorden(Nodo<T>* nodo, list<T>& resultado) const;
    void obtenerHojas(Nodo<T>* nodo, list<T>& hojas) const;

public:
    // Constructor y destructor
    ArbolBinario();
    ~ArbolBinario();
    ArbolBinario(const ArbolBinario<T>& otro);  // Constructor de copia

    // Reconstrucción de árboles
    void reconstruir(const vector<T>& inorden, const vector<T>& recorrido, bool esPreorden);

    // Cálculo de distancias
    int distanciaNodos(const T& x, const T& y) const; //Calcula la distancia entre dos nodos (x y y) en términos de número de aristas que los separan.
    int distanciaNiveles(const T& x, const T& y) const; //Calcula la diferencia de niveles entre dos nodos (x y y) en el árbol.

    // Operaciones básicas
    bool buscar(const T& dato) const; //erifica si un valor (dato) existe en el árbol.
    void insertar(const T& dato); //Agrega un nuevo valor al árbol. La forma en que se implementa depende de las propiedades del árbol 
    bool eliminar(const T& dato); //Elimina un nodo con el valor especificado del árbol, asegurándose de mantener la estructura válida.

    // Recorridos
    list<T> preorden() const;
    list<T> inorden() const;
    list<T> postorden() const;
    list<T> porniveles() const;

    // Métodos adicionales
    bool isomorfo(const ArbolBinario<T>& otro) const; //Determina si dos árboles binarios tienen la misma estructura (aunque los datos almacenados en ellos pueden ser diferentes).

    //GETTERS
    list<T> getHojas() const; //Devuelve una lista con los valores de los nodos hojas del árbol.
    int getPeso(Nodo<T> *raiz);
    Nodo<T> * getRaiz(); //
    T infoRaiz(); //
    ArbolBinario<T> hijoIzq();//
    ArbolBinario<T> hijoDer();//
    //SETTERS
    void setPeso(int peso);
    void setRaiz(Nodo<T> * apuntador);//
    void setInfoRaiz(T valor);

    //RECORRIDOS DEL ARBOL
    void imprimirPreOrden(); 
    void imprimirPostOrden();
    void imprimirInOrden(); 
    void imprimirPorNiveles(); 
    void imprimirPreOrden(Nodo<T> *raiz); 
    void imprimirPostOrden(Nodo<T> *raiz); 
    void imprimirInOrden(Nodo<T> *raiz); 
    void imprimirPorNiveles(queue<Nodo<T>*> actual);


    // Operador de asignación
    ArbolBinario<T>& operator=(const ArbolBinario<T>& otro);
};


//========================== IMPLEMNTACIONES ======================================================================
// Implementación de métodos
template <typename T>
ArbolBinario<T>::ArbolBinario() : raiz(NULL) {} //Inicializa el árbol binario como vacío.

template <typename T> // Libera la memoria utilizada por el árbol binario al eliminar todos los nodos.
ArbolBinario<T>::~ArbolBinario() {
    eliminarArbol(raiz);
}

template <typename T>
ArbolBinario<T>::ArbolBinario(const ArbolBinario<T>& otro) : raiz(NULL) { //Crea una copia exacta del árbol binario proporcionado
    if (otro.raiz) {
        queue<Nodo<T>*> cola;
        cola.push(otro.raiz);
        while (!cola.empty()) {
            Nodo<T>* actual = cola.front();
            cola.pop();
            insertar(actual->valorNodo());
            if (actual->subarbolIzdo()) cola.push(actual->subarbolIzdo());
            if (actual->subarbolDcho()) cola.push(actual->subarbolDcho());
        }
    }
}


template <typename T>
void ArbolBinario<T>::eliminarArbol(Nodo<T>* nodo) { //Nodo<T>* nodo: Es un puntero al nodo desde el cual se comenzará a eliminar. Habitualmente, este nodo será la raíz del árbol.
    if (nodo) {         // if (nodo): La función solo procede si el puntero al nodo no es nullptr. Si el nodo es nulo, significa que ya se ha alcanzado el final de una rama o el árbol está vacío, por lo que no hay nada más que eliminar.
        eliminarArbol(nodo->subarbolIzdo()); //Llama a la función para eliminar primero el subárbol izquierdo del nodo actual
        eliminarArbol(nodo->subarbolDcho()); //llama a la función para eliminar el subárbol derecho
        delete nodo;
    }
}


//reconstruir el arbol 
template <typename T>
void ArbolBinario<T>::reconstruir(
    const vector<T>& inorden, 
    const vector<T>& recorrido, 
    bool esPreorden
) {
    unordered_map<T, int> inMap; //Crea un mapa hash que relaciona los valores de los nodos con sus índices en el recorrido INORDEN. Esto permite buscar rápidamente la posición de cualquier valor en INORDEN.
    for (size_t i = 0; i < inorden.size(); ++i) {
        inMap[inorden[i]] = i;
    }

    try {
        /*
        Si el recorrido es PREORDEN:
        Declara preIndex inicializado en 0 para comenzar desde el inicio del recorrido PREORDEN.
        Llama al método auxiliar reconstruirDesdePreIn, que utiliza el recorrido PREORDEN y el mapa INORDEN para reconstruir el árbol
        */
        if (esPreorden) {
            int preIndex = 0;
            raiz = reconstruirDesdePreIn(inorden, recorrido, 0, inorden.size()-1, preIndex, inMap);
        } else {
            /*
            Si el recorrido es POSTORDEN:
            Declara postIndex inicializado en el último índice del recorrido POSTORDEN (comienza desde el final).
            Llama al método auxiliar reconstruirDesdePostIn, que utiliza el recorrido POSTORDEN y el mapa INORDEN.
            */
            int postIndex = recorrido.size() - 1;
            raiz = reconstruirDesdePostIn(inorden, recorrido, 0, inorden.size()-1, postIndex, inMap);
        }
    } catch (const invalid_argument& e) { //Si se encuentra un valor en el recorrido que no existe en INORDEN, se lanza una excepción invalid_argument con un mensaje de error.
        cerr << "[Error] " << e.what() << endl;
        exit(1);
    }
}

template <typename T>  
Nodo<T>* ArbolBinario<T>::reconstruirDesdePreIn(
    const vector<T>& inorden, 
    const vector<T>& preorden, 
    int inStart, 
    int inEnd, 
    int& preIndex, 
    unordered_map<T, int>& inMap
) {
    if (inStart > inEnd) return NULL; 
    /*Si el rango de índices en INORDEN es inválido (cuando inStart > inEnd), significa que no hay más nodos que procesar. Retorna NULL, indicando que el subárbol actual está vacío.*/

    T valor = preorden[preIndex++]; //El recorrido PREORDEN define que el primer nodo del rango actual es la raíz.
    if (inMap.find(valor) == inMap.end()) { //Se asegura de que el valor obtenido de PREORDEN esté presente en INORDEN.
        throw invalid_argument("Nodo '" + valor + "' no existe en INORDEN");
    }
    int inIndex = inMap[valor];

    Nodo<T>* nodo = new Nodo<T>(valor); //Se crea un nuevo nodo con el valor obtenido de PREORDEN.
    nodo->setIzquierdo(reconstruirDesdePreIn(inorden, preorden, inStart, inIndex - 1, preIndex, inMap)); //El rango de índices en INORDEN para el subárbol izquierdo va desde inStart hasta inIndex - 1.
    nodo->setDerecho(reconstruirDesdePreIn(inorden, preorden, inIndex + 1, inEnd, preIndex, inMap));

    return nodo; //Devuelve el nodo recién creado, que representa la raíz del árbol o subárbol procesado.
}

template <typename T>
Nodo<T>* ArbolBinario<T>::reconstruirDesdePostIn(
    const vector<T>& inorden, 
    const vector<T>& postorden,
    int inStart, 
    int inEnd, 
    int& postIndex, 
    unordered_map<T, int>& inMap
) {
    if (inStart > inEnd) return NULL; //Si inStart es mayor que inEnd, el rango de índices en inorden es inválido. Esto significa que el subárbol actual está vacío, por lo que la función devuelve null
    //El recorrido POSTORDEN especifica que el último nodo del rango actual es la raíz del subárbol.
    T valor = postorden[postIndex--]; 
    if (inMap.find(valor) == inMap.end()) {
        throw invalid_argument("Nodo '" + valor + "' no existe en INORDEN"); //Comprueba que el valor obtenido de POSTORDEN esté presente en INORDEN. Si no lo está, lanza una excepción.
    }
    int inIndex = inMap[valor]; /*Recupera la posición del nodo en INORDEN utilizando el mapa inMap. Este índice divide el vector INORDEN en dos partes Izquierda: Subárbol izquierdo.*/

    Nodo<T>* nodo = new Nodo<T>(valor); //Se crea un nuevo nodo (Nodo<T>) con el valor obtenido de POSTORDEN.
    /*El subárbol derecho debe construirse antes del izquierdo porque el POSTORDEN recorre primero el subárbol izquierdo, luego el derecho, y finalmente la raíz.*/
    nodo->setDerecho(reconstruirDesdePostIn(inorden, postorden, inIndex + 1, inEnd, postIndex, inMap));
    nodo->setIzquierdo(reconstruirDesdePostIn(inorden, postorden, inStart, inIndex - 1, postIndex, inMap));

    return nodo; //Devuelve el nodo recién creado, que representa la raíz del subárbol actual.
}

template <typename T>
int ArbolBinario<T>::distanciaNodos(const T& x, const T& y) const {
    if (!raiz || x == y) return 0;
    if (!buscar(x) || !buscar(y)) return 0;
    /*
    !raiz: Si el árbol está vacío (su raíz es nula), no hay distancia que calcular, por lo que devuelve 0.
    x == y: Si los nodos x y y son iguales, la distancia es 0 porque están en el mismo lugar.
    !buscar(x) o !buscar(y): Si alguno de los nodos no existe en el árbol, la distancia también se considera 0.
    */

    unordered_map<T, T> padres; //unordered_map<T, T> padres: Este mapa almacena el nodo padre de cada nodo del árbol. Facilitará encontrar los caminos desde los nodos x y y hacia la raíz.
    queue<Nodo<T>*> cola; //Cola para recorrido por niveles: Se utiliza una cola para realizar un recorrido en anchura del árbol (BFS), comenzando desde la raíz.
    cola.push(raiz);
    padres[raiz->valorNodo()] = T(); //Asignar padre de la raíz: La raíz no tiene padre, por lo que se asigna un valor especial (como T()).

    
    while (!cola.empty()) {
        Nodo<T>* actual = cola.front();
        cola.pop();

        if (actual->subarbolIzdo()) {
            T val = actual->subarbolIzdo()->valorNodo();   /*Realiza un recorrido en anchura para llenar el mapa de padres (padres). Para cada nodo procesado:*/
            padres[val] = actual->valorNodo();             /*  Si tiene un hijo izquierdo, registra al hijo en el mapa con el nodo actual como su padre y lo añade a la cola.  */
            cola.push(actual->subarbolIzdo());              /* Si tiene un hijo derecho, hace lo mismo */
        }
        if (actual->subarbolDcho()) {
            T val = actual->subarbolDcho()->valorNodo();
            padres[val] = actual->valorNodo();
            cola.push(actual->subarbolDcho());
        }
    }
    /*Esto asegura que, al finalizar, todos los nodos del árbol estén registrados con su padre en el mapa padres.*/

    unordered_set<T> caminoX; /* creo un conjunto llamado caminoX para almacenar todos los nodos en el camino desde el nodo x hacia la raíz.*/
    T curr = x;
    while (padres.find(curr) != padres.end() && padres[curr] != T()) { //Siguiendo el mapa padres, se mueve de x hacia su padre, y luego al padre del padre, hasta llegar a la raíz. Cada nodo encontrado se inserta en el conjunto caminoX.
        caminoX.insert(curr); 
        curr = padres[curr];
    }

    curr = y; //Este bucle recorre el camino desde y hacia la raíz, hasta encontrar un nodo que ya exista en el conjunto caminoX. Esto funciona porque el primer nodo común encontrado entre x y y es su ancestro común más cercano
    int distancia = 0;  
    while (padres.find(curr) != padres.end() && !caminoX.count(curr)) {
        curr = padres[curr];
        distancia++; //Cada paso en este camino incrementa la distancia en 1.
    }
    //calcula la distancia desde x hasta el ancestro común (curr), siguiendo el mapa padres
    T temp = x;
    while (temp != curr) {
        temp = padres[temp];
        distancia++; //Este valor se suma a la distancia ya calculada desde y hasta el ancestro común.
    }
    return distancia - 1; //el valor de distancia representa la cantidad de aristas necesarias para conectar los nodos x y y.
};

//calcula la diferencia de niveles entre dos nodos en un árbol binario
template <typename T>
int ArbolBinario<T>::distanciaNiveles(const T& x, const T& y) const {
    if (!raiz) return 0;  // !raiz: Si la raíz es nula (el árbol está vacío), no hay niveles que calcular y retorna 0.
    if (!buscar(x) || !buscar(y)) {
        throw invalid_argument("Nodo no encontrado en el árbol");  /* si !buscar(x) o !buscar(y): Verifica  que si los nodos x y y existen en el árbol.  Y Si alguno no está, lanza una excepción indicando que el nodo no fue encontrado.*/
    };

    //unordered_map<T, int> profundidad: Crea un mapa para almacenar la profundidad de cada nodo (nivel). La clave es el valor del nodo y el valor asociado es su nivel.
    unordered_map<T, int> profundidad;
    queue<Nodo<T>*> cola; //Se utiliza una cola para realizar un recorrido por niveles BFS comenzando desde la raíz.
    cola.push(raiz);
    profundidad[raiz->valorNodo()] = 0; //La profundidad de la raíz se establece como 0.

    //Mientras la cola no esté vacía Se procesa un nodo, y luego se añaden sus hijos a la cola para seguir recorriendo los niveles
    while (!cola.empty()) {
        Nodo<T>* actual = cola.front();
        cola.pop();

        if (actual->subarbolIzdo()) {       
            T val = actual->subarbolIzdo()->valorNodo();  //Obtiene el valor del hijo izquierdo.
            profundidad[val] = profundidad[actual->valorNodo()] + 1; //Registra su profundidad como profundidad del nodo actual + 1.
            cola.push(actual->subarbolIzdo());
        }
        if (actual->subarbolDcho()) { 
            T val = actual->subarbolDcho()->valorNodo(); //Obtiene el valor del hijo Derecho
            profundidad[val] = profundidad[actual->valorNodo()] + 1; //Registra su profundidad como profundidad del nodo actual + 1.
            cola.push(actual->subarbolDcho());
        }
    }

    return abs(profundidad[x] - profundidad[y]);//Calcula la diferencia entre las profundidades de los nodos x y y.
}

template <typename T>
bool ArbolBinario<T>::buscar(const T& dato) const {
    return buscarNodo(raiz, dato) != NULL; //Llama a un método privado auxiliar, buscarNodo, que realiza la búsqueda real de dato.
}

template <typename T>
Nodo<T>* ArbolBinario<T>::buscarNodo(Nodo<T>* nodo, const T& dato) const {
    //Si el puntero nodo es nullptr, significa que se ha alcanzado el final de una rama y no se encontró el valor dato. En este caso, la función retorna NULL.
    if (!nodo) return NULL; 
    //Compara el valor almacenado en el nodo actual con el valor que se busca (dato).
    //Si los valores coinciden, se devuelve un puntero al nodo actual, lo que indica que el nodo que contiene dato ha sido localizado.
    if (nodo->valorNodo() == dato) return nodo;  
    //buscar el valor dato en el subárbol izquierdo del nodo actual (nodo->subarbolIzdo()).
    Nodo<T>* encontrado = buscarNodo(nodo->subarbolIzdo(), dato);
    return encontrado ? encontrado : buscarNodo(nodo->subarbolDcho(), dato); 
}

template <typename T>
list<T> ArbolBinario<T>::preorden() const {
    list<T> resultado; //lista (resultado) donde se almacenarán los valores de los nodos en el orden preorden.
    preorden(raiz, resultado);
    return resultado;
}

template <typename T>
void ArbolBinario<T>::preorden(Nodo<T>* nodo, list<T>& resultado) const {
    if (nodo) {
        resultado.push_back(nodo->valorNodo());
        preorden(nodo->subarbolIzdo(), resultado);
        preorden(nodo->subarbolDcho(), resultado);
    }
}

template <typename T>
list<T> ArbolBinario<T>::inorden() const {
    list<T> resultado;
    inorden(raiz, resultado);
    return resultado;
}

template <typename T>
void ArbolBinario<T>::inorden(Nodo<T>* nodo, list<T>& resultado) const {
    if (nodo) {
        inorden(nodo->subarbolIzdo(), resultado);
        resultado.push_back(nodo->valorNodo());
        inorden(nodo->subarbolDcho(), resultado);
    }
}

template <typename T>
list<T> ArbolBinario<T>::postorden() const {
    list<T> resultado;
    postorden(raiz, resultado);
    return resultado;
}

template <typename T>
void ArbolBinario<T>::postorden(Nodo<T>* nodo, list<T>& resultado) const {
    if (nodo) {
        postorden(nodo->subarbolIzdo(), resultado);
        postorden(nodo->subarbolDcho(), resultado);
        resultado.push_back(nodo->valorNodo());
    }
}

template <typename T>
list<T> ArbolBinario<T>::porniveles() const {
    list<T> resultado;
    if (!raiz) return resultado;

    queue<Nodo<T>*> cola;
    cola.push(raiz);

    while (!cola.empty()) {
        Nodo<T>* actual = cola.front();
        cola.pop();
        resultado.push_back(actual->valorNodo());

        if (actual->subarbolIzdo()) cola.push(actual->subarbolIzdo());
        if (actual->subarbolDcho()) cola.push(actual->subarbolDcho());
    }

    return resultado;
}

template <typename T>
void ArbolBinario<T>::insertar(const T& dato) {
    if (!raiz) {
        raiz = new Nodo<T>(dato);
        return;
    }

    queue<Nodo<T>*> cola;
    cola.push(raiz);

    while (!cola.empty()) {
        Nodo<T>* actual = cola.front();
        cola.pop();

        if (!actual->subarbolIzdo()) {
            actual->setIzquierdo(new Nodo<T>(dato));
            break;
        } else {
            cola.push(actual->subarbolIzdo());
        }

        if (!actual->subarbolDcho()) {
            actual->setDerecho(new Nodo<T>(dato));
            break;
        } else {
            cola.push(actual->subarbolDcho());
        }
    }
}

template <typename T>
bool ArbolBinario<T>::eliminar(const T& dato) {
    Nodo<T>* padre = NULL;
    Nodo<T>* actual = raiz;

    // Buscar el nodo a eliminar y su padre
    while (actual != NULL && actual->valorNodo() != dato) {
        padre = actual;
        if (dato < actual->valorNodo()) {
            actual = actual->subarbolIzdo();
        } else {
            actual = actual->subarbolDcho();
        }
    }

    // Si el nodo no existe en el árbol
    if (actual == NULL) {
        cerr << "El dato no existe en el árbol." << endl;
        return false;
    }

    // Caso 1: El nodo no tiene hijos (nodo hoja)
    if (actual->subarbolIzdo() == NULL && actual->subarbolDcho() == NULL) {
        if (padre == NULL) { // Si el nodo es la raíz
            raiz = NULL;
        } else if (padre->subarbolIzdo() == actual) {
            padre->setIzquierdo(NULL);
        } else {
            padre->setDerecho(NULL);
        }
        delete actual;
    }
    // Caso 2: El nodo tiene un solo hijo
    else if (actual->subarbolIzdo() == NULL || actual->subarbolDcho() == NULL) {
        Nodo<T>* hijo = (actual->subarbolIzdo() != NULL) ? actual->subarbolIzdo() : actual->subarbolDcho();

        if (padre == NULL) { // Si el nodo es la raíz
            raiz = hijo;
        } else if (padre->subarbolIzdo() == actual) {
            padre->setIzquierdo(hijo);
        } else {
            padre->setDerecho(hijo);
        }
        delete actual;
    }
    // Caso 3: El nodo tiene dos hijos
    else {
        // Buscar el sucesor (el menor del subárbol derecho)
        Nodo<T>* sucesorPadre = actual;
        Nodo<T>* sucesor = actual->subarbolDcho();
        while (sucesor->subarbolIzdo() != NULL) {
            sucesorPadre = sucesor;
            sucesor = sucesor->subarbolIzdo();
        }

        // Reemplazar el valor del nodo actual por el valor del sucesor
        actual->valorNodo() = sucesor->valorNodo();

        // Eliminar el sucesor
        if (sucesorPadre->subarbolIzdo() == sucesor) {
            sucesorPadre->setIzquierdo(sucesor->subarbolDcho());
        } else {
            sucesorPadre->setDerecho(sucesor->subarbolDcho());
        }
        delete sucesor;
    }

    return true;
}


template <typename T>
bool ArbolBinario<T>::isomorfo(const ArbolBinario<T>& otro) const {
    // Implementación de isomorfismo
    queue<Nodo<T>*> cola1, cola2;
    cola1.push(raiz);
    cola2.push(otro.raiz);

    while (!cola1.empty() && !cola2.empty()) {
        Nodo<T>* n1 = cola1.front(); cola1.pop();
        Nodo<T>* n2 = cola2.front(); cola2.pop();

        if (!n1 && !n2) continue;
        if (!n1 || !n2) return false;

        cola1.push(n1->subarbolIzdo());
        cola1.push(n1->subarbolDcho());
        cola2.push(n2->subarbolIzdo());
        cola2.push(n2->subarbolDcho());
    }

    return cola1.empty() && cola2.empty();
}
//=====================================GETTERS======================================================

template <typename T>
list<T> ArbolBinario<T>::getHojas() const {
    list<T> hojas;
    obtenerHojas(raiz, hojas);
    return hojas;
}

template <typename T>
ArbolBinario<T> ArbolBinario<T>::hijoIzq(){
    ArbolBinario<T> hijo;
    if (this->raiz != NULL){
        hijo.raiz = copiarNodos(this->getRaiz()->getHijoIzq());
    }else{
        hijo.raiz = NULL;
    }
    return (hijo);
}

template <typename T>
ArbolBinario<T> ArbolBinario<T>::hijoDer(){
    ArbolBinario<T> hijo;
    if (this->raiz != NULL){
        hijo.raiz = copiarNodos(this->getRaiz()->getHijoDer());
    }else{
        hijo.raiz = NULL;
    }
    return (hijo);
}

template <typename T>
T ArbolBinario<T>::infoRaiz(){
    return (this->getRaiz()->getInfo());
}

/*template <typename T>
int ArbolBinario<T>::getPeso(){
    return this->peso;
}*/

template <typename T>
int ArbolBinario<T>::getPeso(Nodo<T> *raiz){
    list<int> longitud;
    preOrden(raiz, &longitud);
    return  longitud.size();
}

template <typename T>
void ArbolBinario<T>::obtenerHojas(Nodo<T>* nodo, list<T>& hojas) const {
    if (!nodo) return;
    
    if (!nodo->subarbolIzdo() && !nodo->subarbolDcho()) {
        hojas.push_back(nodo->valorNodo());
    } else {
        obtenerHojas(nodo->subarbolIzdo(), hojas);
        obtenerHojas(nodo->subarbolDcho(), hojas);
    }
}

//========================================= SETTERS ======================================================================

template <typename T>
void ArbolBinario<T>::setPeso(int peso){
    this->peso = peso;
}

template <typename T>
void ArbolBinario<T>::setRaiz(Nodo<T> * apuntador){
    this->raiz = apuntador;
}
template <typename T>
void ArbolBinario<T>::setInfoRaiz(T valor){
    if (this->raiz == NULL){
        this->raiz = new Nodo<T>;
        this->raiz->crear(valor);
        this->peso = 1;
    }else{
        this->raiz->setInfo(valor);
    }
}

//====================== OPERATOR ================================================================================================
template <typename T>
ArbolBinario<T>& ArbolBinario<T>::operator=(const ArbolBinario<T>& otro) {
    if (this != &otro) {
        eliminarArbol(raiz);
        raiz = NULL;
        if (otro.raiz) {
            queue<Nodo<T>*> cola;
            cola.push(otro.raiz);
            while (!cola.empty()) {
                Nodo<T>* actual = cola.front();
                cola.pop();
                insertar(actual->valorNodo());
                if (actual->subarbolIzdo()) cola.push(actual->subarbolIzdo());
                if (actual->subarbolDcho()) cola.push(actual->subarbolDcho());
            }
        }
    }
    return *this;
}

#endif // ARBOL_BIN



/* ===================== GLOSARY ==============================*/
// const: Garantiza que este método no puede modificar el estado del objeto. Solo realiza operaciones de lectura.
//