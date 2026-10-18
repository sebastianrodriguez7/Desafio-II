#include "fecha.h"
#include "medidorrecursos.h"
#include <iostream>
#include <ctime>

static bool leerNumero(const char*& p, int& valor) {
    if (*p < '0' || *p > '9') return false;
    int v = 0, digitos = 0;
    while (*p >= '0' && *p <= '9') {
        MedidorRecursos::registrarIteracion();
        v = v * 10 + (*p - '0');
        p++;
        if (++digitos > 9) return false;
    }
    valor = v;
    return true;
}

static void escribir2(char* d, int v) { d[0] = (char)('0' + (v / 10) % 10); d[1] = (char)('0' + v % 10); }
static void escribir4(char* d, int v) {
    d[0] = (char)('0' + (v / 1000) % 10); d[1] = (char)('0' + (v / 100) % 10);
    d[2] = (char)('0' + (v / 10) % 10);   d[3] = (char)('0' + v % 10);
}

Fecha::Fecha() : dia(1), mes(1), anio(2026), hora(0), minuto(0) {}

Fecha::Fecha(int d, int m, int a, int h, int min)
    : dia(d), mes(m), anio(a), hora(h), minuto(min) {}

int Fecha::getDia() const { return dia; }
int Fecha::getMes() const { return mes; }
int Fecha::getAnio() const { return anio; }
int Fecha::getHora() const { return hora; }
int Fecha::getMinuto() const { return minuto; }

void Fecha::setFecha(int d, int m, int a) { dia = d; mes = m; anio = a; }
void Fecha::setHora(int h, int min) { hora = h; minuto = min; }

int Fecha::comparar(const Fecha& o) const {
    if (anio != o.anio) return anio - o.anio;
    if (mes != o.mes) return mes - o.mes;
    if (dia != o.dia) return dia - o.dia;
    if (hora != o.hora) return hora - o.hora;
    return minuto - o.minuto;
}

bool Fecha::operator<(const Fecha& o) const { return comparar(o) < 0; }
bool Fecha::operator<=(const Fecha& o) const { return comparar(o) <= 0; }
bool Fecha::operator==(const Fecha& o) const { return comparar(o) == 0; }

bool Fecha::estaEnRango(const Fecha& inicio, const Fecha& fin) const {
    return inicio <= *this && *this <= fin;
}

int Fecha::diasDelMes(int mes, int anio) {
    if (mes == 2) {
        bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
        return bisiesto ? 29 : 28;
    }
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) return 30;
    return 31;
}

bool Fecha::esValida() const {
    if (anio < 1900 || anio > 2100) return false;
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > diasDelMes(mes, anio)) return false;
    if (hora < 0 || hora > 23) return false;
    if (minuto < 0 || minuto > 59) return false;
    return true;
}

bool Fecha::leerISO(const char* texto) {
    if (!texto) return false;
    const char* p = texto;
    int a, m, d, h = 0, min = 0;
    if (!leerNumero(p, a)) return false;
    if (*p != '-') return false; p++;
    if (!leerNumero(p, m)) return false;
    if (*p != '-') return false; p++;
    if (!leerNumero(p, d)) return false;

    if (*p == ' ' || *p == 'T') {
        p++;
        if (!leerNumero(p, h)) return false;
        if (*p != ':') return false; p++;
        if (!leerNumero(p, min)) return false;
    }
    anio = a; mes = m; dia = d; hora = h; minuto = min;
    return esValida();
}

bool Fecha::leerLatino(const char* texto) {
    if (!texto) return false;
    const char* p = texto;
    int a, m, d, h = 0, min = 0;
    if (!leerNumero(p, d)) return false;
    if (*p != '/') return false; p++;
    if (!leerNumero(p, m)) return false;
    if (*p != '/') return false; p++;
    if (!leerNumero(p, a)) return false;

    if (*p == ' ' || *p == 'T') {
        p++;
        if (!leerNumero(p, h)) return false;
        if (*p != ':') return false; p++;
        if (!leerNumero(p, min)) return false;
    }
    anio = a; mes = m; dia = d; hora = h; minuto = min;
    return esValida();
}

void Fecha::aTexto(char* destino, int formato) const {
    if (!destino) return;
    if (formato == 0) {
        escribir4(destino, anio); destino[4] = '-';
        escribir2(destino + 5, mes); destino[7] = '-';
        escribir2(destino + 8, dia); destino[10] = ' ';
        escribir2(destino + 11, hora); destino[13] = ':';
        escribir2(destino + 14, minuto); destino[16] = '\0';
    } else if (formato == 1) {
        escribir4(destino, anio); destino[4] = '-';
        escribir2(destino + 5, mes); destino[7] = '-';
        escribir2(destino + 8, dia); destino[10] = '\0';
    } else {
        escribir2(destino, dia); destino[2] = '/';
        escribir2(destino + 3, mes); destino[5] = '/';
        escribir4(destino + 6, anio); destino[10] = '\0';
    }
}

void Fecha::mostrar() const {
    char buf[32];
    aTexto(buf, 0);
    std::cout << buf;
}

Fecha Fecha::ahora() {
    MedidorRecursos::registrarUsoExterno(MedidorRecursos::RELOJ);

    std::time_t t = std::time(nullptr);
    std::tm* ahoraLocal = std::localtime(&t);

    return Fecha(
        ahoraLocal->tm_mday,
        ahoraLocal->tm_mon + 1,
        ahoraLocal->tm_year + 1900,
        ahoraLocal->tm_hour,
        ahoraLocal->tm_min
        );
}

size_t Fecha::memoriaDinamica() const {
    return 0;
}