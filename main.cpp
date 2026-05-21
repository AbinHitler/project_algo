#include <iostream>
#include "obat.h"
using namespace std;

void tampilkanMenu() {
    cout << "\n========================================\n";
    cout << " SISTEM PRIORITAS STOK OBAT APOTEK\n";
    cout << "========================================\n";
    cout << "1. Tambah Data Obat\n";
    cout << "2. Tampilkan Semua Obat\n";
    cout << "3. Cari Obat\n";
    cout << "4. Urutkan Obat Berdasarkan Kedaluwarsa\n";
    cout << "5. Cek Obat Stok Hampir Habis\n";
    cout << "6. Hitung Skor Prioritas Obat\n";
    cout << "7. Tampilkan Obat Prioritas Tertinggi\n";
    cout << "0. Keluar\n";
    cout << "Pilih menu: ";
}

int main() {
    int pilihan;

    bacaDariFile();

    do {
        tampilkanMenu();
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                tambahObat();
                break;

            case 2:
                tampilkanObat();
                break;

            case 3:
                cariObat();
                break;

            case 4:
                urutkanKedaluwarsa();
                break;

            case 5:
                cekStokHampirHabis();
                break;

            case 6:
                hitungSemuaSkor();
                break;

            case 7:
                tampilkanPrioritasTertinggi();
                break;

            case 0:
                simpanKeFile();
                cout << "\nData berhasil disimpan. Program selesai.\n";
                break;

            default:
                cout << "\nPilihan tidak valid.\n";
        }

    } while (pilihan != 0);

    return 0;
}