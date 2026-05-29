#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cctype>
using namespace std;

struct Obat {
    string id;
    string nama;
    int stok;
    double harga;
    string expired;
};

vector<Obat> daftarObat;
int nomorId = 1;

string generateId() {
    stringstream ss;
    ss << "OBT" << setw(3) << setfill('0') << nomorId++;
    return ss.str();
}

int hitungSisaHari(string expired) {
    int hari, bulan, tahun;
    char slash1, slash2;

    stringstream ss(expired);
    ss >> hari >> slash1 >> bulan >> slash2 >> tahun;

    time_t sekarang = time(0);
    tm *waktuSekarang = localtime(&sekarang);

    tm tanggalExpired = {};
    tanggalExpired.tm_mday = hari;
    tanggalExpired.tm_mon = bulan - 1;
    tanggalExpired.tm_year = tahun - 1900;

    time_t waktuExpired = mktime(&tanggalExpired);

    double selisih = difftime(waktuExpired, sekarang);

    return selisih / (60 * 60 * 24);
}

void simpanKeCSV() {
    ofstream file("data_obat.csv");

    file << "ID,Nama,Stok,Harga,Expired\n";

    for (int i = 0; i < daftarObat.size(); i++) {
        file << daftarObat[i].id << ","
             << daftarObat[i].nama << ","
             << daftarObat[i].stok << ","
             << daftarObat[i].harga << ","
             << daftarObat[i].expired << endl;
    }

    file.close();
}

void bacaDariCSV() {
    ifstream file("data_obat.csv");

    if (!file.is_open()) {
        return;
    }

    string baris;
    getline(file, baris);

    while (getline(file, baris)) {
        stringstream ss(baris);
        Obat obat;
        string stokStr, hargaStr;

        getline(ss, obat.id, ',');
        getline(ss, obat.nama, ',');
        getline(ss, stokStr, ',');
        getline(ss, hargaStr, ',');
        getline(ss, obat.expired, ',');

        obat.stok = stoi(stokStr);
        obat.harga = stod(hargaStr);

        daftarObat.push_back(obat);

        int angkaId = stoi(obat.id.substr(3));
        if (angkaId >= nomorId) {
            nomorId = angkaId + 1;
        }
    }

    file.close();
}

void daftarkanObat() {
    Obat obat;
    obat.id = generateId();

    cout << "\n=== DAFTARKAN OBAT ===\n";
    cout << "ID Obat        : " << obat.id << endl;

    cin.ignore();

    cout << "Nama Obat      : ";
    getline(cin, obat.nama);

    cout << "Stok Obat      : ";
    cin >> obat.stok;

    cout << "Harga Obat     : ";
    cin >> obat.harga;

    cin.ignore();

    cout << "Tanggal Expired (dd/mm/yyyy): ";
    getline(cin, obat.expired);

    daftarObat.push_back(obat);
    simpanKeCSV();

    cout << "\nObat berhasil didaftarkan dan disimpan ke data_obat.csv!\n";
}

void tambahStok() {
    string namaCari;
    int tambahan;
    bool ditemukan = false;

    cout << "\n=== TAMBAH STOK ===\n";
    cin.ignore();

    cout << "Masukkan Nama Obat : ";
    getline(cin, namaCari);

    // Agar pencarian nama tidak case sensitive
    for (int i = 0; i < namaCari.length(); i++) {
        namaCari[i] = tolower(namaCari[i]);
    }

    // Linear Search
    for (int i = 0; i < daftarObat.size(); i++) {

        string namaObat = daftarObat[i].nama;

        for (int j = 0; j < namaObat.length(); j++) {
            namaObat[j] = tolower(namaObat[j]);
        }

        if (namaObat == namaCari) {
            cout << "\nData Obat Ditemukan\n";
            cout << "ID Obat   : " << daftarObat[i].id << endl;
            cout << "Nama      : " << daftarObat[i].nama << endl;
            cout << "Stok Awal : " << daftarObat[i].stok << endl;

            cout << "\nMasukkan Tambahan Stok : ";
            cin >> tambahan;

            daftarObat[i].stok += tambahan;

            simpanKeCSV();

            cout << "\nStok berhasil ditambahkan!\n";
            cout << "Stok Sekarang : " << daftarObat[i].stok << endl;

            ditemukan = true;
            break;
        }
    }

    if (!ditemukan) {
        cout << "\nObat dengan nama tersebut tidak ditemukan!\n";
    }
}

