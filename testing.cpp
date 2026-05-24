#include <iostream>
#include <ctime>
#include <iomanip>

using namespace std;

int main() {

    // Tanggal produksi
    int tglProd, blnProd, thnProd;

    // Tanggal expired
    int tglExp, blnExp, thnExp;

    cout << "=== PROGRAM HITUNG MASA EXPIRED PRODUK ===\n\n";

    // Input tanggal produksi
    cout << "Masukkan tanggal produksi\n";
    cout << "Tanggal : ";
    cin >> tglProd;

    cout << "Bulan   : ";
    cin >> blnProd;

    cout << "Tahun   : ";
    cin >> thnProd;

    // Input tanggal expired
    cout << "\nMasukkan tanggal expired\n";
    cout << "Tanggal : ";
    cin >> tglExp;

    cout << "Bulan   : ";
    cin >> blnExp;

    cout << "Tahun   : ";
    cin >> thnExp;

    // Struct waktu produksi
    tm produksi = {};
    produksi.tm_mday = tglProd;
    produksi.tm_mon  = blnProd - 1;
    produksi.tm_year = thnProd - 1900;

    // Struct waktu expired
    tm expired = {};
    expired.tm_mday = tglExp;
    expired.tm_mon  = blnExp - 1;
    expired.tm_year = thnExp - 1900;

    // Convert ke time_t
    time_t waktuProduksi = mktime(&produksi);
    time_t waktuExpired  = mktime(&expired);

    // Hitung selisih detik
    double selisihDetik = difftime(waktuExpired, waktuProduksi);

    // Convert ke hari
    int selisihHari = selisihDetik / (60 * 60 * 24);

    // Convert ke tahun, bulan, hari
    int tahun = selisihHari / 365;
    int sisaHari = selisihHari % 365;

    int bulan = sisaHari / 30;
    int hari = sisaHari % 30;

    cout << "\n=== HASIL ===\n";

    cout << "Tanggal Produksi : "
         << setfill('0')
         << setw(2) << tglProd << "-"
         << setw(2) << blnProd << "-"
         << thnProd << endl;

    cout << "Tanggal Expired  : "
         << setw(2) << tglExp << "-"
         << setw(2) << blnExp << "-"
         << thnExp << endl;

    cout << "\nMasa expired produk:\n";
    cout << tahun << " tahun "
         << bulan << " bulan "
         << hari << " hari\n";

    return 0;
}