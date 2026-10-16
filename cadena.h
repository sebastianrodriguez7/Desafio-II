#ifndef CADENA_H
#define CADENA_H

#include <cstddef>

class Cadena {
private:
    char* buffer;
    int longitud;
    int capacidad;

    static int medirLongitud(const char* str);

public:
    Cadena();
    Cadena(const char* str);
    Cadena(const char* str, int cantidad); // Copia solo 'cantidad' caracteres
    Cadena(const Cadena& otra);            // Copia profunda
    Cadena& operator=(const Cadena& otra); // Copia profunda
    ~Cadena();

    int getLongitud() const;
    int getCapacidad() const;
    const char* c_str() const;

    int comparar(const Cadena& otra) const; // <0, 0, >0 (orden ASCII)
    bool operator==(const Cadena& otra) const;
    bool operator!=(const Cadena& otra) const;
    bool operator<(const Cadena& otra) const;
    Cadena operator+(const Cadena& otra) const;

    void aMinusculas();
    bool esVacia() const;
    bool excedeLimite(int maxCaracteres = 512) const;
    size_t memoriaDinamica() const;         // bytes reservados con new
};

#endif
