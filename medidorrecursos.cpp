#include "medidorrecursos.h"
#include <iostream>

unsigned long long MedidorRecursos::contadorIteraciones = 0;
unsigned long long MedidorRecursos::usosExternos[3] = {0, 0, 0};
size_t MedidorRecursos::memoriaLocales = 0;

void MedidorRecursos::reiniciarIteraciones() {
    contadorIteraciones = 0;
    usosExternos[0] = usosExternos[1] = usosExternos[2] = 0;
    memoriaLocales = 0;
}

void MedidorRecursos::registrarIteracion(unsigned long long cantidad) {
    contadorIteraciones += cantidad;
}

void MedidorRecursos::registrarUsoExterno(TipoExterno tipo) {
    usosExternos[tipo]++;
}

void MedidorRecursos::registrarMemoriaLocal(size_t bytes) {
    memoriaLocales += bytes;
}

unsigned long long MedidorRecursos::getIteraciones() {
    return contadorIteraciones;
}

void MedidorRecursos::reportarConsumo(size_t memoriaEstructuras) {
    size_t total = memoriaEstructuras + memoriaLocales;
    std::cout << "\n============================================\n";
    std::cout << "     [MEDICION DE CONSUMO DE RECURSOS]\n";
    std::cout << " Iteraciones (directas e indirectas): " << contadorIteraciones << "\n";
    std::cout << " Componentes externos usados: archivos=" << usosExternos[ARCHIVO]
              << ", generador aleatorio=" << usosExternos[ALEATORIO]
              << ", reloj del sistema=" << usosExternos[RELOJ] << "\n";
    std::cout << " Memoria de estructuras y objetos: " << memoriaEstructuras << " bytes\n";
    std::cout << " Memoria de variables locales:     " << memoriaLocales << " bytes\n";
    std::cout << " Memoria total en este momento:    " << total << " bytes ("
              << (double)total / 1024.0 << " KB)\n";
    std::cout << "============================================\n\n";
}