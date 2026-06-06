#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <ctime>
using namespace std;

struct Obat {
    string id, nama, expiredTerakhir;
    int totalStok, stokTerakhir;
    double hargaTerakhir;
};

struct DetailStok {
    string jenis, id, nama, expiredLama, expiredBaru;
    int stokLama, stokBaru;
    double hargaLama, hargaBaru;
};

vector<Obat> daftarObat;
vector<DetailStok> detailStok;
int nomorId = 1;

string generateId() {
    stringstream ss;
    ss << "OBT" << setw(3) << setfill('0') << nomorId++;
    return ss.str();
}

string toLowerCase(string teks) {
    for (int i = 0; i < teks.length(); i++) {
        teks[i] = tolower(teks[i]);
    }
    return teks;
}

int cariObatByNama() {
    string namaCari;
    vector<int> hasil;

    cin.ignore();

    cout << "Masukkan Nama Obat : ";
    getline(cin, namaCari);

    namaCari = toLowerCase(namaCari);

    // Linear Search partial match
    for (int i = 0; i < daftarObat.size(); i++) {
        string namaObat = toLowerCase(daftarObat[i].nama);

        if (namaObat.find(namaCari) != string::npos) {
            hasil.push_back(i);
        }
    }

    if (hasil.empty()) {
        cout << "\nObat tidak ditemukan!\n";
        return -1;
    }

    if (hasil.size() == 1) {
        return hasil[0];
    }

    cout << "\nDitemukan beberapa obat:\n";
    for (int i = 0; i < hasil.size(); i++) {
        int index = hasil[i];

        cout << i + 1 << ". "
             << daftarObat[index].id << " - "
             << daftarObat[index].nama
             << " | Total Stok: "
             << daftarObat[index].totalStok << endl;
    }

    int pilihan;
    cout << "\nPilih nomor obat: ";
    cin >> pilihan;

    if (pilihan < 1 || pilihan > hasil.size()) {
        cout << "Pilihan tidak valid!\n";
        return -1;
    }

    return hasil[pilihan - 1];
}

void simpanDataObatCSV() {
    ofstream file("data_obat.csv");

    file << "ID,Nama,TotalStok\n";

    for (int i = 0; i < daftarObat.size(); i++) {
        file << daftarObat[i].id << ","
             << daftarObat[i].nama << ","
             << daftarObat[i].totalStok << endl;
    }

    file.close();
}

void simpanDetailStokCSV() {
    ofstream file("detail_stok_obat.csv");

    file << "Jenis,ID,Nama,StokLama,HargaLama,ExpiredLama,StokBaru,HargaBaru,ExpiredBaru\n";

    for (int i = 0; i < detailStok.size(); i++) {
        file << detailStok[i].jenis << ","
             << detailStok[i].id << ","
             << detailStok[i].nama << ","
             << detailStok[i].stokLama << ","
             << detailStok[i].hargaLama << ","
             << detailStok[i].expiredLama << ","
             << detailStok[i].stokBaru << ","
             << detailStok[i].hargaBaru << ","
             << detailStok[i].expiredBaru << endl;
    }

    file.close();
}

void bacaDataObatCSV() {
    ifstream file("data_obat.csv");

    if (!file.is_open()) {
        return;
    }

    string baris;
    getline(file, baris);

    while (getline(file, baris)) {
        stringstream ss(baris);
        Obat obat;
        string totalStokStr;

        getline(ss, obat.id, ',');
        getline(ss, obat.nama, ',');
        getline(ss, totalStokStr, ',');

        obat.totalStok = stoi(totalStokStr);
        obat.stokTerakhir = 0;
        obat.hargaTerakhir = 0;
        obat.expiredTerakhir = "-";

        daftarObat.push_back(obat);

        int angkaId = stoi(obat.id.substr(3));

        if (angkaId >= nomorId) {
            nomorId = angkaId + 1;
        }
    }

    file.close();
}

void bacaDetailStokCSV() {
    ifstream file("detail_stok_obat.csv");

    if (!file.is_open()) {
        return;
    }

    string baris;
    getline(file, baris);

    while (getline(file, baris)) {
        stringstream ss(baris);
        DetailStok detail;
        string stokLamaStr, hargaLamaStr, stokBaruStr, hargaBaruStr;

        getline(ss, detail.jenis, ',');
        getline(ss, detail.id, ',');
        getline(ss, detail.nama, ',');
        getline(ss, stokLamaStr, ',');
        getline(ss, hargaLamaStr, ',');
        getline(ss, detail.expiredLama, ',');
        getline(ss, stokBaruStr, ',');
        getline(ss, hargaBaruStr, ',');
        getline(ss, detail.expiredBaru, ',');

        detail.stokLama = stoi(stokLamaStr);
        detail.hargaLama = stod(hargaLamaStr);
        detail.stokBaru = stoi(stokBaruStr);
        detail.hargaBaru = stod(hargaBaruStr);

        detailStok.push_back(detail);
    }

    file.close();
}

