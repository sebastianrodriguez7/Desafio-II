#ifndef SOLICITUDAMISTAD_H
#define SOLICITUDAMISTAD_H

#include "cadena.h"
#include "fecha.h"

class SolicitudAmistad {
private:
    Cadena emisor;
    Cadena receptor;
    Cadena estado;
    Fecha fechaSolicitud;

public:
    SolicitudAmistad();
    SolicitudAmistad(Cadena emi, Cadena rec, Cadena est, Fecha fecha);

    const Cadena& getEmisor() const;
    const Cadena& getReceptor() const;
    const Cadena& getEstado() const;
    void setEstado(Cadena nuevoEstado);

    size_t obtenerMemoriaConsumida() const;
    size_t memoriaDinamica() const { return obtenerMemoriaConsumida(); }
};

#endif