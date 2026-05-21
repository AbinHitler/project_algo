#ifndef OBAT_H
#define OBAT_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Obat {
    string kode;
    string nama;
    string kategori;
    int stok;
    int harga;
    int hariKedaluwarsa;
    string kebutuhan;
    int skorPrioritas;
};

extern vector<Obat> daftarObat;

int nilaiKebutuhan(string kebutuhan);
int nilaiStok(int stok);
int nilaiExpired(int hari);
int hitungSkor(Obat obat);

void tambahObat();
void tampilkanObat();
void cariObat();
void urutkanKedaluwarsa();
void cekStokHampirHabis();
void hitungSemuaSkor();
void tampilkanPrioritasTertinggi();

void simpanKeFile();
void bacaDariFile();

#endif