void updateDataObatDariDetail() {
    for (int i = 0; i < daftarObat.size(); i++) {
        for (int j = detailStok.size() - 1; j >= 0; j--) {
            if (daftarObat[i].id == detailStok[j].id) {

                if (detailStok[j].jenis == "Daftar Obat") {
                    daftarObat[i].stokTerakhir = detailStok[j].stokLama;
                    daftarObat[i].hargaTerakhir = detailStok[j].hargaLama;
                    daftarObat[i].expiredTerakhir = detailStok[j].expiredLama;
                } else {
                    daftarObat[i].stokTerakhir = detailStok[j].stokBaru;
                    daftarObat[i].hargaTerakhir = detailStok[j].hargaBaru;
                    daftarObat[i].expiredTerakhir = detailStok[j].expiredBaru;
                }

                break;
            }
        }
    }
}

void daftarkanObat() {
    Obat obat;
    string namaInput;

    cout << "\n=== DAFTARKAN OBAT ===\n";

    cin.ignore();

    cout << "Nama Obat : ";
    getline(cin, namaInput);

    string namaInputLower = toLowerCase(namaInput);

    // Cek apakah nama obat sudah terdaftar
    for (int i = 0; i < daftarObat.size(); i++) {
        string namaObatLower = toLowerCase(daftarObat[i].nama);

        if (namaObatLower == namaInputLower) {
            cout << "\nNama obat ini sudah terdaftar!\n";
            cout << "Silakan gunakan menu Tambah Stok untuk restock obat tersebut.\n";
            return;
        }
    }

    obat.id = generateId();
    obat.nama = namaInput;
    obat.totalStok = 0;
    obat.stokTerakhir = 0;
    obat.hargaTerakhir = 0;
    obat.expiredTerakhir = "-";

    daftarObat.push_back(obat);

    simpanDataObatCSV();

    cout << "\nObat berhasil didaftarkan!\n";
    cout << "ID Obat : " << obat.id << endl;
    cout << "Silakan gunakan menu Tambah Stok untuk mengisi stok, harga, dan expired.\n";
}

void tambahStok() {
    string expiredBaru;
    int stokTambahan;
    double hargaBaru;

    cout << "\n=== TAMBAH STOK OBAT ===\n";

    int index = cariObatByNama();

    if (index == -1) {
        return;
    }

    cout << "\nData Obat Ditemukan\n";
    cout << "ID Obat       : " << daftarObat[index].id << endl;
    cout << "Nama Obat     : " << daftarObat[index].nama << endl;
    cout << "Total Stok    : " << daftarObat[index].totalStok << endl;
    cout << "Stok Lama     : " << daftarObat[index].stokTerakhir << endl;
    cout << "Harga Lama    : Rp " << daftarObat[index].hargaTerakhir << endl;
    cout << "Expired Lama  : " << daftarObat[index].expiredTerakhir << endl;

    cout << "\nMasukkan Stok yang Ingin Ditambahkan : ";
    cin >> stokTambahan;

    cout << "Masukkan Harga Baru                  : ";
    cin >> hargaBaru;

    cin.ignore();

    cout << "Masukkan Expired Baru (dd/mm/yyyy)   : ";
    getline(cin, expiredBaru);

DetailStok detail;

detail.jenis = "Tambah Stok";
detail.id = daftarObat[index].id;
detail.nama = daftarObat[index].nama;

detail.stokLama = daftarObat[index].stokTerakhir;
detail.hargaLama = daftarObat[index].hargaTerakhir;
detail.expiredLama = daftarObat[index].expiredTerakhir;

detail.stokBaru = stokTambahan;
detail.hargaBaru = hargaBaru;
detail.expiredBaru = expiredBaru;

    detailStok.push_back(detail);

    daftarObat[index].totalStok += stokTambahan;
    daftarObat[index].stokTerakhir = stokTambahan;
    daftarObat[index].hargaTerakhir = hargaBaru;
    daftarObat[index].expiredTerakhir = expiredBaru;

    simpanDataObatCSV();
    simpanDetailStokCSV();

    cout << "\nStok berhasil ditambahkan!\n";
    cout << "Total stok sekarang: " << daftarObat[index].totalStok << endl;
}

