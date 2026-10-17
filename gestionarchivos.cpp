#include "gestionarchivos.h"
#include "medidorrecursos.h"
#include <fstream>
#include <cstdio>

bool GestionArchivos::cargarUsuarios(const char* ruta, ListaDinamica<Usuario>& usuarios) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) return false;

    char linea[512];
    int idCount = 0;
    while (archivo.getline(linea, 512)) {
        MedidorRecursos::registrarIteracion();
        if (linea[0] == '\0') continue;

        char username[64], password[64], nombre[128], correo[128], rutaFoto[256], ciudad[64], pais[64];
        int parseados = sscanf(linea, "%63[^;];%63[^;];%127[^;];%127[^;];%255[^;];%63[^;];%63[^\n]",
                               username, password, nombre, correo, rutaFoto, ciudad, pais);

        Fecha fechaDefecto;
        if (parseados >= 2) {
            Usuario nuevo(idCount, Cadena(username), Cadena(password), Cadena(nombre), fechaDefecto,
                          Cadena(correo), Cadena(rutaFoto), Cadena(ciudad), Cadena(pais), fechaDefecto);
            usuarios.agregar(nuevo);
            idCount++;
        }
    }
    archivo.close();
    return true;
}

bool GestionArchivos::cargarAmistades(const char* ruta, ListaDinamica<Usuario>& usuarios, MatrizAdyacente& matriz) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) return false;

    int u1, u2;
    while (archivo >> u1 >> u2) {
        MedidorRecursos::registrarIteracion();
        matriz.conectar(u1, u2);
        if (u1 < usuarios.getTamano() && u2 < usuarios.getTamano()) {
            usuarios.obtener(u1).agregarAmigo(u2);
            usuarios.obtener(u2).agregarAmigo(u1);
        }
    }
    archivo.close();
    return true;
}

bool GestionArchivos::cargarAlbumesYFotos(const char* ruta, ListaDinamica<Usuario>& usuarios) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) return false;

    int idUsuario;
    char buffer[512];

    while (archivo >> idUsuario) {
        MedidorRecursos::registrarIteracion();
        archivo.ignore();

        char nomAlbum[64], descAlbum[128], privAlbum[16], rutaFoto[256];
        int tamanoFoto, likesFoto;

        archivo.getline(nomAlbum, 64, ';');
        archivo.getline(descAlbum, 128, ';');
        archivo.getline(buffer, 64, ';');
        archivo.getline(privAlbum, 16, ';');
        archivo.getline(rutaFoto, 256, ';');
        archivo.getline(buffer, 64, ';');
        archivo >> tamanoFoto;
        archivo.ignore();
        archivo >> likesFoto;

        if (idUsuario < usuarios.getTamano()) {
            Usuario& user = usuarios.obtener(idUsuario);
            ListaDinamica<Album>& albumes = user.getAlbumes();

            Album* albumActual = nullptr;
            Cadena cNomAlbum(nomAlbum);
            for (int i = 0; i < albumes.getTamano(); ++i) {
                MedidorRecursos::registrarIteracion();
                if (albumes.obtener(i).getNombre() == cNomAlbum) {
                    albumActual = &albumes.obtener(i);
                    break;
                }
            }

            if (!albumActual) {
                Fecha fechaCreacionAlbum;
                Cadena cDescAlbum(descAlbum);
                Cadena cPrivAlbum(privAlbum);

                Album nuevoAlbum(cNomAlbum, cDescAlbum, fechaCreacionAlbum, cPrivAlbum);
                user.agregarAlbum(nuevoAlbum);
                albumActual = &user.getAlbumes().obtener(user.getAlbumes().getTamano() - 1);
            }

            Fecha fechaCapturaFoto;
            Cadena cRutaFoto(rutaFoto);
            Foto nuevaFoto(fechaCapturaFoto, tamanoFoto, cRutaFoto, likesFoto);
            albumActual->agregarFoto(nuevaFoto);
        }
    }
    archivo.close();
    return true;
}

bool GestionArchivos::cargarMensajes(const char* ruta, ListaDinamica<Usuario>& usuarios) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) return false;

    char linea[512];
    while (archivo.getline(linea, 512)) {
        MedidorRecursos::registrarIteracion();
        if (linea[0] == '\0') continue;

        char emisor[64], receptor[64], texto[512], fechaStr[32];
        char estadoStr[16];
        int parseados = sscanf(linea, "%63[^;];%63[^;];%511[^;];%31[^;];%15[^\n]",
                               emisor, receptor, texto, fechaStr, estadoStr);

        if (parseados >= 4) {
            int d = 1, m = 1, a = 2026, h = 0, min = 0;
            sscanf(fechaStr, "%d-%d-%d %d:%d", &d, &m, &a, &h, &min);
            Fecha fechaEnvio(d, m, a, h, min);

            bool leido = false;
            if (parseados >= 5) {
                if (Cadena(estadoStr) == Cadena("true") || Cadena(estadoStr) == Cadena("1")) {
                    leido = true;
                }
            }

            Mensaje msj(Cadena(emisor), Cadena(receptor), Cadena(texto), fechaEnvio, leido);

            for (int i = 0; i < usuarios.getTamano(); ++i) {
                MedidorRecursos::registrarIteracion();
                if (usuarios.obtener(i).getNombreUsuario() == Cadena(receptor)) {
                    usuarios.obtener(i).recibirMensaje(msj);
                }
                if (usuarios.obtener(i).getNombreUsuario() == Cadena(emisor)) {
                    usuarios.obtener(i).enviarMensaje(msj);
                }
            }
        }
    }
    archivo.close();
    return true;
}

bool GestionArchivos::cargarSolicitudes(const char* ruta, ListaDinamica<Usuario>& usuarios) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) return false;

    char linea[512];
    while (archivo.getline(linea, 512)) {
        MedidorRecursos::registrarIteracion();
        if (linea[0] == '\0') continue;

        char emisor[64], receptor[64], estado[32];
        int parseados = sscanf(linea, "%63[^;];%63[^;];%31[^\n]", emisor, receptor, estado);
        if (parseados >= 3) {
            Fecha fechaDefecto;
            SolicitudAmistad sol(Cadena(emisor), Cadena(receptor), Cadena(estado), fechaDefecto);

            for (int i = 0; i < usuarios.getTamano(); ++i) {
                MedidorRecursos::registrarIteracion();
                if (usuarios.obtener(i).getNombreUsuario() == Cadena(receptor)) {
                    usuarios.obtener(i).agregarSolicitud(sol);
                    break;
                }
            }
        }
    }
    archivo.close();
    return true;
}

bool GestionArchivos::guardarMensaje(const char* ruta, const Mensaje& msj) {
    std::ofstream archivo(ruta, std::ios::app);
    if (!archivo.is_open()) return false;

    const Fecha& f = msj.getFechaEnvio();
    archivo << msj.getEmisor().c_str() << ";"
            << msj.getReceptor().c_str() << ";"
            << msj.getTexto().c_str() << ";"
            << "15-10-2026 12:00;"
            << (msj.estaLeido() ? "true" : "false") << "\n";
    archivo.close();
    return true;
}

bool GestionArchivos::guardarSolicitud(const char* ruta, const SolicitudAmistad& sol) {
    std::ofstream archivo(ruta, std::ios::app);
    if (!archivo.is_open()) return false;
    archivo << sol.getEmisor().c_str() << ";" << sol.getReceptor().c_str() << ";" << sol.getEstado().c_str() << "\n";
    archivo.close();
    return true;
}
