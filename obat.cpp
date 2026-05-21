#include "obat.h"
#include <fstream>
#include <iomanip>

vector<Obat> daftarObat;

int nilaiKebutuhan(string kebutuhan) {
    if (kebutuhan == "Tinggi" || kebutuhan == "tinggi") {
        return 40;
    } else if (kebutuhan == "Sedang" || kebutuhan == "sedang") {
        return 25;
    } else {
        return 10;
    }
}

int nilaiStok(int stok) {
    if (stok < 10) {
        return 30;
    } else if (stok <= 20) {
        return 15;
    } else {
        return 5;
    }
}

int nilaiExpired(int hari) {
    if (hari <= 30) {
        return 30;
    } else if (hari <= 90) {
        return 15;
    } else {
        return 5;
    }
}

int hitungSkor(Obat obat) {
    return nilaiKebutuhan(obat.kebutuhan)
         + nilaiStok(obat.stok)
         + nilaiExpired(obat.hariKedaluwarsa);
}

void tambahObat() {
    Obat obat;

    cout << "\n=== Tambah Data Obat ===\n";

    cout << "Kode obat: ";
    cin >> obat.kode;

    cin.ignore();
    cout << "Nama obat: ";
    getline(cin, obat.nama);

    cout << "Kategori obat: ";
    getline(cin, obat.kategori);

    cout << "Stok obat: ";
    cin >> obat.stok;

    cout << "Harga obat: ";
    cin >> obat.harga;

    cout << "Hari menuju kedaluwarsa: ";
    cin >> obat.hariKedaluwarsa;

    cout << "Tingkat kebutuhan (Tinggi/Sedang/Rendah): ";
    cin >> obat.kebutuhan;

    obat.skorPrioritas = hitungSkor(obat);

    daftarObat.push_back(obat);

    simpanKeFile();

    cout << "\nData obat berhasil ditambahkan dan disimpan ke file.\n";
}

void tampilkanObat() {
    if (daftarObat.empty()) {
        cout << "\nData obat masih kosong.\n";
        return;
    }

    cout << "\n=== Daftar Obat Apotek ===\n";

    cout << left << setw(10) << "Kode"
         << setw(20) << "Nama"
         << setw(15) << "Kategori"
         << setw(10) << "Stok"
         << setw(10) << "Harga"
         << setw(15) << "Expired"
         << setw(12) << "Kebutuhan"
         << setw(10) << "Skor" << endl;

    cout << string(100, '-') << endl;

    for (int i = 0; i < daftarObat.size(); i++) {
        cout << left << setw(10) << daftarObat[i].kode
             << setw(20) << daftarObat[i].nama
             << setw(15) << daftarObat[i].kategori
             << setw(10) << daftarObat[i].stok
             << setw(10) << daftarObat[i].harga
             << setw(15) << daftarObat[i].hariKedaluwarsa
             << setw(12) << daftarObat[i].kebutuhan
             << setw(10) << daftarObat[i].skorPrioritas
             << endl;
    }
}

void cariObat() {
    if (daftarObat.empty()) {
        cout << "\nData obat masih kosong.\n";
        return;
    }

    string keyword;
    bool ditemukan = false;

    cin.ignore();
    cout << "\nMasukkan nama atau kode obat yang dicari: ";
    getline(cin, keyword);

    cout << "\n=== Hasil Pencarian ===\n";

    for (int i = 0; i < daftarObat.size(); i++) {
        if (daftarObat[i].nama == keyword || daftarObat[i].kode == keyword) {
            cout << "Kode obat      : " << daftarObat[i].kode << endl;
            cout << "Nama obat      : " << daftarObat[i].nama << endl;
            cout << "Kategori       : " << daftarObat[i].kategori << endl;
            cout << "Stok           : " << daftarObat[i].stok << endl;
            cout << "Harga          : Rp" << daftarObat[i].harga << endl;
            cout << "Kedaluwarsa    : " << daftarObat[i].hariKedaluwarsa << " hari lagi" << endl;
            cout << "Kebutuhan      : " << daftarObat[i].kebutuhan << endl;
            cout << "Skor prioritas : " << daftarObat[i].skorPrioritas << endl;
            ditemukan = true;
        }
    }

    if (!ditemukan) {
        cout << "Obat tidak ditemukan.\n";
    }
}

