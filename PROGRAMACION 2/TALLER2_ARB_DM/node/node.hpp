#ifndef NODEBIN
#define NODEBIN

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

template <typename T>
class Nodo {
private:
    T dato;                   // Dato almacenado en el nodo
    Nodo<T>* izquierdo;       // Puntero al hijo izquierdo
    Nodo<T>* derecho;         // Puntero al hijo derecho

public:
    // Constructor
    Nodo(const T& val) : dato(val), izquierdo(NULL), derecho(NULL) {} //onstructor que inicializa el nodo con un valor (val) del tipo T.

    // Devuelve el dato almacenado (modificable)
    T& valorNodo() { return dato; }

    // Devuelve el dato almacenado (constante)
    const T& valorNodo() const { return dato; }

    // Métodos de acceso a subárboles
    Nodo<T>* subarbolIzdo() const { return izquierdo; }
    Nodo<T>* subarbolDcho() const { return derecho; }

    // Métodos de modificación de subárboles
    void setIzquierdo(Nodo<T>* izq) { izquierdo = izq; }
    void setDerecho(Nodo<T>* der) { derecho = der; }

    // Método para visualizar el nodo
    void visitar() const { std::cout << dato << " "; }


};

    // Reconstruye árbol desde INORDEN + PREORDEN
    Nodo<string>* reconstruirDesdePreIn(vector<string>& inorden, vector<string>& preorden, int inStart, int inEnd, int& preIndex, unordered_map<string, int>& inMap) {
        if (inStart > inEnd) return NULL; //Si el rango de índices en inorden es inválido (cuando inStart > inEnd), significa que no hay más nodos que procesar. Retorna NULL para indicar que el subárbol está vacío.
    
        string valor = preorden[preIndex++]; //Se incrementa preIndex para preparar el siguiente elemento para futuras llamadas recursivas
        Nodo<string>* nodo = new Nodo<string>(valor);//Crea un nuevo nodo (Nodo<string>) con el valor obtenido.
        int inIndex = inMap[valor];
    
        nodo->setIzquierdo(reconstruirDesdePreIn(inorden, preorden, inStart, inIndex - 1, preIndex, inMap));
        nodo->setDerecho(reconstruirDesdePreIn(inorden, preorden, inIndex + 1, inEnd, preIndex, inMap));
    
        return nodo;
    };
    
    // Reconstruye árbol desde INORDEN + POSTORDEN
        Nodo<string>* reconstruirDesdePostIn(vector<string>& inorden, vector<string>& postorden, int inStart, int inEnd, int& postIndex, unordered_map<string, int>& inMap) {
        if (inStart > inEnd) return NULL; //Caso base de la recursión: Si el rango de índices en inorden es inválido (es decir, inStart > inEnd), no hay más nodos que procesar. Devuelve null
    
        string valor = postorden[postIndex--]; //Obtiene el valor del nodo raíz actual desde el recorrido postorden. La raíz siempre es el último elemento del rango actual en el recorrido postorden.Decrementa postIndex para que apunte al siguiente nodo raíz en la próxima llamada recursiva.
        Nodo<string>* nodo = new Nodo<string>(valor);//Crea un nuevo nodo del árbol binario con el valor obtenido de postorden.
        int inIndex = inMap[valor];
        /*Busca el índice del valor actual en el recorrido inorden utilizando el mapa inMap. Este índice divide el recorrido inorden en dos partes:Subárbol izquierdo: Desde inStart hasta inIndex - 1.Subárbol derecho: Desde inIndex + 1 hasta inEnd.*/
    
        nodo->setDerecho(reconstruirDesdePostIn(inorden, postorden, inIndex + 1, inEnd, postIndex, inMap));
        nodo->setIzquierdo(reconstruirDesdePostIn(inorden, postorden, inStart, inIndex - 1, postIndex, inMap));
    
        return nodo;
    };
    

#endif