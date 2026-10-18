#include "gestionarchivos.h"
#include "medidorrecursos.h"
#include <fstream>
#include <iostream>


namespace {

const int LINEA_MAX = 2048;

int leerCampo(const char* linea, int pos, char* destino, int max) {
    int k = 0;
    while (linea[pos] != ';' && linea[pos] != '\0' && linea[pos] != '\r' && linea[pos] != '\n') {
        MedidorRecursos::registrarIteracion();
        if (k < max - 1) destino[k++] = linea[pos];
        pos++;
    }
    destino[k] = '\0';
    return (linea[pos] == ';') ? pos + 1 : -1;
}

void copiarRango(const char* origen, int desde, int hasta, char* destino, int max) {
    int k = 0;
    for (int i = desde; i < hasta && k < max - 1; ++i) {
        MedidorRecursos::registrarIteracion();
        destino[k++] = origen[i];
    }
    destino[k] = '\0';
}

int parsearEntero(const char* s) {
    int i = 0, v = 0;
    while (s[i] == ' ') i++;
    while (s[i] >= '0' && s[i] <= '9' && i < 10) { v = v * 10 + (s[i] - '0'); i++; }
    return v;
}

bool leerFechaFlexible(const char* texto, Fecha& f) {
    if (f.leerISO(texto)) return true;
    if (f.leerLatino(texto)) return true;
    int n = 0;
    while (texto[n] != '\0' && n < 10) n++;
    if (n == 10 && texto[2] == '-' && texto[5] == '-') {
        char tmp[32];
        tmp[0] = texto[6]; tmp[1] = texto[7]; tmp[2] = texto[8]; tmp[3] = texto[9]; tmp[4] = '-';
        tmp[5] = texto[3]; tmp[6] = texto[4]; tmp[7] = '-';
        tmp[8] = texto[0]; tmp[9] = texto[1];
        int k = 10;
        for (int i = 10; texto[i] != '\0' && k < 31; ++i) tmp[k++] = texto[i];
        tmp[k] = '\0';
        return f.leerISO(tmp);
    }
    return false;
}

}

bool GestionArchivos::cargarUsuarios(const char* ruta, ListaDinamica<Usuario>& usuarios) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) return false;

    char linea[LINEA_MAX];
    char username[64], password[64], nombre[128], correo[128], rutaFoto[256], ciudad[64], pais[64];
    int idCount = 0;
    while (archivo.getline(linea, LINEA_MAX)) {
        MedidorRecursos::registrarIteracion();
        if (linea[0] == '\0' || linea[0] == '\r') continue;

        int pos = leerCampo(linea, 0, username, 64);   if (pos < 0) continue;
        pos = leerCampo(linea, pos, password, 64);     if (pos < 0) continue;
        pos = leerCampo(linea, pos, nombre, 128);      if (pos < 0) continue;
        pos = leerCampo(linea, pos, correo, 128);      if (pos < 0) continue;
        pos = leerCampo(linea, pos, rutaFoto, 256);    if (pos < 0) continue;
        pos = leerCampo(linea, pos, ciudad, 64);       if (pos < 0) continue;
        leerCampo(linea, pos, pais, 64);

        Fecha fechaDefecto;
        Usuario nuevo(idCount, Cadena(username), Cadena(password), Cadena(nombre), fechaDefecto,
                      Cadena(correo), Cadena(rutaFoto), Cadena(ciudad), Cadena(pais), fechaDefecto);
        usuarios.agregar(nuevo);
        idCount++;
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
        if (u1 < 0 || u2 < 0 || u1 == u2 || u1 >= usuarios.getTamano() || u2 >= usuarios.getTamano()) continue;
        matriz.conectar(u1, u2);
        usuarios.obtener(u1).agregarAmigo(u2);
        usuarios.obtener(u2).agregarAmigo(u1);
    }
    archivo.close();
    return true;
}