void urutkanKedaluwarsa() {
    if (daftarObat.empty()) {
        cout << "\nData obat masih kosong.\n";
        return;
    }

    // Bubble Sort berdasarkan hari kedaluwarsa paling dekat
    for (int i = 0; i < daftarObat.size() - 1; i++) {
        for (int j = 0; j < daftarObat.size() - i - 1; j++) {
            if (daftarObat[j].hariKedaluwarsa > daftarObat[j + 1].hariKedaluwarsa) {
                swap(daftarObat[j], daftarObat[j + 1]);
            }
        }
    }

    cout << "\nData berhasil diurutkan berdasarkan kedaluwarsa terdekat.\n";
    tampilkanObat();
}

void cekStokHampirHabis() {
    if (daftarObat.empty()) {
        cout << "\nData obat masih kosong.\n";
        return;
    }

    int batas;
    bool ada = false;

    cout << "\nMasukkan batas minimum stok: ";
    cin >> batas;

    cout << "\n=== Obat dengan Stok Hampir Habis ===\n";

    for (int i = 0; i < daftarObat.size(); i++) {
        if (daftarObat[i].stok <= batas) {
            cout << "- " << daftarObat[i].nama
                 << " | Stok: " << daftarObat[i].stok
                 << " | Status: Perlu Restok\n";
            ada = true;
        }
    }

    if (!ada) {
        cout << "Tidak ada obat yang stoknya hampir habis.\n";
    }
}

void hitungSemuaSkor() {
    if (daftarObat.empty()) {
        cout << "\nData obat masih kosong.\n";
        return;
    }

    for (int i = 0; i < daftarObat.size(); i++) {
        daftarObat[i].skorPrioritas = hitungSkor(daftarObat[i]);
    }

    simpanKeFile();

    cout << "\nSkor prioritas semua obat berhasil dihitung ulang.\n";
    tampilkanObat();
}

void tampilkanPrioritasTertinggi() {
    if (daftarObat.empty()) {
        cout << "\nData obat masih kosong.\n";
        return;
    }

    // Bubble Sort berdasarkan skor tertinggi
    for (int i = 0; i < daftarObat.size() - 1; i++) {
        for (int j = 0; j < daftarObat.size() - i - 1; j++) {
            if (daftarObat[j].skorPrioritas < daftarObat[j + 1].skorPrioritas) {
                swap(daftarObat[j], daftarObat[j + 1]);
            }
        }
    }

    cout << "\n=== Obat Prioritas Tertinggi ===\n";

    for (int i = 0; i < daftarObat.size(); i++) {
        cout << i + 1 << ". " << daftarObat[i].nama
             << " | Stok: " << daftarObat[i].stok
             << " | Expired: " << daftarObat[i].hariKedaluwarsa << " hari lagi"
             << " | Kebutuhan: " << daftarObat[i].kebutuhan
             << " | Skor: " << daftarObat[i].skorPrioritas;

        if (daftarObat[i].skorPrioritas >= 80) {
            cout << " | Status: PRIORITAS TINGGI";
        } else if (daftarObat[i].skorPrioritas >= 50) {
            cout << " | Status: PERHATIAN";
        } else {
            cout << " | Status: AMAN";
        }

        cout << endl;
    }
}

void simpanKeFile() {
    ofstream file("data_obat.txt");

    if (!file) {
        cout << "Gagal membuka file untuk menyimpan data.\n";
        return;
    }

    for (int i = 0; i < daftarObat.size(); i++) {
        file << daftarObat[i].kode << "|"
             << daftarObat[i].nama << "|"
             << daftarObat[i].kategori << "|"
             << daftarObat[i].stok << "|"
             << daftarObat[i].harga << "|"
             << daftarObat[i].hariKedaluwarsa << "|"
             << daftarObat[i].kebutuhan << "|"
             << daftarObat[i].skorPrioritas << endl;
    }

    file.close();
}

void bacaDariFile() {
    ifstream file("data_obat.txt");

    if (!file) {
        return;
    }

    daftarObat.clear();

    Obat obat;
    string stok, harga, expired, skor;

    while (getline(file, obat.kode, '|')) {
        getline(file, obat.nama, '|');
        getline(file, obat.kategori, '|');
        getline(file, stok, '|');
        getline(file, harga, '|');
        getline(file, expired, '|');
        getline(file, obat.kebutuhan, '|');
        getline(file, skor);

        obat.stok = stoi(stok);
        obat.harga = stoi(harga);
        obat.hariKedaluwarsa = stoi(expired);
        obat.skorPrioritas = stoi(skor);

        daftarObat.push_back(obat);
    }

    file.close();
}