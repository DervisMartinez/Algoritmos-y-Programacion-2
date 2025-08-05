#ifndef DOUBLYNODE_H
#define DOUBLYNODE_H

#include "Node/SinglyNode.hpp"
#include <cstddef>

template <typename T>
class DoublyNode : public SinglyNode<T> {
protected:
    DoublyNode<T>* prev; /*puntero al nodo anterior*/

public:
    /* Constructores */
    DoublyNode(T newData) : 
        SinglyNode<T>(newData), prev(NULL) {} /*constructor básico con solo dato*/
    
    DoublyNode(T newData, DoublyNode<T>* newPrev, DoublyNode<T>* newNext) : 
        SinglyNode<T>(newData, static_cast<SinglyNode<T>*>(newNext)), prev(newPrev) {} /*constructor completo*/

    /* Getters */
    DoublyNode<T>* getPrev() const {
        return this->prev; /*devuelve puntero al nodo anterior*/
    }

    DoublyNode<T>* getNext() const {
        return static_cast<DoublyNode<T>*>(SinglyNode<T>::getNext()); /*devuelve puntero al siguiente nodo (sobrescribe el de SinglyNode)*/
    }

    /* Setters */
    void setPrev(DoublyNode<T>* newPrev) {
        this->prev = newPrev; /*establece puntero al nodo anterior*/
    }
};

#endif