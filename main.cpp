#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>

#include "kasir.h"
#include "inventory.h"

using namespace std;

string hashPassword(string password) {
    unsigned long long hash = 0;

    for (int i = 0; i < password.length(); i++) {
        hash = hash * 31 + password[i];
    }

    return to_string(hash);
}

void buatUserDefault() {
    ifstream cekFile("users.csv");

    if (cekFile.is_open()) {
        cekFile.close();
        return;
    }

    ofstream file("users.csv");

    file << "role,password_hash\n";
    file << "kasir," << hashPassword("kasir123") << endl;
    file << "inventory," << hashPassword("inv123") << endl;

    file.close();
}

bool login(string role) {
    string passwordInput;
    string roleCSV, hashCSV;

    cout << "\n=====================================\n";
    cout << "          LOGIN " << role << endl;
    cout << "=====================================\n";
    cout << "Masukkan Password : ";
    cin >> passwordInput;

    string hashInput = hashPassword(passwordInput);

    ifstream file("users.csv");

    if (!file.is_open()) {
        cout << "\nFile users.csv tidak ditemukan!\n";
        return false;
    }

    string baris;
    getline(file, baris);

    while (getline(file, baris)) {
        stringstream ss(baris);

        getline(ss, roleCSV, ',');
        getline(ss, hashCSV, ',');

        if (roleCSV == role) {
            file.close();

            if (hashCSV == hashInput) {
                cout << "\nLogin berhasil sebagai " << role << "!\n";
                return true;
            } else {
                cout << "\nPassword salah!\n";
                return false;
            }
        }
    }

    file.close();

    cout << "\nRole tidak ditemukan!\n";
    return false;
}

bool konfirmasiKeluar() {
    char jawab;

    cout << "\nApakah anda yakin ingin keluar dari sistem? (y/n): ";
    cin >> jawab;

    jawab = tolower(jawab);

    if (jawab == 'y') {
        return true;
    } else if (jawab == 'n') {
        cout << "\nKeluar sistem dibatalkan.\n";
        return false;
    } else {
        cout << "\nInput tidak valid. Keluar sistem dibatalkan.\n";
        return false;
    }
}

int main() {
    buatUserDefault();

    int pilihan;
    bool keluar = false;

    do {
        cout << "\n=====================================\n";
        cout << "          SISTEM APOTEK\n";
        cout << "=====================================\n";
        cout << "1. Login as Kasir\n";
        cout << "2. Login as Inventory\n";
        cout << "3. Keluar Sistem\n";
        cout << "=====================================\n";
        cout << "Pilih Menu : ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                if (login("kasir")) {
                    menuKasir();
                }
                break;

            case 2:
                if (login("inventory")) {
                    menuInventory();
                }
                break;

            case 3:
                if (konfirmasiKeluar()) {
                    cout << "\nTerima kasih telah menggunakan sistem.\n";
                    keluar = true;
                }
                break;

            default:
                cout << "\nMenu tidak tersedia!\n";
        }

    } while (!keluar);

    return 0;
}