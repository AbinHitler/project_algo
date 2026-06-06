#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cctype>
using namespace std;

struct Obat {
    string id;
    string nama;
    int totalStok;
};

struct DetailStok {
    string jenis;
    string id;
    string nama;

    int stokLama;
    double hargaLama;
    string expiredLama;

    int stokBaru;
    double hargaBaru;
    string expiredBaru;
};

struct ItemKeranjang {
    string idObat;
    string namaObat;
    int jumlah;
    double hargaSatuan;
    string expired;
    double subtotal;
    int indexBatch;
};

vector<Obat> daftarObat;
vector<DetailStok> detailStok;
vector<ItemKeranjang> keranjang;

int nomorTransaksi = 1;

// =========================
// HELPER
// =========================
string toLowerCase(string teks) {
    for (int i = 0; i < teks.length(); i++) {
        teks[i] = tolower(teks[i]);
    }
    return teks;
}

string tanggalSekarang() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    stringstream ss;
    ss << setw(2) << setfill('0') << ltm->tm_mday << "/"
       << setw(2) << setfill('0') << ltm->tm_mon + 1 << "/"
       << ltm->tm_year + 1900 << " "
       << setw(2) << setfill('0') << ltm->tm_hour << ":"
       << setw(2) << setfill('0') << ltm->tm_min << ":"
       << setw(2) << setfill('0') << ltm->tm_sec;

    return ss.str();
}

