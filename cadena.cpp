#include "cadena.h"
#include "medidorrecursos.h"

// NOTA: No se debe repetir la palabra 'static' fuera de la clase
int Cadena::medirLongitud(const char* str) {
    int len = 0;
    while (str && str[len] != '\0') {
        MedidorRecursos::registrarIteracion();
        len++;
    }
    return len;
}

Cadena::Cadena() : buffer(nullptr), longitud(0), capacidad(0) {}

Cadena::Cadena(const char* str) : buffer(nullptr), longitud(0), capacidad(0) {
    longitud = medirLongitud(str);
    if (longitud > 0) {
        capacidad = longitud + 1;
        buffer = new char[capacidad];
        for (int i = 0; i <= longitud; ++i) {
            MedidorRecursos::registrarIteracion();
            buffer[i] = str[i];
        }
    }
}

Cadena::Cadena(const char* str, int cantidad) : buffer(nullptr), longitud(0), capacidad(0) {
    if (str && cantidad > 0) {
        longitud = cantidad;
        capacidad = cantidad + 1;
        buffer = new char[capacidad];
        for (int i = 0; i < cantidad; ++i) {
            MedidorRecursos::registrarIteracion();
            buffer[i] = str[i];
        }
        buffer[cantidad] = '\0';
    }
}

Cadena::Cadena(const Cadena& otra) : buffer(nullptr), longitud(otra.longitud), capacidad(0) {
    if (otra.longitud > 0) {
        capacidad = otra.longitud + 1;
        buffer = new char[capacidad];
        for (int i = 0; i <= longitud; ++i) {
            MedidorRecursos::registrarIteracion();
            buffer[i] = otra.buffer[i];
        }
    }
}

Cadena& Cadena::operator=(const Cadena& otra) {
    if (this != &otra) {
        if (otra.longitud == 0) {
            delete[] buffer;
            buffer = nullptr;
            longitud = 0;
            capacidad = 0;
        } else {
            if (capacidad < otra.longitud + 1) {
                delete[] buffer;
                capacidad = otra.longitud + 1;
                buffer = new char[capacidad];
            }
            longitud = otra.longitud;
            for (int i = 0; i <= longitud; ++i) {
                MedidorRecursos::registrarIteracion();
                buffer[i] = otra.buffer[i];
            }
        }
    }
    return *this;
}

Cadena::~Cadena() {
    delete[] buffer;
}

int Cadena::getLongitud() const { return longitud; }
int Cadena::getCapacidad() const { return capacidad; }
const char* Cadena::c_str() const { return buffer ? buffer : ""; }

int Cadena::comparar(const Cadena& otra) const {
    const char* a = c_str();
    const char* b = otra.c_str();
    int i = 0;
    while (a[i] != '\0' && a[i] == b[i]) {
        MedidorRecursos::registrarIteracion();
        i++;
    }
    return (int)(unsigned char)a[i] - (int)(unsigned char)b[i];
}

bool Cadena::operator==(const Cadena& otra) const {
    if (longitud != otra.longitud) return false;
    for (int i = 0; i < longitud; ++i) {
        MedidorRecursos::registrarIteracion();
        if (buffer[i] != otra.buffer[i]) return false;
    }
    return true;
}

bool Cadena::operator!=(const Cadena& otra) const { return !(*this == otra); }
bool Cadena::operator<(const Cadena& otra) const { return comparar(otra) < 0; }

Cadena Cadena::operator+(const Cadena& otra) const {
    Cadena resultado;
    int nuevaLen = longitud + otra.longitud;
    if (nuevaLen > 0) {
        resultado.capacidad = nuevaLen + 1;
        resultado.buffer = new char[resultado.capacidad];
        for (int i = 0; i < longitud; ++i) {
            MedidorRecursos::registrarIteracion();
            resultado.buffer[i] = buffer[i];
        }
        for (int j = 0; j < otra.longitud; ++j) {
            MedidorRecursos::registrarIteracion();
            resultado.buffer[longitud + j] = otra.buffer[j];
        }
        resultado.buffer[nuevaLen] = '\0';
        resultado.longitud = nuevaLen;
    }
    return resultado;
}

void Cadena::aMinusculas() {
    for (int i = 0; i < longitud; ++i) {
        MedidorRecursos::registrarIteracion();
        if (buffer[i] >= 'A' && buffer[i] <= 'Z') buffer[i] = (char)(buffer[i] + 32);
    }
}

bool Cadena::esVacia() const { return longitud == 0; }
bool Cadena::excedeLimite(int maxCaracteres) const { return longitud > maxCaracteres; }
size_t Cadena::memoriaDinamica() const { return sizeof(char) * (size_t)capacidad; }