void hapusObat() {
    char konfirmasi;

    cout << "\n=== HAPUS OBAT ===\n";

    int index = cariObatByNama();

    if (index == -1) {
        return;
    }

    cout << "\nData Obat Ditemukan\n";
    cout << "ID Obat    : " << daftarObat[index].id << endl;
    cout << "Nama Obat  : " << daftarObat[index].nama << endl;
    cout << "Total Stok : " << daftarObat[index].totalStok << endl;

    cout << "\nApakah anda yakin ingin menghapus obat? (y/n): ";
    cin >> konfirmasi;

    konfirmasi = tolower(konfirmasi);

    if (konfirmasi == 'y') {
        string idHapus = daftarObat[index].id;

        daftarObat.erase(daftarObat.begin() + index);

        for (int j = 0; j < detailStok.size(); j++) {
            if (detailStok[j].id == idHapus) {
                detailStok.erase(detailStok.begin() + j);
                j--;
            }
        }

        simpanDataObatCSV();
        simpanDetailStokCSV();

        cout << "\nObat berhasil dihapus!\n";
    } else if (konfirmasi == 'n') {
        cout << "\nPenghapusan obat dibatalkan.\n";
    } else {
        cout << "\nInput tidak valid. Penghapusan dibatalkan.\n";
    }
}

int hitungSisaHari(string expired) {
    if (expired == "-") {
        return 999999;
    }

    int hari, bulan, tahun;
    char slash1, slash2;

    stringstream ss(expired);
    ss >> hari >> slash1 >> bulan >> slash2 >> tahun;

    time_t sekarang = time(0);

    tm tanggalExpired = {};
    tanggalExpired.tm_mday = hari;
    tanggalExpired.tm_mon = bulan - 1;
    tanggalExpired.tm_year = tahun - 1900;
    tanggalExpired.tm_hour = 0;
    tanggalExpired.tm_min = 0;
    tanggalExpired.tm_sec = 0;

    time_t waktuExpired = mktime(&tanggalExpired);

    double selisih = difftime(waktuExpired, sekarang);

    return selisih / (60 * 60 * 24);
}

void selectionSortStokSedikit(vector<Obat> &data) {
    for (int i = 0; i < data.size() - 1; i++) {
        int minIndex = i;

        for (int j = i + 1; j < data.size(); j++) {
            if (data[j].totalStok < data[minIndex].totalStok) {
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
            if (data[j].totalStok > data[maxIndex].totalStok) {
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
            if (hitungSisaHari(data[j].expiredTerakhir) < hitungSisaHari(data[minIndex].expiredTerakhir)) {
                minIndex = j;
            }
        }

        swap(data[i], data[minIndex]);
    }
}

void cetakDataObat(vector<Obat> data) {
    if (data.empty()) {
        cout << "Belum ada data obat.\n";
        return;
    }

    for (int i = 0; i < data.size(); i++) {
        int sisaHari = hitungSisaHari(data[i].expiredTerakhir);

        cout << "\n----------------------------\n";
        cout << "ID Obat        : " << data[i].id << endl;
        cout << "Nama Obat      : " << data[i].nama << endl;
        cout << "Total Stok     : " << data[i].totalStok << endl;
        cout << "Harga Terakhir : Rp " << data[i].hargaTerakhir << endl;
        cout << "Expired        : " << data[i].expiredTerakhir << endl;

        if (sisaHari == 999999) {
            cout << "Sisa Expired   : Belum ada stok/expired\n";
        } else if (sisaHari >= 0) {
            cout << "Sisa Expired   : " << sisaHari << " hari lagi\n";
        } else {
            cout << "Sisa Expired   : Sudah kadaluarsa\n";
        }
    }
}

void tampilkanSemuaObat() {
    int pilihan;
    vector<Obat> dataUrut = daftarObat;

    cout << "\n=== TAMPILKAN SEMUA OBAT ===\n";

    if (daftarObat.empty()) {
        cout << "Belum ada data obat.\n";
        return;
    }

    cout << "1. Tampilkan Biasa (Urut by ID Obat)\n";
    cout << "2. Tampilkan by Stok Paling Sedikit\n";
    cout << "3. Tampilkan by Stok Paling Banyak\n";
    cout << "4. Tampilkan by Tanggal Expired Terdekat\n";
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
    bacaDataObatCSV();
    bacaDetailStokCSV();
    updateDataObatDariDetail();

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