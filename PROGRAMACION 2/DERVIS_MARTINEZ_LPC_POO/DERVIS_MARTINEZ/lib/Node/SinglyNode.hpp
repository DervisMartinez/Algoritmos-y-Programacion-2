#ifndef SINGLYNODE_H
#define SINGLYNODE_H

#include <cstddef>

template <typename T>
class SinglyNode {
protected:
    T data; /*dato almacenado en el nodo*/
    SinglyNode<T>* next; /*apuntador al nodo siguiente*/

public:
    /*constructores*/
    SinglyNode() : data(T()), next(NULL) {} /*constructor por defecto, inicializa el dato a su valor por defecto y el puntero a null*/
    
    SinglyNode(T newData) : data(newData), next(NULL) {} /*coloca puntero a null*/
    
    SinglyNode(T newData, SinglyNode<T>* newNext) : 
        data(newData), next(newNext) {} /*asigna puntero*/

    /*getters*/
    T getData() const { return this->data; } /*devuelve el contenido*/
    
    SinglyNode<T>* getNext() const { return this->next; } /*devuelve el puntero*/

    /*setters*/
    void setData(T newData) { this->data = newData; } /*modifica contenido*/
    
    void setNext(SinglyNode<T>* newNext) { this->next = newNext; } /*modifica el puntero*/
};

#endif