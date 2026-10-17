#ifndef MENSAJE_H
#define MENSAJE_H

#include "cadena.h"
#include "fecha.h"

class Mensaje {
private:
    Cadena emisor;
    Cadena receptor;
    Cadena texto;
    Fecha fechaEnvio;
    bool leido;

public:
    Mensaje();
    Mensaje(Cadena emi, Cadena rec, Cadena txt, Fecha fecha, bool estado = false);

    const Cadena& getEmisor() const;
    const Cadena& getReceptor() const;
    const Cadena& getTexto() const;
    const Fecha& getFechaEnvio() const;
    bool estaLeido() const;
    void marcarComoLeido();

    size_t obtenerMemoriaConsumida() const;
    size_t memoriaDinamica() const { return obtenerMemoriaConsumida(); }
};
#endif