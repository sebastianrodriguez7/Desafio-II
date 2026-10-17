#ifndef FOTO_H
#define FOTO_H

#include "cadena.h"
#include "fecha.h"

class Foto {
private:
    Fecha fechaCaptura;
    size_t tamanoBytes;
    Cadena rutaArchivo;
    int conteoLikes;

public:
    Foto();
    Foto(Fecha fecha, size_t tamano, Cadena ruta, int likes = 0);

    const Cadena& getRuta() const;
    int getLikes() const;
    void darLike();
    void quitarLike();

    void mostrarInfo() const;
    size_t obtenerMemoriaConsumida() const;
    size_t memoriaDinamica() const { return obtenerMemoriaConsumida(); }
};

#endif