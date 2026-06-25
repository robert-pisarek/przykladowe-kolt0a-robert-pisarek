#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>
#include <bitset>
#include <stdexcept>
#include "PK1_kolA.h"

using namespace std;

namespace DaneT {
    enum SHIFT {
        s_liczba = 15,
        s_symbol = 4,
        s_aktywna = 14,
        s_wczytana = 0,
        s_obliczona = 2,
        s_estymowana = 31
    };
    enum MASK {
        liczba = 0xFFFF,
        symbol = 0xFF,
        aktywna = 1,
        wczytana = 1,
        obliczona = 1,
        estymowana = 1
    };
}

void Dane_compress(const Dane src, uint32_t &dest){
    dest |= src.liczba << DaneT::SHIFT::s_liczba;
	dest |= src.symbol << DaneT::SHIFT::s_symbol;
	dest |= src.aktywna << DaneT::SHIFT::s_aktywna;
	dest |= src.wczytana << DaneT::SHIFT::s_wczytana;
	dest |= src.obliczona << DaneT::SHIFT::s_obliczona;
	dest |= src.estymowana << DaneT::SHIFT::s_estymowana;
}

void Dane_decompress(const uint32_t src, Dane &dest){
    dest.liczba = (src >> DaneT::SHIFT::s_liczba) & DaneT::MASK::liczba;
	dest.symbol = (src >> DaneT::SHIFT::s_symbol) & DaneT::MASK::symbol;
	dest.aktywna = (src >> DaneT::SHIFT::s_aktywna) & DaneT::MASK::aktywna;
	dest.wczytana = (src >> DaneT::SHIFT::s_wczytana) & DaneT::MASK::wczytana;
	dest.obliczona = (src >> DaneT::SHIFT::s_obliczona) & DaneT::MASK::obliczona;
	dest.estymowana = (src >> DaneT::SHIFT::s_estymowana) & DaneT::MASK::estymowana;
}

void Dane_setObliczona(uint32_t &dest, bool value){
    dest &= ~(DaneT::MASK::obliczona << DaneT::SHIFT::s_obliczona);
    dest |= value << DaneT::SHIFT::s_obliczona;
}

short Dane_getLiczba(uint32_t &src){
    return (src >> DaneT::SHIFT::s_liczba) & DaneT::MASK::liczba;
}

struct DA {
    uint32_t *data;
    uint32_t size;
};

DA *DA_alloc(uint32_t size){
    DA *temp = new DA();
    temp->data = new uint32_t[size];
    temp->size = size;
    for(int i = 0; i < size; i++){
        temp->data[i] = 0;
    }
    // memset(temp->data, 0, size * sizeof(uint32_t));
    return temp;
}

void DA_free(DA *array){
    delete[] array->data;
    delete array;
    array = nullptr;
}

void DA_set(DA *array, const uint32_t src, uint32_t index){
    if(index >= array->size) throw std::out_of_range("invalid index");
    array->data[index] = src;
    // memcpy(array->data, &src, sizeof(uint32_t));
}

void DA_set(DA *array, const Dane src, uint32_t index){
    if(index >= array->size) throw std::out_of_range("invalid index");
    uint32_t csrc = 0;
    Dane_compress(src, csrc);
    array->data[index] = csrc;
    // memcpy(array->data, &csrc, sizeof(uint32_t));
}

uint32_t DA_getSize(DA *array){
    return array->size;
}

void DA_writeToFile(const char *filename, DA *src){
    FILE *fd = fopen(filename, "w");
    if(fd != nullptr){
        for(int i = 0; i < src->size; i++){
            fprintf(fd, "%u; ", src->data[i]);
        }
    }
    fprintf(fd, "\n%u", DA_getSize(src));
    fclose(fd);
    fd = nullptr;
}

