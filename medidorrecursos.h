#ifndef MEDIDORRECURSOS_H
#define MEDIDORRECURSOS_H

#include <cstddef>

// Mide las dos metricas del enunciado (VII): iteraciones y memoria.
// Tambien registra el uso de componentes externos (archivos, aleatorios, reloj).
class MedidorRecursos {
public:
    enum TipoExterno { ARCHIVO = 0, ALEATORIO = 1, RELOJ = 2 };

private:
    static unsigned long long contadorIteraciones;
    static unsigned long long usosExternos[3];
    static size_t memoriaLocales;

public:
    static void reiniciarIteraciones();                 // reinicia iteraciones, usos externos y locales
    static void registrarIteracion(unsigned long long cantidad = 1);
    static void registrarUsoExterno(TipoExterno tipo);
    static void registrarMemoriaLocal(size_t bytes);    // variables locales / buffers de la funcionalidad
    static unsigned long long getIteraciones();
    static void reportarConsumo(size_t memoriaEstructuras);
};

#endif
