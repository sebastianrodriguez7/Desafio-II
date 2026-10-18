
#include "redsocial.h"
#include "gestionarchivos.h"
#include "medidorrecursos.h"
#include <iostream>
#include <cstdlib>

RedSocial::RedSocial() : usuarioAutenticado(nullptr) {}
RedSocial::~RedSocial() {}

void RedSocial::iniciarSistema() {
    MedidorRecursos::reiniciarIteraciones();
    GestionArchivos::cargarUsuarios("usuarios.txt", usuarios);
    GestionArchivos::cargarAmistades("amistades.txt", usuarios, matrizAmistades);
    GestionArchivos::cargarAlbumesYFotos("albumes_fotos.txt", usuarios);
    GestionArchivos::cargarSolicitudes("solicitudes.txt", usuarios);
    GestionArchivos::cargarMensajes("mensajes.txt", usuarios);
    MedidorRecursos::reportarConsumo(calcularMemoriaTotalSistema());

    if (usuarios.getTamano() > 0) {
        while (true) {
            if (!autenticarUsuario()) {
                std::cout << "Saliendo completamente de Ude@Book...\n";
                break;
            }
            menuPrincipal();
        }
    } else {
        std::cout << "Advertencia: No se encontraron usuarios en usuarios.txt.\n";
    }
}

bool RedSocial::autenticarUsuario() {
    char bufferUser[128];
    char bufferPwd[128];
    MedidorRecursos::registrarMemoriaLocal(sizeof(bufferUser) + sizeof(bufferPwd));

    std::cout << "\n============================================\n";
    std::cout << "        INICIO DE SESION - Ude@Book         \n";
    std::cout << "============================================\n";
    std::cout << "Ingrese username (o 'SALIR' para terminar): ";
    std::cin >> bufferUser;

    Cadena ingresoUser(bufferUser);
    if (ingresoUser == Cadena("SALIR") || ingresoUser == Cadena("salir")) {
        return false;
    }

    Usuario* encontrado = buscarUsuarioPorNombre(ingresoUser);
    if (!encontrado) {
        std::cout << "\n[Error] Usuario no encontrado. Intente nuevamente.\n";
        return autenticarUsuario();
    }

    std::cout << "Ingrese contrasena para " << encontrado->getNombreUsuario().c_str() << ": ";
    std::cin >> bufferPwd;

    if (encontrado->verificarPassword(Cadena(bufferPwd))) {
        usuarioAutenticado = encontrado;
        std::cout << "\n> Acceso concedido. Bienvenid@ " << usuarioAutenticado->getNombreUsuario().c_str() << "\n";
        return true;
    } else {
        std::cout << "\n[Error] Contrasena incorrecta. Acceso denegado.\n";
        return autenticarUsuario();
    }
}

Usuario* RedSocial::buscarUsuarioPorNombre(const Cadena& nombre) {
    for (int i = 0; i < usuarios.getTamano(); ++i) {
        MedidorRecursos::registrarIteracion();
        if (usuarios.obtener(i).getNombreUsuario() == nombre) {
            return &usuarios.obtener(i);
        }
    }
    return nullptr;
}

size_t RedSocial::calcularMemoriaTotalSistema() const {
    size_t mem = sizeof(*this) + usuarios.memoriaDinamica() + matrizAmistades.obtenerMemoriaConsumida();
    for (int i = 0; i < usuarios.getTamano(); ++i) {
        mem += usuarios.obtener(i).obtenerMemoriaConsumida();
    }
    return mem;
}