int hitungSisaHari(string expired) {
    if (expired == "-") return 999999;

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

string generateIdTransaksi() {
    stringstream ss;
    ss << "TRX" << setw(3) << setfill('0') << nomorTransaksi++;
    return ss.str();
}

// =========================
// BACA CSV INVENTORI
// =========================
void bacaDataObatCSV() {
    ifstream file("data_obat.csv");

    if (!file.is_open()) {
        cout << "File data_obat.csv tidak ditemukan!\n";
        return;
    }

    string baris;
    getline(file, baris);

    while (getline(file, baris)) {
        stringstream ss(baris);
        Obat obat;
        string stokStr;

        getline(ss, obat.id, ',');
        getline(ss, obat.nama, ',');
        getline(ss, stokStr, ',');

        obat.totalStok = stoi(stokStr);

        daftarObat.push_back(obat);
    }

    file.close();
}

void bacaDetailStokCSV() {
    ifstream file("detail_stok_obat.csv");

    if (!file.is_open()) {
        cout << "File detail_stok_obat.csv tidak ditemukan!\n";
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

void bacaNomorTransaksi() {
    ifstream file("riwayat_transaksi.csv");

    if (!file.is_open()) {
        return;
    }

    string baris;
    getline(file, baris);

    int maxNomor = 0;

    while (getline(file, baris)) {
        stringstream ss(baris);
        string idTransaksi;

        getline(ss, idTransaksi, ',');

        if (idTransaksi.length() >= 4) {
            int nomor = stoi(idTransaksi.substr(3));
            if (nomor > maxNomor) {
                maxNomor = nomor;
            }
        }
    }

    nomorTransaksi = maxNomor + 1;
    file.close();
}

// =========================
// SIMPAN CSV
// =========================
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

void simpanRiwayatTransaksi(string idTransaksi, string tanggal, double total, double bayar, double kembalian) {
    bool fileAda = false;
    ifstream cek("riwayat_transaksi.csv");

    if (cek.good()) {
        fileAda = true;
    }

    cek.close();

    ofstream file("riwayat_transaksi.csv", ios::app);

    if (!fileAda) {
        file << "IDTransaksi,Tanggal,TotalBayar,UangBayar,Kembalian\n";
    }

    file << idTransaksi << ","
         << tanggal << ","
         << total << ","
         << bayar << ","
         << kembalian << endl;

    file.close();
}

void simpanDetailPenjualan(string idTransaksi, string tanggal) {
    bool fileAda = false;
    ifstream cek("detail_penjualan.csv");

    if (cek.good()) {
        fileAda = true;
    }

    cek.close();

    ofstream file("detail_penjualan.csv", ios::app);

    if (!fileAda) {
        file << "IDTransaksi,Tanggal,IDObat,NamaObat,JumlahTerjual,HargaSatuan,ExpiredYangDijual,Subtotal\n";
    }

    for (int i = 0; i < keranjang.size(); i++) {
        file << idTransaksi << ","
             << tanggal << ","
             << keranjang[i].idObat << ","
             << keranjang[i].namaObat << ","
             << keranjang[i].jumlah << ","
             << keranjang[i].hargaSatuan << ","
             << keranjang[i].expired << ","
             << keranjang[i].subtotal << endl;
    }

    file.close();
}

// =========================
// SEARCH OBAT BY NAMA
// =========================
int cariObatByNama() {
    string namaCari;
    vector<int> hasil;

    cin.ignore();

    cout << "Masukkan nama obat: ";
    getline(cin, namaCari);

    namaCari = toLowerCase(namaCari);

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

// =========================
// SORT BATCH BY EXPIRED TERDEKAT
// =========================
void selectionSortBatchExpired(vector<int> &batchIndex) {
    for (int i = 0; i < batchIndex.size() - 1; i++) {
        int minIndex = i;

        for (int j = i + 1; j < batchIndex.size(); j++) {
            int indexA = batchIndex[j];
            int indexB = batchIndex[minIndex];

            if (hitungSisaHari(detailStok[indexA].expiredBaru) <
                hitungSisaHari(detailStok[indexB].expiredBaru)) {
                minIndex = j;
            }
        }

        swap(batchIndex[i], batchIndex[minIndex]);
    }
}

// =========================
// CEK STOK DI KERANJANG
// =========================
int jumlahDiKeranjang(string idObat) {
    int total = 0;

    for (int i = 0; i < keranjang.size(); i++) {
        if (keranjang[i].idObat == idObat) {
            total += keranjang[i].jumlah;
        }
    }

    return total;
}

// =========================
// TRANSAKSI PENJUALAN
// =========================
void tambahObatKeKeranjang() {
    int indexObat = cariObatByNama();

    if (indexObat == -1) {
        return;
    }

    Obat obat = daftarObat[indexObat];

    cout << "\nData Obat Ditemukan\n";
    cout << "ID Obat    : " << obat.id << endl;
    cout << "Nama Obat  : " << obat.nama << endl;
    cout << "Total Stok : " << obat.totalStok << endl;

    int jumlahBeli;
    cout << "Masukkan jumlah beli: ";
    cin >> jumlahBeli;

    int stokTersedia = obat.totalStok - jumlahDiKeranjang(obat.id);

    if (jumlahBeli <= 0) {
        cout << "\nJumlah beli tidak valid!\n";
        return;
    }

    if (jumlahBeli > stokTersedia) {
        cout << "\nStok tidak mencukupi!\n";
        cout << "Stok tersedia saat ini: " << stokTersedia << endl;
        return;
    }

    vector<int> batchIndex;

    for (int i = 0; i < detailStok.size(); i++) {
        if (detailStok[i].id == obat.id && detailStok[i].stokBaru > 0) {
            batchIndex.push_back(i);
        }
    }

    selectionSortBatchExpired(batchIndex);

    int sisaBeli = jumlahBeli;

    for (int i = 0; i < batchIndex.size(); i++) {
        if (sisaBeli == 0) {
            break;
        }

        int idx = batchIndex[i];
        int ambil;

        if (detailStok[idx].stokBaru >= sisaBeli) {
            ambil = sisaBeli;
        } else {
            ambil = detailStok[idx].stokBaru;
        }

        ItemKeranjang item;
        item.idObat = obat.id;
        item.namaObat = obat.nama;
        item.jumlah = ambil;
        item.hargaSatuan = detailStok[idx].hargaBaru;
        item.expired = detailStok[idx].expiredBaru;
        item.subtotal = ambil * detailStok[idx].hargaBaru;
        item.indexBatch = idx;

        keranjang.push_back(item);

        sisaBeli -= ambil;
    }

    cout << "\nObat berhasil dimasukkan ke keranjang.\n";
}

double hitungTotalKeranjang() {
    double total = 0;

    for (int i = 0; i < keranjang.size(); i++) {
        total += keranjang[i].subtotal;
    }

    return total;
}

void tampilkanKeranjang() {
    cout << "\n=== KERANJANG BELANJA ===\n";

    if (keranjang.empty()) {
        cout << "Keranjang masih kosong.\n";
        return;
    }

    for (int i = 0; i < keranjang.size(); i++) {
        cout << i + 1 << ". "
             << keranjang[i].namaObat
             << " | Qty: " << keranjang[i].jumlah
             << " | Harga: Rp " << keranjang[i].hargaSatuan
             << " | Expired: " << keranjang[i].expired
             << " | Subtotal: Rp " << keranjang[i].subtotal << endl;
    }

    cout << "Total sementara: Rp " << hitungTotalKeranjang() << endl;
}

void kurangiStokInventory() {
    for (int i = 0; i < keranjang.size(); i++) {
        int idxBatch = keranjang[i].indexBatch;

        detailStok[idxBatch].stokBaru -= keranjang[i].jumlah;

        for (int j = 0; j < daftarObat.size(); j++) {
            if (daftarObat[j].id == keranjang[i].idObat) {
                daftarObat[j].totalStok -= keranjang[i].jumlah;
                break;
            }
        }
    }
}

void tampilkanStruk(string idTransaksi, string tanggal, double total, double bayar, double kembalian) {
    cout << "\n=====================================\n";
    cout << "              STRUK KASIR\n";
    cout << "=====================================\n";
    cout << "ID Transaksi : " << idTransaksi << endl;
    cout << "Tanggal      : " << tanggal << endl;
    cout << "-------------------------------------\n";

    for (int i = 0; i < keranjang.size(); i++) {
        cout << "Nama Obat    : " << keranjang[i].namaObat << endl;
        cout << "Qty          : " << keranjang[i].jumlah << endl;
        cout << "Harga Satuan : Rp " << keranjang[i].hargaSatuan << endl;
        cout << "Expired      : " << keranjang[i].expired << endl;
        cout << "Subtotal     : Rp " << keranjang[i].subtotal << endl;
        cout << "-------------------------------------\n";
    }

    cout << "Total        : Rp " << total << endl;
    cout << "Bayar        : Rp " << bayar << endl;
    cout << "Kembalian    : Rp " << kembalian << endl;
    cout << "=====================================\n";
    cout << "        Terima kasih telah belanja\n";
    cout << "=====================================\n";
}

void checkout() {
    if (keranjang.empty()) {
        cout << "\nKeranjang masih kosong!\n";
        return;
    }

    tampilkanKeranjang();

    double total = hitungTotalKeranjang();
    double bayar;

    cout << "\nTotal belanja : Rp " << total << endl;
    cout << "Masukkan uang bayar: Rp ";
    cin >> bayar;

    if (bayar < total) {
        cout << "\nUang tidak cukup. Transaksi dibatalkan.\n";
        return;
    }

    double kembalian = bayar - total;
    string idTransaksi = generateIdTransaksi();
    string tanggal = tanggalSekarang();

    kurangiStokInventory();

    simpanDataObatCSV();
    simpanDetailStokCSV();
    simpanRiwayatTransaksi(idTransaksi, tanggal, total, bayar, kembalian);
    simpanDetailPenjualan(idTransaksi, tanggal);

    tampilkanStruk(idTransaksi, tanggal, total, bayar, kembalian);

    keranjang.clear();
}

void transaksiPenjualan() {
    int pilihan;

    do {
        cout << "\n=== TRANSAKSI PENJUALAN ===\n";
        cout << "1. Tambah Obat ke Keranjang\n";
        cout << "2. Lihat Keranjang\n";
        cout << "3. Checkout\n";
        cout << "4. Batalkan Transaksi\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                tambahObatKeKeranjang();
                break;

            case 2:
                tampilkanKeranjang();
                break;

            case 3:
                checkout();
                return;

            case 4:
                keranjang.clear();
                cout << "\nTransaksi dibatalkan.\n";
                return;

            default:
                cout << "\nPilihan tidak valid!\n";
        }

    } while (true);
}

// =========================
// RIWAYAT TRANSAKSI
// =========================
void tampilkanRiwayatTransaksi() {
    ifstream file("riwayat_transaksi.csv");

    if (!file.is_open()) {
        cout << "\nBelum ada riwayat transaksi.\n";
        return;
    }

    file.close();

    cout << "\nMembuka file riwayat_transaksi.csv...\n";

    system("start riwayat_transaksi.csv");
}

// =========================
// MENU KASIR
// =========================
int main() {
    bacaDataObatCSV();
    bacaDetailStokCSV();
    bacaNomorTransaksi();

    int pilihan;

    do {
        cout << "\n=====================================\n";
        cout << "          SISTEM KASIR APOTEK\n";
        cout << "=====================================\n";
        cout << "1. Transaksi Penjualan\n";
        cout << "2. Riwayat Transaksi\n";
        cout << "3. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                transaksiPenjualan();
                break;

            case 2:
                tampilkanRiwayatTransaksi();
                break;

            case 3:
                cout << "\nTerima kasih telah menggunakan sistem kasir.\n";
                break;

            default:
                cout << "\nMenu tidak tersedia!\n";
        }

    } while (pilihan != 3);

    return 0;
}