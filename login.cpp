#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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
    string roleCSV;
    string hashCSV;

    cout << "\n=== LOGIN " << role << " ===\n";
    cout << "Masukkan Password : ";
    cin >> passwordInput;

    string hashInput = hashPassword(passwordInput);

    ifstream file("users.csv");

    if (!file.is_open()) {
        cout << "\nFile users.csv tidak ditemukan!\n";
        return false;
    }

    string baris;

    getline(file, baris); // skip header

    while (getline(file, baris)) {
        stringstream ss(baris);

        getline(ss, roleCSV, ',');
        getline(ss, hashCSV, ',');

        if (roleCSV == role) {
            if (hashCSV == hashInput) {
                cout << "\nLogin berhasil!\n";
                file.close();
                return true;
            } else {
                cout << "\nPassword salah!\n";
                file.close();
                return false;
            }
        }
    }

    file.close();

    cout << "\nRole tidak ditemukan!\n";
    return false;
}

void menuKasir() {
    cout << "\n==================================";
    cout << "\n     SELAMAT DATANG KASIR";
    cout << "\n==================================\n";

    cout << "Nanti menu kasir disini...\n";
}

void menuInventory() {
    cout << "\n==================================";
    cout << "\n   SELAMAT DATANG INVENTORY";
    cout << "\n==================================\n";

    cout << "Nanti menu inventory disini...\n";
}

int main() {

    buatUserDefault();

    int pilihan;

    do {

        cout << "\n=====================================\n";
        cout << "         SISTEM APOTEK\n";
        cout << "=====================================\n";
        cout << "1. Login as Kasir\n";
        cout << "2. Login as Inventory\n";
        cout << "3. Keluar\n";
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

                cout << "\nTerima kasih telah menggunakan sistem.\n";
                break;

            default:

                cout << "\nMenu tidak tersedia!\n";
        }

    } while (pilihan != 3);

    return 0;
}