void RedSocial::obtenerTresFotosAmigosPonderadas(Cadena rutasOut[3]) {
    const ListaDinamica<int>& amigos = usuarioAutenticado->getAmigos();
    int totalAmigos = amigos.getTamano();

    for (int i = 0; i < 3; ++i) {
        rutasOut[i] = Cadena("//ruta absoluta respectiva a la foto de perfil");
    }

    if (totalAmigos == 0) return;

    if (totalAmigos <= 3) {
        for (int i = 0; i < totalAmigos; ++i) {
            MedidorRecursos::registrarIteracion();
            int idAmigo = amigos.obtener(i);
            Usuario& amigo = usuarios.obtener(idAmigo);
            rutasOut[i] = amigo.getRutaFotoPerfil();
        }
        return;
    }

    int seleccionados[3] = {-1, -1, -1};
    int count = 0;

    MedidorRecursos::registrarUsoExterno(MedidorRecursos::ALEATORIO);
    while (count < 3) {
        int idxAleatorio = rand() % totalAmigos;
        int idAmigo = amigos.obtener(idxAleatorio);

        int enComun = matrizAmistades.contarAmigosEnComun(usuarioAutenticado->getId(), idAmigo, usuarios.getTamano());

        int peso = 2;
        if (enComun >= 11 && enComun <= 20) {
            peso = 5;
        } else if (enComun > 20) {
            peso = 7;
        }

        int tirada = rand() % 10;
        bool aceptado = (tirada < peso);

        bool yaEsta = false;
        for (int j = 0; j < count; ++j) {
            if (seleccionados[j] == idxAleatorio) yaEsta = true;
        }

        if ((aceptado || totalAmigos - count <= 3) && !yaEsta) {
            seleccionados[count] = idxAleatorio;
            Usuario& amigo = usuarios.obtener(idAmigo);
            rutasOut[count] = amigo.getRutaFotoPerfil();
            count++;
        }
    }
}

