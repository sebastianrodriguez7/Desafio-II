#ifndef GESTIONARCHIVOS_H
#define GESTIONARCHIVOS_H

#include "usuario.h"
#include "matrizadyacente.h"
#include "listadinamica.h"
class GestionArchivos
{
public:
    static bool cargarUsuarios(const char* ruta, ListaDinamica<Usuario>& usuarios);
    static bool cargarAmistades(const char* ruta, ListaDinamica<Usuario>& usuarios, MatrizAdyacente& matriz);
    static bool cargarAlbumesYFotos(const char* ruta, ListaDinamica<Usuario>& usuarios);
    static bool cargarMensajes(const char* ruta, ListaDinamica<Usuario>& usuarios);
    static bool cargarSolicitudes(const char* ruta, ListaDinamica<Usuario>& usuarios);
    static bool guardarMensaje(const char* ruta, const Mensaje& msj);
    static bool guardarSolicitud(const char* ruta, const SolicitudAmistad& sol);
};

#endif // GESTIONARCHIVOS_H