bool GestionArchivos::cargarAlbumesYFotos(const char* ruta, ListaDinamica<Usuario>& usuarios) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) return false;

    int idUsuario;
    while (archivo >> idUsuario) {
        MedidorRecursos::registrarIteracion();
        archivo.ignore();

        char nomAlbum[64], descAlbum[128], fechaAlbumTxt[64], privAlbum[16], rutaFoto[256], fechaFotoTxt[64];
        int tamanoFoto = 0, likesFoto = 0;

        archivo.getline(nomAlbum, 64, ';');
        archivo.getline(descAlbum, 128, ';');
        archivo.getline(fechaAlbumTxt, 64, ';');
        archivo.getline(privAlbum, 16, ';');
        archivo.getline(rutaFoto, 256, ';');
        archivo.getline(fechaFotoTxt, 64, ';');
        archivo >> tamanoFoto;
        archivo.ignore();
        archivo >> likesFoto;
        if (archivo.fail()) break;

        if (idUsuario >= 0 && idUsuario < usuarios.getTamano()) {
            Usuario& user = usuarios.obtener(idUsuario);
            ListaDinamica<Album>& albumes = user.getAlbumes();

            Album* albumActual = nullptr;
            Cadena cNomAlbum(nomAlbum);
            for (int i = albumes.getTamano() - 1; i >= 0; --i) {
                MedidorRecursos::registrarIteracion();
                if (albumes.obtener(i).getNombre() == cNomAlbum) {
                    albumActual = &albumes.obtener(i);
                    break;
                }
            }

            if (!albumActual) {
                Fecha fechaCreacionAlbum;
                leerFechaFlexible(fechaAlbumTxt, fechaCreacionAlbum);
                Album nuevoAlbum(cNomAlbum, Cadena(descAlbum), fechaCreacionAlbum, Cadena(privAlbum));
                user.agregarAlbum(nuevoAlbum);
                albumActual = &user.getAlbumes().obtener(user.getAlbumes().getTamano() - 1);
            }

            Fecha fechaCapturaFoto;
            leerFechaFlexible(fechaFotoTxt, fechaCapturaFoto);
            Foto nuevaFoto(fechaCapturaFoto, tamanoFoto, Cadena(rutaFoto), likesFoto);
            albumActual->agregarFoto(nuevaFoto);
        }
    }
    archivo.close();
    return true;
}


bool GestionArchivos::cargarMensajes(const char* ruta, ListaDinamica<Usuario>& usuarios) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) return false;

    char linea[LINEA_MAX];
    char emisor[64], receptor[64], texto[1024], fechaTxt[32], estadoTxt[16];
    int ignoradas = 0;

    while (archivo.getline(linea, LINEA_MAX)) {
        MedidorRecursos::registrarIteracion();
        if (linea[0] == '\0' || linea[0] == '\r') continue;

        int len = 0;
        while (linea[len] != '\0' && linea[len] != '\r' && linea[len] != '\n') { MedidorRecursos::registrarIteracion(); len++; }

        int p1 = -1, p2 = -1, q1 = -1, q2 = -1;
        for (int i = 0; i < len && p2 < 0; ++i) {
            MedidorRecursos::registrarIteracion();
            if (linea[i] == ';') { if (p1 < 0) p1 = i; else p2 = i; }
        }
        for (int i = len - 1; i >= 0 && q2 < 0; --i) {
            MedidorRecursos::registrarIteracion();
            if (linea[i] == ';') { if (q1 < 0) q1 = i; else q2 = i; }
        }
        if (p1 < 0 || p2 < 0 || q1 < 0 || q2 < 0 || q2 <= p2) { ignoradas++; continue; }

        copiarRango(linea, 0, p1, emisor, 64);
        copiarRango(linea, p1 + 1, p2, receptor, 64);
        copiarRango(linea, p2 + 1, q2, texto, 513);
        copiarRango(linea, q2 + 1, q1, fechaTxt, 32);
        copiarRango(linea, q1 + 1, len, estadoTxt, 16);

        Fecha fechaEnvio;
        if (!leerFechaFlexible(fechaTxt, fechaEnvio)) { ignoradas++; continue; }
        bool leido = (estadoTxt[0] == 't' || estadoTxt[0] == 'T' || estadoTxt[0] == '1');

        Mensaje msj(Cadena(emisor), Cadena(receptor), Cadena(texto), fechaEnvio, leido);

        Cadena cEmisor(emisor), cReceptor(receptor);
        bool hallaronEmisor = false, hallaronReceptor = false;
        for (int i = 0; i < usuarios.getTamano() && !(hallaronEmisor && hallaronReceptor); ++i) {
            MedidorRecursos::registrarIteracion();
            if (!hallaronReceptor && usuarios.obtener(i).getNombreUsuario() == cReceptor) {
                usuarios.obtener(i).recibirMensaje(msj);
                hallaronReceptor = true;
            }
            if (!hallaronEmisor && usuarios.obtener(i).getNombreUsuario() == cEmisor) {
                usuarios.obtener(i).enviarMensaje(msj);
                hallaronEmisor = true;
            }
        }
    }
    archivo.close();
    if (ignoradas > 0) std::cout << "[Aviso] mensajes.txt: " << ignoradas << " lineas ignoradas por formato invalido.\n";
    return true;
}