void RedSocial::menuPrincipal() {
    char opcionChar = ' ';
    bool fotosActualizadas = false;

    do {
        MedidorRecursos::reiniciarIteraciones();

        Cadena rutasFotos[3];
        obtenerTresFotosAmigosPonderadas(rutasFotos);

        std::cout << "\n============================================================\n";
        std::cout << "Foto perfil de amigo 1: " << rutasFotos[0].c_str() << "\n";
        std::cout << "Foto perfil de amigo 2: " << rutasFotos[1].c_str() << "\n";
        std::cout << "Foto perfil de amigo 3: " << rutasFotos[2].c_str() << "\n";
        std::cout << "\n        Bienvenido a Ude@Book // " << usuarioAutenticado->getNombreUsuario().c_str() << "\n";
        std::cout << "============================================================\n";
        std::cout << "Menu de opciones:\n";
        std::cout << " 1.- Administrar amistades\n";
        std::cout << " 2.- Visitar perfil de un amigo\n";
        std::cout << " 3.- Ver sugerencias de amistad\n";
        std::cout << " 4.- Administrar mensajes\n";
        std::cout << "------------------------------------------------------------\n";

        if (!fotosActualizadas) {
            std::cout << " .- Ingrese 'N' o 'n' para alternar las fotos de perfil (Disponible 1 sola vez).\n";
        }

        std::cout << " .- Ingrese 'S' o 's' para cerrar su sesion.\n";
        std::cout << "Su opcion: ";
        std::cin >> opcionChar;

        if (opcionChar == 'N' || opcionChar == 'n') {
            if (!fotosActualizadas) {
                fotosActualizadas = true;
                std::cout << "\n> ¡Fotos de perfil actualizadas por única vez con éxito!\n";
                continue;
            } else {
                std::cout << "\n[Aviso] Ya utilizaste tu oportunidad de actualizar las fotos en esta sesión.\n";
                continue;
            }
        } else if (opcionChar == 'S' || opcionChar == 's') {
            std::cout << "\nCerrando sesion de " << usuarioAutenticado->getNombreUsuario().c_str() << "...\n";
            usuarioAutenticado = nullptr;
            break;
        }

        int opcion = opcionChar - '0';
        switch (opcion) {
        case 1: administrarAmistades(); break;
        case 2: visitarPerfilAmigo(); break;
        case 3: verSugerenciasAmistad(); break;
        case 4: administrarMensajes(); break;
        default:
            std::cout << "[Error] Opción inválida. Intente de nuevo.\n";
            continue;
        }

        MedidorRecursos::reportarConsumo(calcularMemoriaTotalSistema());

    } while (usuarioAutenticado != nullptr);
}
void RedSocial::administrarAmistades() {
    int op = 0;
    std::cout << "\n--- ADMINISTRAR AMISTADES ---\n";
    std::cout << "1. Enviar solicitud de amistad\n";
    std::cout << "2. Aceptar solicitudes recibidas\n";
    std::cout << "Seleccione una opcion: ";
    std::cin >> op;

    if (op == 1) {
        char nombreDestino[64];
        std::cout << "Ingrese el nombre de usuario a quien desea enviar solicitud: ";
        std::cin >> nombreDestino;

        Usuario* destino = buscarUsuarioPorNombre(Cadena(nombreDestino));
        if (!destino) {
            std::cout << "[Error] El usuario no existe.\n";
            return;
        }

        if (destino->getId() == usuarioAutenticado->getId()) {
            std::cout << "[Error] No puedes enviarte una solicitud a ti mismo.\n";
            return;
        }

        if (matrizAmistades.sonAmigos(usuarioAutenticado->getId(), destino->getId())) {
            std::cout << "[Error] Ya son amigos en la plataforma.\n";
            return;
        }

        SolicitudAmistad nuevaSol(usuarioAutenticado->getNombreUsuario(), destino->getNombreUsuario(), Cadena("pendiente"), Fecha::ahora());
        destino->agregarSolicitud(nuevaSol);
        GestionArchivos::guardarSolicitud("solicitudes.txt", nuevaSol);
        std::cout << "> Solicitud de amistad enviada con exito\n";

    } else if (op == 2) {
        ListaDinamica<SolicitudAmistad>& sols = usuarioAutenticado->getSolicitudes();
        std::cout << "\nSolicitudes de amistad recibidas:\n";
        int pendientesCount = 0;

        for (int i = 0; i < sols.getTamano(); ++i) {
            MedidorRecursos::registrarIteracion();
            if (sols.obtener(i).getEstado() == Cadena("pendiente")) {
                std::cout << "[" << i + 1 << "] De: " << sols.obtener(i).getEmisor().c_str() << "\n";
                pendientesCount++;
            }
        }

        if (pendientesCount == 0) {
            std::cout << "No tienes solicitudes pendientes.\n";
            return;
        }

        int indexSel = -1;
        std::cout << "Ingrese el indice de la solicitud que desea aceptar (o -1 para salir): ";
        std::cin >> indexSel;

        if (indexSel >= 1 && indexSel <= sols.getTamano()) {
            SolicitudAmistad& solSeleccionada = sols.obtener(indexSel - 1);
            if (solSeleccionada.getEstado() == Cadena("pendiente")) {
                solSeleccionada.setEstado(Cadena("aceptada"));

                Usuario* emisorSol = buscarUsuarioPorNombre(solSeleccionada.getEmisor());
                if (emisorSol) {
                    usuarioAutenticado->agregarAmigo(emisorSol->getId());
                    emisorSol->agregarAmigo(usuarioAutenticado->getId());
                    matrizAmistades.conectar(usuarioAutenticado->getId(), emisorSol->getId());
                    std::cout << " Solicitud aceptada! Ahora son amigos.\n";
                }
            } else {
                std::cout << "[Aviso] Esa solicitud ya fue procesada.\n";
            }
        }
    }
}