void hapusObat() {
    string namaCari;
    char konfirmasi;
    bool ditemukan = false;

    cout << "\n=== HAPUS OBAT ===\n";
    cin.ignore();

    cout << "Masukkan Nama Obat : ";
    getline(cin, namaCari);

    // Ubah input user menjadi huruf kecil
    for (int i = 0; i < namaCari.length(); i++) {
        namaCari[i] = tolower(namaCari[i]);
    }

    // Linear Search berdasarkan nama obat
    for (int i = 0; i < daftarObat.size(); i++) {

        string namaObat = daftarObat[i].nama;

        // Ubah nama obat dari vector menjadi huruf kecil
        for (int j = 0; j < namaObat.length(); j++) {
            namaObat[j] = tolower(namaObat[j]);
        }

        if (namaObat == namaCari) {
            cout << "\nData Obat Ditemukan\n";
            cout << "ID Obat   : " << daftarObat[i].id << endl;
            cout << "Nama      : " << daftarObat[i].nama << endl;
            cout << "Stok      : " << daftarObat[i].stok << endl;
            cout << "Harga     : Rp " << daftarObat[i].harga << endl;
            cout << "Expired   : " << daftarObat[i].expired << endl;

            cout << "\nApakah anda yakin ingin menghapus obat? (y/n): ";
            cin >> konfirmasi;

            konfirmasi = tolower(konfirmasi);

            if (konfirmasi == 'y') {
                daftarObat.erase(daftarObat.begin() + i);
                simpanKeCSV();

                cout << "\nObat berhasil dihapus dan data CSV diperbarui!\n";
            } 
            else if (konfirmasi == 'n') {
                cout << "\nPenghapusan obat dibatalkan.\n";
            } 
            else {
                cout << "\nInput konfirmasi tidak valid!\n";
            }

            ditemukan = true;
            break;
        }
    }

    if (!ditemukan) {
        cout << "\nObat dengan nama tersebut tidak ditemukan!\n";
    }
}

void cetakDataObat(vector<Obat> data) {
    if (data.empty()) {
        cout << "Belum ada data obat.\n";
        return;
    }

    for (int i = 0; i < data.size(); i++) {
        int sisaHari = hitungSisaHari(data[i].expired);

        cout << "\n----------------------------\n";
        cout << "ID Obat   : " << data[i].id << endl;
        cout << "Nama      : " << data[i].nama << endl;
        cout << "Stok      : " << data[i].stok << endl;
        cout << "Harga     : Rp " << data[i].harga << endl;
        cout << "Expired   : " << data[i].expired << endl;

        if (sisaHari >= 0) {
            cout << "Sisa Hari : " << sisaHari << " hari lagi\n";
        } else {
            cout << "Sisa Hari : Sudah kadaluarsa\n";
        }
    }
}

void selectionSortStokSedikit(vector<Obat> &data) {
    for (int i = 0; i < data.size() - 1; i++) {
        int minIndex = i;

        for (int j = i + 1; j < data.size(); j++) {
            if (data[j].stok < data[minIndex].stok) {
                minIndex = j;
            }
        }

        swap(data[i], data[minIndex]);
    }
}

void selectionSortStokBanyak(vector<Obat> &data) {
    for (int i = 0; i < data.size() - 1; i++) {
        int maxIndex = i;

        for (int j = i + 1; j < data.size(); j++) {
            if (data[j].stok > data[maxIndex].stok) {
                maxIndex = j;
            }
        }

        swap(data[i], data[maxIndex]);
    }
}

void selectionSortExpiredTerdekat(vector<Obat> &data) {
    for (int i = 0; i < data.size() - 1; i++) {
        int minIndex = i;

        for (int j = i + 1; j < data.size(); j++) {
            if (hitungSisaHari(data[j].expired) < hitungSisaHari(data[minIndex].expired)) {
                minIndex = j;
            }
        }

        swap(data[i], data[minIndex]);
    }
}

void tampilkanSemuaObat() {
    int pilihan;
    vector<Obat> dataUrut = daftarObat;

    cout << "\n=== TAMPILKAN SEMUA OBAT ===\n";
    cout << "1. Tampilkan Biasa\n";
    cout << "2. Urutkan Stok Paling Sedikit\n";
    cout << "3. Urutkan Stok Paling Banyak\n";
    cout << "4. Urutkan Tanggal Kadaluarsa Terdekat\n";
    cout << "Pilih Menu : ";
    cin >> pilihan;

    switch (pilihan) {
        case 1:
            cetakDataObat(dataUrut);
            break;

        case 2:
            selectionSortStokSedikit(dataUrut);
            cetakDataObat(dataUrut);
            break;

        case 3:
            selectionSortStokBanyak(dataUrut);
            cetakDataObat(dataUrut);
            break;

        case 4:
            selectionSortExpiredTerdekat(dataUrut);
            cetakDataObat(dataUrut);
            break;

        default:
            cout << "Pilihan tidak valid!\n";
    }
}

int main() {
    bacaDariCSV();

    int pilihan;

    do {
        cout << "\n=====================================\n";
        cout << " SISTEM INVENTORI OBAT APOTEK\n";
        cout << "=====================================\n";
        cout << "1. Daftarkan Obat\n";
        cout << "2. Tambah Stok\n";
        cout << "3. Hapus Obat\n";
        cout << "4. Tampilkan Semua Obat\n";
        cout << "5. Keluar\n";
        cout << "Pilih Menu : ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                daftarkanObat();
                break;
            case 2:
                tambahStok();
                break;
            case 3:
                hapusObat();
                break;
            case 4:
                tampilkanSemuaObat();
                break;
            case 5:
                cout << "\nTerima kasih telah menggunakan program.\n";
                break;
            default:
                cout << "\nMenu tidak tersedia!\n";
        }

    } while (pilihan != 5);

    return 0;
}