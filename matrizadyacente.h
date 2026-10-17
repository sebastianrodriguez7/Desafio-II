#ifndef MATRIZADYACENTE_H
#define MATRIZADYACENTE_H

#include <cstddef>

class MatrizAdyacente {
private:
    bool** matriz;
    int capacidad;

    void redimensionar(int nuevaCapacidad);

public:
    MatrizAdyacente(int capacidadInicial = 100);
    ~MatrizAdyacente();

    void conectar(int idUsuario1, int idUsuario2);
    bool sonAmigos(int idUsuario1, int idUsuario2) const;
    int contarAmigosEnComun(int idUsuario1, int idUsuario2, int totalUsuarios) const;

    size_t obtenerMemoriaConsumida() const;
};

#endif