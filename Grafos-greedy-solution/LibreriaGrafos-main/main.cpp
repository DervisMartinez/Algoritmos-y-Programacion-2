#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "grafoNoDirigido.hpp"

using namespace std;

list<string> encontrarConjuntoDominante(GrafoNoDirigido<string>& grafo) {
    list<string> vertices = grafo.vertices();
    list<string> conjuntoDominante;
    vector<bool> cubiertos(grafo.getNVertices(), false);
    vector<string> mapeo;
    
    // Mapear los vértices a índices numéricos
    GrafoNoDirigido<int> grafoMapeado = grafo.mapear(&mapeo);
    
    // Ordenar los vértices por grado descendente (heurística greedy)
    vertices.sort([&grafo](const string& a, const string& b) {
        return grafo.vecinos(a).size() > grafo.vecinos(b).size();
    });
    
    for (const string& pueblo : vertices) {
        int idx = grafo.buscarMapeo(mapeo, pueblo, grafo.getNVertices());
        if (!cubiertos[idx]) {
            conjuntoDominante.push_back(pueblo);
            cubiertos[idx] = true;
            
            // Marcar todos los vecinos como cubiertos
            list<string> vecinos = grafo.vecinos(pueblo);
            for (const string& vecino : vecinos) {
                int vecinoIdx = grafo.buscarMapeo(mapeo, vecino, grafo.getNVertices());
                cubiertos[vecinoIdx] = true;
            }
        }
    }
    
    // Ordenar alfabéticamente el resultado
    conjuntoDominante.sort();
    
    return conjuntoDominante;
}

int main() {
    GrafoNoDirigido<string> grafo;
    string pueblo1, pueblo2;
    
    // Leer la entrada y construir el grafo
    while (cin >> pueblo1 >> pueblo2) {
        grafo.agregarVertice(pueblo1);
        grafo.agregarVertice(pueblo2);
        grafo.agregarArcoND(pueblo1, pueblo2);
    }
    
    // Encontrar el conjunto dominante mínimo
    list<string> conjuntoDominante = encontrarConjuntoDominante(grafo);
    
    // Imprimir el resultado
    bool primero = true;
    for (const string& pueblo : conjuntoDominante) {
        if (!primero) {
            cout << " ";
        }
        cout << pueblo;
        primero = false;
    }
    cout << endl;
    
    return 0;
}