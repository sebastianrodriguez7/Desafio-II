#include "solicitudamistad.h"

SolicitudAmistad::SolicitudAmistad() : emisor(), receptor(), estado(), fechaSolicitud() {}

SolicitudAmistad::SolicitudAmistad(Cadena emi, Cadena rec, Cadena est, Fecha fecha)
    : emisor(emi), receptor(rec), estado(est), fechaSolicitud(fecha) {}

const Cadena& SolicitudAmistad::getEmisor() const { return emisor; }
const Cadena& SolicitudAmistad::getReceptor() const { return receptor; }
const Cadena& SolicitudAmistad::getEstado() const { return estado; }
void SolicitudAmistad::setEstado(Cadena nuevoEstado) { estado = nuevoEstado; }

size_t SolicitudAmistad::obtenerMemoriaConsumida() const {
    return sizeof(*this) + emisor.memoriaDinamica() + receptor.memoriaDinamica() + estado.memoriaDinamica();
}