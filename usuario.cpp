#include "usuario.h"
#include "medidorrecursos.h"

Usuario::Usuario() : id(-1) {}

Usuario::Usuario(int id, Cadena username, Cadena pwd, Cadena nombre, Fecha fnac, Cadena email, Cadena foto, Cadena cudad, Cadena pais, Fecha finsc)
    : id(id), nombreUsuario(username), password(pwd), nombreCompleto(nombre), fechaNacimiento(fnac),
    correo(email), rutaFotoPerfil(foto), ciudad(cudad), pais(pais), fechaInscripcion(finsc) {}

int Usuario::getId() const { return id; }
const Cadena& Usuario::getNombreUsuario() const { return nombreUsuario; }

bool Usuario::verificarPassword(const Cadena& pwd) const {
    return password == pwd;
}

const Cadena& Usuario::getRutaFotoPerfil() const { return rutaFotoPerfil; }

void Usuario::agregarAmigo(int idAmigo) {
    if (!esAmigoDe(idAmigo)) {
        idsAmigos.agregar(idAmigo);
    }
}

bool Usuario::esAmigoDe(int idAmigo) const {
    for (int i = 0; i < idsAmigos.getTamano(); ++i) {
        MedidorRecursos::registrarIteracion();
        if (idsAmigos.obtener(i) == idAmigo) return true;
    }
    return false;
}

const ListaDinamica<int>& Usuario::getAmigos() const { return idsAmigos; }
void Usuario::agregarAlbum(const Album& album) { albumes.agregar(album); }
ListaDinamica<Album>& Usuario::getAlbumes() { return albumes; }

void Usuario::recibirMensaje(const Mensaje& msj) { mensajesRecibidos.agregar(msj); }
void Usuario::enviarMensaje(const Mensaje& msj) { mensajesEnviados.agregar(msj); }
ListaDinamica<Mensaje>& Usuario::getMensajesRecibidos() { return mensajesRecibidos; }

void Usuario::agregarSolicitud(const SolicitudAmistad& sol) { solicitudes.agregar(sol); }
ListaDinamica<SolicitudAmistad>& Usuario::getSolicitudes() { return solicitudes; }

size_t Usuario::obtenerMemoriaConsumida() const {
    size_t total = sizeof(*this) +
                   nombreUsuario.memoriaDinamica() +
                   password.memoriaDinamica() +
                   nombreCompleto.memoriaDinamica() +
                   correo.memoriaDinamica() +
                   rutaFotoPerfil.memoriaDinamica() +
                   ciudad.memoriaDinamica() +
                   pais.memoriaDinamica() +
                   idsAmigos.memoriaDinamica() +
                   albumes.memoriaDinamica() +
                   mensajesRecibidos.memoriaDinamica() +
                   mensajesEnviados.memoriaDinamica() +
                   solicitudes.memoriaDinamica();
    return total;
}