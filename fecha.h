#ifndef FECHA_H
#define FECHA_H

#include <cstddef>

class Fecha {
private:
    int dia, mes, anio, hora, minuto;

    static int diasDelMes(int mes, int anio);

public:
    Fecha();
    Fecha(int d, int m, int a, int h = 0, int min = 0);

    int getDia() const;
    int getMes() const;
    int getAnio() const;
    int getHora() const;
    int getMinuto() const;
    void setFecha(int d, int m, int a);
    void setHora(int h, int min);

    int comparar(const Fecha& otra) const;
    bool operator<(const Fecha& otra) const;
    bool operator<=(const Fecha& otra) const;
    bool operator==(const Fecha& otra) const;
    bool estaEnRango(const Fecha& inicio, const Fecha& fin) const;

    bool esValida() const;
    bool leerISO(const char* texto);
    bool leerLatino(const char* texto);

    void aTexto(char* destino, int formato) const;
    void mostrar() const;

    static Fecha ahora();
    size_t memoriaDinamica() const;
};
#endif