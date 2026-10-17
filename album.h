#ifndef ALBUM_H
#define ALBUM_H

#include "cadena.h"
#include "fecha.h"
#include "foto.h"
#include "listadinamica.h"

class Album {
private:
    Cadena nombre;
    Cadena descripcion;
    Fecha fechaCreacion;
    Cadena privacidad;
    ListaDinamica<Foto> fotos;

public:
    Album();
    Album(Cadena nom, Cadena desc, Fecha fecha, Cadena priv);

    void agregarFoto(const Foto& foto);
    const Cadena& getNombre() const;
    const Cadena& getPrivacidad() const;
    int getCantidadFotos() const;
    Foto& obtenerFoto(int indice);

    size_t obtenerMemoriaConsumida() const;
    size_t memoriaDinamica() const { return obtenerMemoriaConsumida(); }
};

#endif