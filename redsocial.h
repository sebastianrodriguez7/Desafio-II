#ifndef REDSOCIAL_H
#define REDSOCIAL_H

#include "usuario.h"
#include "matrizadyacente.h"
#include "listadinamica.h"

class RedSocial {
private:
    ListaDinamica<Usuario> usuarios;
    MatrizAdyacente matrizAmistades;
    Usuario* usuarioAutenticado;
    bool autenticarUsuario();
    void mostrarMejoresAmigos();
    void administrarAmistades();
    void visitarPerfilAmigo();
    void verSugerenciasAmistad();
    void administrarMensajes();
    void obtenerTresFotosAmigosPonderadas(Cadena rutasOut[3]);

    Usuario* buscarUsuarioPorNombre(const Cadena& nombre);
    size_t calcularMemoriaTotalSistema() const;

public:
    RedSocial();
    ~RedSocial();

    void iniciarSistema();
    void menuPrincipal();
};

#endif