#include "mensaje.h"

Mensaje::Mensaje() : emisor(), receptor(), texto(), fechaEnvio(), leido(false) {}

Mensaje::Mensaje(Cadena emi, Cadena rec, Cadena txt, Fecha fecha, bool estado)
    : emisor(emi), receptor(rec), texto(txt), fechaEnvio(fecha), leido(estado) {}

const Cadena& Mensaje::getEmisor() const { return emisor; }
const Cadena& Mensaje::getReceptor() const { return receptor; }
const Cadena& Mensaje::getTexto() const { return texto; }
const Fecha& Mensaje::getFechaEnvio() const { return fechaEnvio; }
bool Mensaje::estaLeido() const { return leido; }
void Mensaje::marcarComoLeido() { leido = true; }

size_t Mensaje::obtenerMemoriaConsumida() const {
    return sizeof(*this) + emisor.memoriaDinamica() + receptor.memoriaDinamica() + texto.memoriaDinamica();
}