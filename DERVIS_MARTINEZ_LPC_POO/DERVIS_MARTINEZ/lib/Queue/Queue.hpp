#ifndef QUEUE_H
#define QUEUE_H

#include "../Node/SinglyNode.hpp"
#include "../Stack/Stack.hpp"  /* para la funcion reverse() */
#include <cstddef>  /* para null */

template <typename T>
class Queue {
private:
    /*atributos*/
    SinglyNode<T>* front; /*apuntador al primer elemento de la cola (para remover)*/
    SinglyNode<T>* last;  /*apuntador al último elemento de la cola (para añadir)*/
    int length;             /*número de elementos en la cola*/

    /*metodos privados auxiliares*/
    bool isNullNode(SinglyNode<T>* node) const {
        return node == NULL; /*verifica si un nodo es nulo, O(1)*/
    }

    void addUpdateLast(SinglyNode<T>* newLast) {
        last = newLast;
        length++; /*actualiza el puntero al nuevo último elemento y aumenta el tamaño*/
    }

    void removeUpdateFront(SinglyNode<T>* newFront) {
        front = newFront;
        length--; /*actualiza el puntero al nuevo primer elemento y disminuye el tamaño*/
    }

    void uniqueElementUpdate(SinglyNode<T>* newFront) {
        front = last = newFront;
        length = 1; /*actualiza el puntero al nuevo primer elemento y establece el tamaño a 1*/
    }

    void emptyQueueUpdate() {
        front = last = NULL;
        length = 0; /*actualiza el estado cuando la cola queda vacía*/
    }

public:
    /*constructor*/
    Queue() : front(NULL), last(NULL), length(0) {} /*inicializa una cola vacía*/

    /*constructor de copia*/
    Queue(const Queue<T>& other) : front(NULL), last(NULL), length(0) {
        if (!other.isEmpty()) {
            SinglyNode<T>* current = other.front;
            while (current != NULL) {
                this->enqueue(current->getData());
                current = current->getNext();
            }
        } /*crea una nueva cola con distintos apuntadores de memoria*/
    }

    /*destructor*/
    ~Queue() {
        clear(); /*libera toda la memoria de los nodos*/
    }

    /*getters*/
    int getLength() const {
        return length; /*obtiene el tamaño actual de la cola*/
    }

    bool isEmpty() const {
        return length == 0; /*verifica si la cola está vacía*/
    }

    T getFront() const {
        if (isEmpty()) {
            return T(); /*retorna valor por defecto si la cola esta vacia*/
        }
        return front->getData(); /*obtiene el elemento al frente sin removerlo*/
    }

    /*operaciones básicas*/
    void enqueue(T element) {
        SinglyNode<T>* newNode = new SinglyNode<T>(element);
        if (isEmpty()) {
            uniqueElementUpdate(newNode);
        } else {
            last->setNext(newNode);
            addUpdateLast(newNode);
        } /*añade un elemento al final de la cola, O(1)*/
    }

    void dequeue() {
        if (!isEmpty()) {
            SinglyNode<T>* nodeToRemove = front;
            removeUpdateFront(front->getNext());
            delete nodeToRemove;

            if (isEmpty()) {
                emptyQueueUpdate();
            }
        } /*remueve el elemento al frente de la cola, O(1)*/
    }

    void clear() {
        while (!isEmpty()) {
            dequeue();  /*reutilizamos dequeue() para liberar memoria*/
        } /*vacía toda la cola, O(n)*/
    }

    /*operaciones adicionales*/
    Queue<T>* clone() const {
        return new Queue<T>(*this);  /*usa el constructor de copia para crear una copia independiente de la cola*/
    }

    void reverse() {
        if (isEmpty() || length == 1) {
            return;  /*no hay nada que invertir*/
        }

        Stack<T> tempStack;
        while (!isEmpty()) {
            tempStack.push(getFront());
            dequeue();
        }

        while (!tempStack.isEmpty()) {
            enqueue(tempStack.pop());
        } /*invierte el orden de los elementos en la cola*/
    }
};

#endif
