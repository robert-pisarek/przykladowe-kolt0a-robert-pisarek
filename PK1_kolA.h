#pragma once
#include <iostream>
#include <cstring>
struct Dane
{
	short liczba;
	char symbol;
	bool aktywna;
	bool wczytana;
	bool obliczona;
	bool estymowana;
};

void wypelnij(Dane& d,
    short liczba,
    char symbol,
    bool aktywna,
    bool wczytana,
    bool obliczona,
    bool estymowana)
{
    d.liczba = liczba;
    d.symbol = symbol;
    d.aktywna = aktywna;
    d.wczytana = wczytana;
    d.obliczona = obliczona;
    d.estymowana = estymowana;
}

std::ostream& operator<<(std::ostream& os, const Dane& d)
{
    os << "liczba: " << d.liczba
        << ", symbol: " << d.symbol
        << ", aktywna: " << d.aktywna
        << ", wczytana: " << d.wczytana
        << ", obliczona: " << d.obliczona
        << ", estymowana: " << d.estymowana;

    return os;
}
bool operator==(const Dane& a, const Dane& b)
{
    // 1. porównanie logiczne
    bool logiczne =
        (a.liczba == b.liczba) &&
        (a.symbol == b.symbol) &&
        (a.aktywna == b.aktywna) &&
        (a.wczytana == b.wczytana) &&
        (a.obliczona == b.obliczona) &&
        (a.estymowana == b.estymowana);
    if (!logiczne)
        std::cerr << "Wartosci pol sie nie zgadzaja!" << std::endl;

    // 2. porównanie binarne
    bool binarne = (std::memcmp(&a, &b, sizeof(Dane)) == 0);
    if (!binarne)
        std::cerr << "instancje maj¹ rozny zapis binarny!" << std::endl;
    return logiczne && binarne;
}