DA *DA_readFromFile(const char *filename){
    FILE *fd = fopen(filename, "r");
    int size = 0;
    const size_t len = 256;
    char buff[len];
    fgets(buff, len, fd);
    fgets(buff, len, fd);
    sscanf(buff, "%d", &size);
    cout << endl << "size: " << size << endl;
    rewind(fd);
    DA *array = DA_alloc(size);
    for(int i = 0; i < size && !feof(fd); i++){
        uint32_t temp = 0;
        fscanf(fd, "%u; ", &temp);
        DA_set(array, temp, i);
    }
    fclose(fd);
    fd = nullptr;
    return array;
}

int main(){
    Dane a = {}, b = {};
    wypelnij(a, 5, 'A', true, false, false, true);
    wypelnij(b, 18, 'B', false, true, true, true);
    cout << "przed kompresja" << endl;
    cout << "  a:" << endl << "    " << a << endl;
    cout << "  b:" << endl << "    " << b << endl;
    uint32_t ca = 0, cb = 0;
    Dane_compress(a, ca);
    Dane_compress(b, cb);
    cout << "po kompresji" << endl;
    cout << "    " << "ELLLLLLLLLLLLLLLLA__SSSSSSSS_O_W" << endl;
    cout << "  a:" << bitset<32>(ca) << endl;
    cout << "  a.getLiczba:" << Dane_getLiczba(ca) << endl;
    cout << "  b:" << bitset<32>(cb) << endl;
    cout << "  b.getLiczba:" << Dane_getLiczba(cb) << endl;
    // Dane_setObliczona(ca, true);
    // Dane_setObliczona(cb, false);
    Dane da = {}, db = {};
    Dane_decompress(ca, da);
    Dane_decompress(cb, db);
    cout << "po dekompresji" << endl;
    cout << "  a:" << endl << "    " << da << endl;
    cout << "    test: " << ((da == a) ? "PASS" : "FAIL") << endl;
    // cout << "  a.liczba:" << bitset<16>(da.liczba) << endl;
    // cout << "  a.liczba MASK:" << bitset<32>(DaneT::MASK::liczba) << endl;
    // cout << "  a.symbol:" << bitset<8>(da.symbol) << endl;
    // cout << "  a.symbol MASK:" << bitset<32>(DaneT::MASK::symbol) << endl;
    cout << "  b:" << endl << "    " << db << endl;
    cout << "    test: " << ((da == a) ? "PASS" : "FAIL") << endl;
    // cout << "  b.liczba:" << bitset<16>(db.liczba) << endl;
    // cout << "  b.liczba MASK:" << bitset<32>(DaneT::MASK::liczba) << endl;
    // cout << "  b.symbol:" << bitset<8>(db.symbol) << endl;
    // cout << "  b.symbol MASK:" << bitset<32>(DaneT::MASK::symbol) << endl;

    cout << endl;
    uint32_t size = 10;
    DA *array = DA_alloc(size);
    cout << "new array(" << DA_getSize(array) << "):" << endl;
    for(int i = 0; i < size; i++){
        Dane temp = {};
        Dane_decompress(array->data[i], temp);
        cout << "  " << temp << endl;
    }
    cout << endl;
    DA_set(array, da, 0);
    DA_set(array, da, 1);
    DA_set(array, db, 0);
    //              ELLLLLLLLLLLLLLLLA__SSSSSSSS_O_W
    DA_set(array, 0b10000001000100001101010010100011, 2);
    DA_set(array, 0b11111111111111111101010010100011, 3);
    cout << "mod array(" << DA_getSize(array) << "):" << endl;
    for(int i = 0; i < size; i++){
        Dane temp = {};
        Dane_decompress(array->data[i], temp);
        cout << "  " << temp << endl;
    }
    cout << endl;

    DA_writeToFile("da.txt", array);
    DA *new_array = DA_readFromFile("da.txt");
    cout << "file array(" << DA_getSize(new_array) << "):" << endl;
    for(int i = 0; i < size; i++){
        Dane temp = {};
        Dane_decompress(new_array->data[i], temp);
        cout << "  " << temp << endl;
    }
    cout << endl;


    DA_free(array);
    return 0;
}
