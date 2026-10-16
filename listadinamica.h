#ifndef LISTADINAMICA_H
#define LISTADINAMICA_H

#include <cstddef>
#include "medidorrecursos.h"

template <typename T>
size_t memoriaDinamicaDe(const T& x) { return x.memoriaDinamica(); }
inline size_t memoriaDinamicaDe(const int&) { return 0; }
template <typename T>
class ListaDinamica {
private:
    T* elementos;
    int tamano;
    int capacidad;

    void redimensionar(int nuevaCapacidad) {
        T* nuevo = new T[nuevaCapacidad];
        for (int i = 0; i < tamano; ++i) {
            MedidorRecursos::registrarIteracion();
            nuevo[i] = elementos[i];
        }
        delete[] elementos;
        elementos = nuevo;
        capacidad = nuevaCapacidad;
    }

    void mezclar(T* aux, int ini, int med, int fin) {
        int i = ini, j = med + 1, k = ini;
        while (i <= med && j <= fin) {
            MedidorRecursos::registrarIteracion();
            if (elementos[j] < elementos[i]) aux[k++] = elementos[j++];
            else                             aux[k++] = elementos[i++];
        }
        while (i <= med) { MedidorRecursos::registrarIteracion(); aux[k++] = elementos[i++]; }
        while (j <= fin) { MedidorRecursos::registrarIteracion(); aux[k++] = elementos[j++]; }
        for (int x = ini; x <= fin; ++x) { MedidorRecursos::registrarIteracion(); elementos[x] = aux[x]; }
    }

    void mergeSort(T* aux, int ini, int fin) {
        if (ini >= fin) return;
        int med = ini + (fin - ini) / 2;
        mergeSort(aux, ini, med);
        mergeSort(aux, med + 1, fin);
        if (!(elementos[med + 1] < elementos[med])) return;
        mezclar(aux, ini, med, fin);
    }

public:
    ListaDinamica() : elementos(nullptr), tamano(0), capacidad(0) {}

    ListaDinamica(const ListaDinamica& otra) : elementos(nullptr), tamano(otra.tamano), capacidad(otra.capacidad) {
        if (capacidad > 0) {
            elementos = new T[capacidad];
            for (int i = 0; i < tamano; ++i) {
                MedidorRecursos::registrarIteracion();
                elementos[i] = otra.elementos[i];
            }
        }
    }

    ListaDinamica& operator=(const ListaDinamica& otra) {
        if (this != &otra) {
            delete[] elementos;
            elementos = nullptr;
            tamano = otra.tamano;
            capacidad = otra.capacidad;
            if (capacidad > 0) {
                elementos = new T[capacidad];
                for (int i = 0; i < tamano; ++i) {
                    MedidorRecursos::registrarIteracion();
                    elementos[i] = otra.elementos[i];
                }
            }
        }
        return *this;
    }

    ~ListaDinamica() { delete[] elementos; }

    void reservar(int minimo) {
        if (minimo > capacidad) redimensionar(minimo);
    }

    void agregar(const T& elemento) {
        if (tamano == capacidad) redimensionar(capacidad == 0 ? 4 : capacidad * 2);
        elementos[tamano++] = elemento;
    }

    T& obtener(int indice) { return elementos[indice]; }
    const T& obtener(int indice) const { return elementos[indice]; }
    T& operator[](int indice) { return elementos[indice]; }
    const T& operator[](int indice) const { return elementos[indice]; }

    void eliminarEn(int indice) {
        if (indice < 0 || indice >= tamano) return;
        for (int i = indice; i < tamano - 1; ++i) {
            MedidorRecursos::registrarIteracion();
            elementos[i] = elementos[i + 1];
        }
        tamano--;
    }

    int getTamano() const { return tamano; }
    int getCapacidad() const { return capacidad; }

    bool estaOrdenada() const {
        for (int i = 1; i < tamano; ++i) {
            MedidorRecursos::registrarIteracion();
            if (elementos[i] < elementos[i - 1]) return false;
        }
        return true;
    }

    void ordenar() {
        if (tamano < 2 || estaOrdenada()) return;
        T* aux = new T[tamano];
        mergeSort(aux, 0, tamano - 1);
        delete[] aux;
    }

    size_t memoriaDinamica() const {
        size_t total = sizeof(T) * (size_t)capacidad;
        for (int i = 0; i < tamano; ++i) total += memoriaDinamicaDe(elementos[i]);
        return total;
    }
};

#endif