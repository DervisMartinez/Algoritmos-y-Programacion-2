#include <iostream>
#include <string>
#include <sstream>
#include "../lib/Node/DoublyNode.hpp"

using namespace std;

template <typename T>
class CircularDoublyList {
private:
    DoublyNode<T>* head; // Apuntador a la Cabezera de la Lista 
    int size;   // Longitud De La Lista 

public:
    CircularDoublyList() : head(nullptr), size(0) {}   //Constructor

    ~CircularDoublyList() {   // Destructor
        while (head != nullptr) {
            removeNode(head);
        }
    }
/*Añade al Final de La lista */
    void addToEnd(T data) {
        DoublyNode<T>* newNode = new DoublyNode<T>(data);
        if (head == nullptr) {
            head = newNode;
            head->setNext(head);
            head->setPrev(head);
        } else {
            DoublyNode<T>* tail = head->getPrev();
            tail->setNext(newNode);
            newNode->setPrev(tail);
            newNode->setNext(head);
            head->setPrev(newNode);
        }
        size++;
    }
 /*Remueve un Nodo */
    void removeNode(DoublyNode<T>* node) {
        if (node == nullptr || size == 0) return;
        if (size == 1) {  // si la longitud es igual a 1
            delete head;  // elimina el nodo Cabecera 
            head = nullptr;  // El Nodo Cabecera ahora apunta a NULL
        } else {
            DoublyNode<T>* prevNode = node->getPrev();
            DoublyNode<T>* nextNode = node->getNext();
            prevNode->setNext(nextNode);
            nextNode->setPrev(prevNode);
            if (node == head) head = nextNode;
            delete node;
        }
        size--;
    }

    DoublyNode<T>* getHead() const { return head; }
    int getSize() const { return size; }
};

/* 

libreria CircularDoublyList derivada de la libreria SinglyList 
Para simplificar el ejercicio ,sin necesidad de usar Listas de Listas 

*/