void RedSocial::visitarPerfilAmigo() {
    char nombreAmigo[64];
    std::cout << "\nIngrese el nombre de usuario del amigo que desea visitar: ";
    std::cin >> nombreAmigo;

    Usuario* amigo = buscarUsuarioPorNombre(Cadena(nombreAmigo));
    if (!amigo || !matrizAmistades.sonAmigos(usuarioAutenticado->getId(), amigo->getId())) {
        std::cout << "[Error] El usuario no existe o no es tu amigo.\n";
        return;
    }

    ListaDinamica<Album>& albumes = amigo->getAlbumes();
    std::cout << "\nAlbumes de " << amigo->getNombreUsuario().c_str() << ":\n";
    int validosCount = 0;

    for (int i = 0; i < albumes.getTamano(); ++i) {
        MedidorRecursos::registrarIteracion();
        Cadena priv = albumes.obtener(i).getPrivacidad();
        if (priv == Cadena("publico") || priv == Cadena("amigos")) {
            std::cout << "[" << validosCount + 1 << "] " << albumes.obtener(i).getNombre().c_str()
            << " (Privacidad: " << priv.c_str() << ", Fotos: " << albumes.obtener(i).getCantidadFotos() << ")\n";
            validosCount++;
        }
    }

    if (validosCount == 0) {
        std::cout << "Este usuario no tiene albumes visibles para ti.\n";
        return;
    }

    int idxAlbum = -1;
    std::cout << "Seleccione el numero de album a visualizar en carrusel: ";
    std::cin >> idxAlbum;

    if (idxAlbum < 1 || idxAlbum > validosCount) {
        std::cout << "Seleccion invalida.\n";
        return;
    }

    Album& albumSel = albumes.obtener(idxAlbum - 1);
    if (albumSel.getCantidadFotos() == 0) {
        std::cout << "El album seleccionado no contiene fotos.\n";
        return;
    }

    int fotoActual = 0;
    char opcionCarrusel = ' ';
    bool fotoConLikeActual = false;

    while (opcionCarrusel != 's' && opcionCarrusel != 'S') {
        MedidorRecursos::registrarIteracion();
        Foto& foto = albumSel.obtenerFoto(fotoActual);

        std::cout << "\n--- CARRUSEL DE FOTOS (Circular) ---\n";
        std::cout << "Foto " << fotoActual + 1 << " de " << albumSel.getCantidadFotos() << "\n";
        std::cout << "Ruta: " << foto.getRuta().c_str() << "\n";
        std::cout << "Likes: " << foto.getLikes() << "\n";
        std::cout << "------------------------------------\n";
        std::cout << "[n] Siguiente | [p] Previa | [l] Dar/Quitar Like | [s] Salir\n";
        std::cout << "Opcion: ";
        std::cin >> opcionCarrusel;

        if (opcionCarrusel == 'n' || opcionCarrusel == 'N') {
            fotoActual = (fotoActual + 1) % albumSel.getCantidadFotos();
            fotoConLikeActual = false;
        } else if (opcionCarrusel == 'p' || opcionCarrusel == 'P') {
            fotoActual = (fotoActual - 1 + albumSel.getCantidadFotos()) % albumSel.getCantidadFotos();
            fotoConLikeActual = false;
        } else if (opcionCarrusel == 'l' || opcionCarrusel == 'L') {
            if (!fotoConLikeActual) {
                foto.darLike();
                fotoConLikeActual = true;
                std::cout << " Has dado like a la foto.\n";
            } else {
                foto.quitarLike();
                fotoConLikeActual = false;
                std::cout << " Has quitado el like a la foto.\n";
            }
        }
    }
}

void RedSocial::verSugerenciasAmistad() {
    int k = 5;
    std::cout << "\nIngrese la cantidad K de sugerencias a mostrar (1 - 20): ";
    std::cin >> k;
    if (k < 1) k = 1;
    if (k > 20) k = 20;

    std::cout << "\n--- TOP " << k << " SUGERENCIAS DE AMISTAD ---\n";
    int mostradas = 0;

    for (int i = 0; i < usuarios.getTamano() && mostradas < k; ++i) {
        MedidorRecursos::registrarIteracion();
        int idCandidato = usuarios.obtener(i).getId();

        if (idCandidato == usuarioAutenticado->getId()) continue;
        if (matrizAmistades.sonAmigos(usuarioAutenticado->getId(), idCandidato)) continue;

        int enComun = matrizAmistades.contarAmigosEnComun(usuarioAutenticado->getId(), idCandidato, usuarios.getTamano());
        std::cout << "[" << mostradas + 1 << "] Usuario: " << usuarios.obtener(i).getNombreUsuario().c_str()
                  << " | Amigos en comun: " << enComun << "\n";
        mostradas++;
    }

    if (mostradas == 0) {
        std::cout << "No hay sugerencias disponibles en este momento.\n";
    }
}

