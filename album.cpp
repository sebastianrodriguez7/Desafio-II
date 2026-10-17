#include "album.h"

Album::Album() : nombre(), descripcion(), fechaCreacion(), privacidad(), fotos() {}

Album::Album(Cadena nom, Cadena desc, Fecha fecha, Cadena priv)
    : nombre(nom), descripcion(desc), fechaCreacion(fecha), privacidad(priv), fotos() {}

void Album::agregarFoto(const Foto& foto) {
    fotos.agregar(foto);
}

const Cadena& Album::getNombre() const { return nombre; }
const Cadena& Album::getPrivacidad() const { return privacidad; }
int Album::getCantidadFotos() const { return fotos.getTamano(); }
Foto& Album::obtenerFoto(int indice) { return fotos.obtener(indice); }

size_t Album::obtenerMemoriaConsumida() const {
    size_t total = sizeof(*this) + nombre.memoriaDinamica() +
                   descripcion.memoriaDinamica() + privacidad.memoriaDinamica() +
                   fotos.memoriaDinamica();
    return total;
}