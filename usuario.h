#ifndef USUARIO_H
#define USUARIO_H

#include "cadena.h"
#include "fecha.h"
#include "album.h"
#include "mensaje.h"
#include "solicitudamistad.h"
#include "listadinamica.h"

class Usuario {
private:
    int id;
    Cadena nombreUsuario;
    Cadena password;
    Cadena nombreCompleto;
    Fecha fechaNacimiento;
    Cadena correo;
    Cadena rutaFotoPerfil;
    Cadena ciudad;
    Cadena pais;
    Fecha fechaInscripcion;

    ListaDinamica<int> idsAmigos;
    ListaDinamica<Album> albumes;
    ListaDinamica<Mensaje> mensajesRecibidos;
    ListaDinamica<Mensaje> mensajesEnviados;
    ListaDinamica<SolicitudAmistad> solicitudes;

public:
    Usuario();
    Usuario(int id, Cadena username, Cadena pwd, Cadena nombre, Fecha fnac, Cadena email, Cadena foto, Cadena cudad, Cadena pais, Fecha finsc);
    size_t memoriaDinamica() const { return obtenerMemoriaConsumida(); }
    int getId() const;
    const Cadena& getNombreUsuario() const;
    bool verificarPassword(const Cadena& pwd) const;
    const Cadena& getRutaFotoPerfil() const;

    void agregarAmigo(int idAmigo);
    bool esAmigoDe(int idAmigo) const;
    const ListaDinamica<int>& getAmigos() const;

    void agregarAlbum(const Album& album);
    ListaDinamica<Album>& getAlbumes();

    void recibirMensaje(const Mensaje& msj);
    void enviarMensaje(const Mensaje& msj);
    ListaDinamica<Mensaje>& getMensajesRecibidos();

    void agregarSolicitud(const SolicitudAmistad& sol);
    ListaDinamica<SolicitudAmistad>& getSolicitudes();

    size_t obtenerMemoriaConsumida() const;
};


#endif