bool GestionArchivos::cargarSolicitudes(const char* ruta, ListaDinamica<Usuario>& usuarios) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) return false;

    char linea[LINEA_MAX];
    char emisor[64], receptor[64], estado[32], fechaTxt[32];
    while (archivo.getline(linea, LINEA_MAX)) {
        MedidorRecursos::registrarIteracion();
        if (linea[0] == '\0' || linea[0] == '\r') continue;

        int pos = leerCampo(linea, 0, emisor, 64);     if (pos < 0) continue;
        pos = leerCampo(linea, pos, receptor, 64);     if (pos < 0) continue;
        pos = leerCampo(linea, pos, estado, 32);
        Fecha fechaSol;
        if (pos >= 0) {
            leerCampo(linea, pos, fechaTxt, 32);
            leerFechaFlexible(fechaTxt, fechaSol);
        }

        SolicitudAmistad sol(Cadena(emisor), Cadena(receptor), Cadena(estado), fechaSol);
        Cadena cReceptor(receptor);
        for (int i = 0; i < usuarios.getTamano(); ++i) {
            MedidorRecursos::registrarIteracion();
            if (usuarios.obtener(i).getNombreUsuario() == cReceptor) {
                usuarios.obtener(i).agregarSolicitud(sol);
                break;
            }
        }
    }
    archivo.close();
    return true;
}

bool GestionArchivos::guardarMensaje(const char* ruta, const Mensaje& msj) {
    std::ofstream archivo(ruta, std::ios::app);
    if (!archivo.is_open()) return false;

    char fechaTxt[17];
    msj.getFechaEnvio().aTexto(fechaTxt, 0);
    archivo << msj.getEmisor().c_str() << ";"
            << msj.getReceptor().c_str() << ";"
            << msj.getTexto().c_str() << ";"
            << fechaTxt << ";"
            << (msj.estaLeido() ? "true" : "false") << "\n";
    archivo.close();
    return true;
}

bool GestionArchivos::guardarSolicitud(const char* ruta, const SolicitudAmistad& sol) {
    std::ofstream archivo(ruta, std::ios::app);
    if (!archivo.is_open()) return false;

    char fechaTxt[17];
    Fecha::ahora().aTexto(fechaTxt, 0);
    archivo << sol.getEmisor().c_str() << ";" << sol.getReceptor().c_str() << ";"
            << sol.getEstado().c_str() << ";" << fechaTxt << "\n";
    archivo.close();
    return true;
}