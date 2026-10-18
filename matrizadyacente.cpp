#include "matrizadyacente.h"
#include "medidorrecursos.h"

MatrizAdyacente::MatrizAdyacente(int capacidadInicial) {
    capacidad = capacidadInicial;
    matriz = new bool*[capacidad];
    for (int i = 0; i < capacidad; ++i) {
        MedidorRecursos::registrarIteracion();
        matriz[i] = new bool[capacidad];
        for (int j = 0; j < capacidad; ++j) {
            MedidorRecursos::registrarIteracion();
            matriz[i][j] = false;
        }
    }
}

MatrizAdyacente::~MatrizAdyacente() {
    for (int i = 0; i < capacidad; ++i) {
        delete[] matriz[i];
    }
    delete[] matriz;
}

void MatrizAdyacente::redimensionar(int nuevaCapacidad) {
    bool** nuevaMatriz = new bool*[nuevaCapacidad];
    for (int i = 0; i < nuevaCapacidad; ++i) {
        MedidorRecursos::registrarIteracion();
        nuevaMatriz[i] = new bool[nuevaCapacidad];
        for (int j = 0; j < nuevaCapacidad; ++j) {
            MedidorRecursos::registrarIteracion();
            if (i < capacidad && j < capacidad) {
                nuevaMatriz[i][j] = matriz[i][j];
            } else {
                nuevaMatriz[i][j] = false;
            }
        }
    }

    for (int i = 0; i < capacidad; ++i) {
        delete[] matriz[i];
    }
    delete[] matriz;

    matriz = nuevaMatriz;
    capacidad = nuevaCapacidad;
}

void MatrizAdyacente::conectar(int idUsuario1, int idUsuario2) {
    if (idUsuario1 < 0 || idUsuario2 < 0) {
        return;
    }

    int maxId = (idUsuario1 > idUsuario2) ? idUsuario1 : idUsuario2;
    if (maxId >= capacidad) {
        redimensionar(maxId + 50);
    }

    matriz[idUsuario1][idUsuario2] = true;
    matriz[idUsuario2][idUsuario1] = true;
}

bool MatrizAdyacente::sonAmigos(int idUsuario1, int idUsuario2) const {
    if (idUsuario1 >= capacidad || idUsuario2 >= capacidad) return false;
    return matriz[idUsuario1][idUsuario2];
}

int MatrizAdyacente::contarAmigosEnComun(int idUsuario1, int idUsuario2, int totalUsuarios) const {
    int enComun = 0;
    for (int i = 0; i < totalUsuarios; ++i) {
        MedidorRecursos::registrarIteracion();
        if (i < capacidad && matriz[idUsuario1][i] && matriz[idUsuario2][i]) {
            enComun++;
        }
    }
    return enComun;
}

size_t MatrizAdyacente::obtenerMemoriaConsumida() const {
    size_t mem = sizeof(*this);
    mem += sizeof(bool*) * capacidad;
    mem += (sizeof(bool) * capacidad) * capacidad;
    return mem;
}