void RedSocial::administrarMensajes() {
    int op = 0;
    std::cout << "\n--- ADMINISTRAR MENSAJES ---\n";
    std::cout << "1. Enviar mensaje a un amigo\n";
    std::cout << "2. Leer mensajes nuevos no leidos\n";
    std::cout << "3. Buscar mensajes por rango de fecha\n";
    std::cout << "Seleccione una opcion: ";
    std::cin >> op;

    if (op == 1) {
        char nombreAmigo[64];
        std::cout << "Ingrese el nombre de su amigo: ";
        std::cin >> nombreAmigo;

        Usuario* amigo = buscarUsuarioPorNombre(Cadena(nombreAmigo));
        if (!amigo || !matrizAmistades.sonAmigos(usuarioAutenticado->getId(), amigo->getId())) {
            std::cout << "[Error] El usuario no existe o no es tu amigo.\n";
            return;
        }

        std::cout << "Ingrese el texto del mensaje (max 512 caracteres):\n";
        std::cin.ignore();
        char textoMsj[512];
        MedidorRecursos::registrarMemoriaLocal(sizeof(textoMsj));
        std::cin.getline(textoMsj, 512);

        Mensaje nuevoMsj(usuarioAutenticado->getNombreUsuario(), amigo->getNombreUsuario(), Cadena(textoMsj), Fecha::ahora(), false);
        amigo->recibirMensaje(nuevoMsj);
        usuarioAutenticado->enviarMensaje(nuevoMsj);
        GestionArchivos::guardarMensaje("mensajes.txt", nuevoMsj);
        std::cout << " Mensaje enviado con exito \n";

    } else if (op == 2) {
        ListaDinamica<Mensaje>& recibidos = usuarioAutenticado->getMensajesRecibidos();
        std::cout << "\n--- MENSAJES RECIBIDOS NO LEIDOS ---\n";
        int noLeidosCount = 0;

        for (int i = 0; i < recibidos.getTamano(); ++i) {
            MedidorRecursos::registrarIteracion();
            if (!recibidos.obtener(i).estaLeido()) {
                std::cout << "De: " << recibidos.obtener(i).getEmisor().c_str() << " | Fecha: ";
                recibidos.obtener(i).getFechaEnvio().mostrar();
                std::cout << " | Texto: " << recibidos.obtener(i).getTexto().c_str() << "\n";
                recibidos.obtener(i).marcarComoLeido();
                noLeidosCount++;
            }
        }

        if (noLeidosCount == 0) {
            std::cout << "No tienes mensajes nuevos sin leer.\n";
        }
    } else if (op == 3) {
        ListaDinamica<Mensaje>& recibidos = usuarioAutenticado->getMensajesRecibidos();
        std::cout << "\n--- BUSCAR MENSAJES POR RANGO DE FECHA ---\n";
        int d1, m1, a1, d2, m2, a2;
        std::cout << "Ingrese fecha de inicio (DD MM AAAA): ";
        std::cin >> d1 >> m1 >> a1;
        std::cout << "Ingrese fecha de fin (DD MM AAAA): ";
        std::cin >> d2 >> m2 >> a2;

        Fecha fechaInicio(d1, m1, a1, 0, 0);
        Fecha fechaFin(d2, m2, a2, 23, 59);

        int encontrados = 0;
        for (int i = 0; i < recibidos.getTamano(); ++i) {
            MedidorRecursos::registrarIteracion();
            const Mensaje& m = recibidos.obtener(i);
            const Fecha& f = m.getFechaEnvio();

            if (!(f < fechaInicio) && !(fechaFin < f)) {
                std::cout << "De: " << m.getEmisor().c_str() << " | Fecha: ";
                f.mostrar();
                std::cout << " | Texto: " << m.getTexto().c_str() << "\n";
                encontrados++;
            }
        }

        if (encontrados == 0) {
            std::cout << "No se encontraron mensajes en ese rango de fechas.\n";
        } else {
            std::cout << "Mensajes encontrados: " << encontrados << "\n";
        }
    }
}