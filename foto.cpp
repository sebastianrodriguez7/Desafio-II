#include "foto.h"

Foto::Foto() : fechaCaptura(), tamanoBytes(0), rutaArchivo(), conteoLikes(0) {}

Foto::Foto(Fecha fecha, size_t tamano, Cadena ruta, int likes)
    : fechaCaptura(fecha), tamanoBytes(tamano), rutaArchivo(ruta), conteoLikes(likes) {}

const Cadena& Foto::getRuta() const { return rutaArchivo; }
int Foto::getLikes() const { return conteoLikes; }

void Foto::darLike() { conteoLikes++; }
void Foto::quitarLike() { if (conteoLikes > 0) conteoLikes--; }

void Foto::mostrarInfo() const {

}

size_t Foto::obtenerMemoriaConsumida() const {
    return sizeof(*this) + rutaArchivo.memoriaDinamica();
}