cmake_minimum_required(VERSION 3.16)

project(DesafioII LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DesafioII main.cpp
    redsocial.h redsocial.cpp
    medidorrecursos.h medidorrecursos.cpp
    gestionarchivos.h gestionarchivos.cpp
    usuario.h usuario.cpp
    solicitudamistad.h solicitudamistad.cpp
    fecha.h fecha.cpp
    album.h album.cpp
    foto.h foto.cpp
    mensaje.h mensaje.cpp
    matrizadyacente.h matrizadyacente.cpp
    listadinamica.h listadinamica.cpp
    cadena.h cadena.cpp)

include(GNUInstallDirs)
install(TARGETS DesafioII
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
