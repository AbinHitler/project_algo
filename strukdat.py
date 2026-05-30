import csv
from datetime import datetime

# =========================
# QUEUE MANUAL
# =========================
class Queue:
    def __init__(self):
        self.data = []

    def enqueue(self, item):
        self.data.append(item)

    def is_empty(self):
        return len(self.data) == 0

    def tampilkan(self):
        return self.data


# =========================
# HEAP MANUAL / MAX HEAP
# =========================
class MaxHeap:
    def __init__(self):
        self.data = []

    def insert(self, prioritas, id_laporan):
        self.data.append([prioritas, id_laporan])
        self.heapify_up(len(self.data) - 1)

    def heapify_up(self, index):
        while index > 0:
            parent = (index - 1) // 2

            if self.data[index][0] > self.data[parent][0]:
                self.data[index], self.data[parent] = self.data[parent], self.data[index]
                index = parent
            else:
                break

    def get_sorted_data(self):
        hasil = self.data.copy()

        for i in range(len(hasil)):
            max_index = i
            for j in range(i + 1, len(hasil)):
                if hasil[j][0] > hasil[max_index][0]:
                    max_index = j

            hasil[i], hasil[max_index] = hasil[max_index], hasil[i]

        return hasil


# =========================
# HASH TABLE MANUAL
# =========================
class HashTable:
    def __init__(self, ukuran=100):
        self.ukuran = ukuran
        self.table = [[] for _ in range(ukuran)]

    def hash_function(self, key):
        total = 0
        for huruf in key:
            total += ord(huruf)
        return total % self.ukuran

    def insert(self, key, value):
        index = self.hash_function(key)

        for i in range(len(self.table[index])):
            if self.table[index][i][0] == key:
                self.table[index][i][1] = value
                return

        self.table[index].append([key, value])

    def get(self, key):
        index = self.hash_function(key)

        for pasangan in self.table[index]:
            if pasangan[0] == key:
                return pasangan[1]

        return None

    def values(self):
        hasil = []

        for bucket in self.table:
            for pasangan in bucket:
                hasil.append(pasangan[1])

        return hasil


# =========================
# DATA PROGRAM
# =========================
USERNAME_ADMIN = "admin"
PASSWORD_ADMIN = "admin123"

bobot_kategori = {
    "Penganiayaan/Kekerasan Fisik": 70,
    "Tawuran": 60,
    "Orang Mencurigakan": 50,
    "Pencurian Ringan": 45,
    "Gangguan Ketertiban": 40,
    "Balap Liar": 35,
    "Kehilangan Barang": 25
}

bobot_darurat = {
    "Sangat Darurat": 30,
    "Cukup Darurat": 20,
    "Tidak Darurat": 10
}

bobot_bukti = {
    "Ada Bukti": 10,
    "Tidak Ada Bukti": 0
}

queue_laporan = Queue()
heap_prioritas = MaxHeap()
data_laporan = HashTable()

id_laporan = 1


# =========================
# CSV
# =========================
def simpan_csv():
    with open("laporan.csv", "w", newline="", encoding="utf-8") as file:
        writer = csv.writer(file)

        writer.writerow([
            "ID",
            "Waktu",
            "Nama",
            "NoTelp",
            "Kategori",
            "Darurat",
            "Bukti",
            "FileBukti",
            "Lokasi",
            "KetLokasi",
            "Deskripsi",
            "Prioritas",
            "Status"
        ])

        for laporan in data_laporan.values():
            writer.writerow([
                laporan["id"],
                laporan["waktu"],
                laporan["nama"],
                laporan["no_telp"],
                laporan["kategori"],
                laporan["darurat"],
                laporan["bukti"],
                laporan["file_bukti"],
                laporan["lokasi"],
                laporan["ket_lokasi"],
                laporan["deskripsi"],
                laporan["prioritas"],
                laporan["status"]
            ])


def baca_csv():
    global id_laporan

    try:
        with open("laporan.csv", "r", encoding="utf-8") as file:
            reader = csv.DictReader(file)
            max_id = 0

            for row in reader:
                laporan = {
                    "id": row["ID"],
                    "waktu": row["Waktu"],
                    "nama": row["Nama"],
                    "no_telp": row["NoTelp"],
                    "kategori": row["Kategori"],
                    "darurat": row["Darurat"],
                    "bukti": row["Bukti"],
                    "file_bukti": row["FileBukti"],
                    "lokasi": row["Lokasi"],
                    "ket_lokasi": row["KetLokasi"],
                    "deskripsi": row["Deskripsi"],
                    "prioritas": int(row["Prioritas"]),
                    "status": row["Status"]
                }

                data_laporan.insert(laporan["id"], laporan)
                queue_laporan.enqueue(laporan["id"])
                heap_prioritas.insert(laporan["prioritas"], laporan["id"])

                nomor = int(laporan["id"].replace("LP", ""))
                if nomor > max_id:
                    max_id = nomor

            id_laporan = max_id + 1

    except FileNotFoundError:
        pass


# =========================
# FUNGSI BANTUAN
# =========================
def hitung_prioritas(kategori, darurat, bukti):
    return bobot_kategori[kategori] + bobot_darurat[darurat] + bobot_bukti[bukti]


def pilih_dari_dict(data, judul):
    print(f"\n=== {judul} ===")
    daftar = list(data.keys())

    for i in range(len(daftar)):
        print(f"{i + 1}. {daftar[i]} - Bobot {data[daftar[i]]}")

    while True:
        try:
            pilihan = int(input("Pilih nomor: "))

            if 1 <= pilihan <= len(daftar):
                return daftar[pilihan - 1]
            else:
                print("Pilihan tidak valid!")

        except ValueError:
            print("Input harus angka!")


def tampilkan_detail(laporan):
    print("\n==============================")
    print("ID Laporan   :", laporan["id"])
    print("Waktu        :", laporan["waktu"])
    print("Nama         :", laporan["nama"])
    print("No Telepon   :", laporan["no_telp"])
    print("Kategori     :", laporan["kategori"])
    print("Darurat      :", laporan["darurat"])
    print("Bukti        :", laporan["bukti"])
    print("File Bukti   :", laporan["file_bukti"])
    print("Lokasi       :", laporan["lokasi"])
    print("Ket. Lokasi  :", laporan["ket_lokasi"])
    print("Deskripsi    :", laporan["deskripsi"])
    print("Prioritas    :", laporan["prioritas"])
    print("Status       :", laporan["status"])
    print("==============================")


# =========================
# MENU PELAPOR
# =========================
def menu_pelapor():
    global id_laporan

    print("\n=== FORM PELAPORAN MASYARAKAT ===")

    nama = input("Nama Pelapor       : ")
    no_telp = input("No Telepon         : ")

    kategori = pilih_dari_dict(bobot_kategori, "PILIH KATEGORI")
    darurat = pilih_dari_dict(bobot_darurat, "PILIH TINGKAT DARURAT")

    print("\n=== BUKTI FOTO / VIDEO ===")
    print("1. Ada Bukti")
    print("2. Tidak Ada Bukti")
    pilih = input("Pilih: ")

    if pilih == "1":
        bukti = "Ada Bukti"
        file_bukti = input("Masukkan nama/path file bukti: ")
    else:
        bukti = "Tidak Ada Bukti"
        file_bukti = "-"

    lokasi = input("Lokasi Desa/Kelurahan : ")
    ket_lokasi = input("Keterangan Lokasi     : ")
    deskripsi = input("Deskripsi Laporan     : ")

    prioritas = hitung_prioritas(kategori, darurat, bukti)
    id_baru = f"LP{id_laporan:03d}"
    waktu = datetime.now().strftime("%d-%m-%Y %H:%M:%S")

    laporan = {
        "id": id_baru,
        "waktu": waktu,
        "nama": nama,
        "no_telp": no_telp,
        "kategori": kategori,
        "darurat": darurat,
        "bukti": bukti,
        "file_bukti": file_bukti,
        "lokasi": lokasi,
        "ket_lokasi": ket_lokasi,
        "deskripsi": deskripsi,
        "prioritas": prioritas,
        "status": "Pending"
    }

    queue_laporan.enqueue(id_baru)
    heap_prioritas.insert(prioritas, id_baru)
    data_laporan.insert(id_baru, laporan)

    simpan_csv()

    id_laporan += 1

    print("\nLaporan berhasil dikirim!")
    print("ID Laporan     :", id_baru)
    print("Nilai Prioritas:", prioritas)
    print("Status         : Pending")


# =========================
# LOGIN ADMIN
# =========================
def login_admin():
    print("\n=== LOGIN ADMIN ===")
    username = input("Username: ")
    password = input("Password: ")

    if username == USERNAME_ADMIN and password == PASSWORD_ADMIN:
        print("Login berhasil.")
        menu_admin()
    else:
        print("Username atau password salah.")


# =========================
# ADMIN: LIHAT SEMUA LAPORAN
# =========================
def lihat_semua_laporan():
    print("\n=== SEMUA LAPORAN BERDASARKAN WAKTU MASUK / QUEUE ===")

    if queue_laporan.is_empty():
        print("Belum ada laporan.")
        return

    for id_lp in queue_laporan.tampilkan():
        laporan = data_laporan.get(id_lp)
        print(laporan["id"], "|", laporan["waktu"], "|", laporan["kategori"], "|", laporan["status"])

    while True:
        print("\n1. Pilih laporan berdasarkan ID")
        print("2. Back")
        pilih = input("Pilih: ")

        if pilih == "1":
            cari = input("Masukkan ID laporan: ").upper()
            laporan = data_laporan.get(cari)

            if laporan:
                tampilkan_detail(laporan)
            else:
                print("ID laporan tidak ditemukan.")

        elif pilih == "2":
            break
        else:
            print("Pilihan tidak valid.")


# =========================
# ADMIN: LIHAT PRIORITAS
# =========================
def lihat_laporan_prioritas():
    print("\n=== LAPORAN BERDASARKAN PRIORITAS / HEAP ===")

    data_urut = heap_prioritas.get_sorted_data()
    ada = False

    for item in data_urut:
        prioritas = item[0]
        id_lp = item[1]
        laporan = data_laporan.get(id_lp)

        if laporan["status"] == "Pending":
            print(id_lp, "| Prioritas:", prioritas, "|", laporan["kategori"], "|", laporan["status"])
            ada = True

    if not ada:
        print("Tidak ada laporan pending.")
        return

    while True:
        print("\n1. Ubah Pending menjadi Dijalankan")
        print("2. Ubah Pending menjadi Ditolak")
        print("3. Back")

        pilih = input("Pilih: ")

        if pilih == "1":
            ubah_status_pending("Dijalankan")
        elif pilih == "2":
            ubah_status_pending("Ditolak")
        elif pilih == "3":
            break
        else:
            print("Pilihan tidak valid.")


def ubah_status_pending(status_baru):
    cari = input("Masukkan ID laporan: ").upper()
    laporan = data_laporan.get(cari)

    if laporan:
        if laporan["status"] == "Pending":
            laporan["status"] = status_baru
            data_laporan.insert(cari, laporan)
            simpan_csv()
            print("Status berhasil diubah menjadi", status_baru)
        else:
            print("Laporan bukan berstatus Pending.")
    else:
        print("ID laporan tidak ditemukan.")


# =========================
# ADMIN: UBAH DIJALANKAN KE SELESAI
# =========================
def ubah_status_laporan():
    print("\n=== LAPORAN YANG SEDANG DIJALANKAN ===")

    ada = False

    for laporan in data_laporan.values():
        if laporan["status"] == "Dijalankan":
            print(laporan["id"], "|", laporan["kategori"], "|", laporan["lokasi"])
            ada = True

    if not ada:
        print("Tidak ada laporan yang sedang dijalankan.")
        return

    while True:
        print("\n1. Ubah status Dijalankan menjadi Selesai")
        print("2. Back")
        pilih = input("Pilih: ")

        if pilih == "1":
            cari = input("Masukkan ID laporan: ").upper()
            laporan = data_laporan.get(cari)

            if laporan:
                if laporan["status"] == "Dijalankan":
                    laporan["status"] = "Selesai"
                    data_laporan.insert(cari, laporan)
                    simpan_csv()
                    print("Status berhasil diubah menjadi Selesai.")
                else:
                    print("Laporan bukan berstatus Dijalankan.")
            else:
                print("ID laporan tidak ditemukan.")

        elif pilih == "2":
            break
        else:
            print("Pilihan tidak valid.")


# =========================
# ADMIN: RIWAYAT
# =========================
def lihat_riwayat():
    while True:
        print("\n=== RIWAYAT LAPORAN ===")
        print("1. Laporan Selesai")
        print("2. Laporan Ditolak")
        print("3. Back")

        pilih = input("Pilih: ")

        if pilih == "1":
            tampilkan_riwayat("Selesai")
        elif pilih == "2":
            tampilkan_riwayat("Ditolak")
        elif pilih == "3":
            break
        else:
            print("Pilihan tidak valid.")


def tampilkan_riwayat(status):
    print(f"\n=== RIWAYAT LAPORAN {status.upper()} ===")

    ada = False

    for laporan in data_laporan.values():
        if laporan["status"] == status:
            print(laporan["id"], "|", laporan["kategori"], "|", laporan["lokasi"], "|", laporan["waktu"])
            ada = True

    if not ada:
        print("Belum ada riwayat laporan", status)


# =========================
# ADMIN: BUKTI FOTO
# =========================
def lihat_bukti():
    print("\n=== LIHAT BUKTI FOTO / VIDEO LAPORAN ===")

    cari = input("Masukkan ID laporan: ").upper()
    laporan = data_laporan.get(cari)

    if laporan:
        if laporan["bukti"] == "Ada Bukti":
            print("File bukti:", laporan["file_bukti"])
        else:
            print("Laporan ini tidak memiliki bukti.")
    else:
        print("ID laporan tidak ditemukan.")


# =========================
# ADMIN: STATISTIK
# =========================
def statistik_laporan():
    print("\n=== STATISTIK LAPORAN SELESAI BY KATEGORI ===")

    statistik = {}

    for kategori in bobot_kategori:
        statistik[kategori] = 0

    for laporan in data_laporan.values():
        if laporan["status"] == "Selesai":
            statistik[laporan["kategori"]] += 1

    for kategori in statistik:
        print(kategori, ":", statistik[kategori], "laporan selesai")


# =========================
# MENU ADMIN
# =========================
def menu_admin():
    while True:
        print("\n=== MENU ADMIN ===")
        print("1. Lihat Semua Laporan")
        print("2. Lihat Laporan Berdasarkan Prioritas")
        print("3. Ubah Status Laporan")
        print("4. Lihat Riwayat Laporan")
        print("5. Lihat Bukti Foto Laporan")
        print("6. Statistik Laporan")
        print("7. Logout")

        pilih = input("Pilih menu: ")

        if pilih == "1":
            lihat_semua_laporan()
        elif pilih == "2":
            lihat_laporan_prioritas()
        elif pilih == "3":
            ubah_status_laporan()
        elif pilih == "4":
            lihat_riwayat()
        elif pilih == "5":
            lihat_bukti()
        elif pilih == "6":
            statistik_laporan()
        elif pilih == "7":
            print("Logout berhasil.")
            break
        else:
            print("Pilihan tidak valid.")


# =========================
# MENU UTAMA
# =========================
def menu_utama():
    while True:
        print("\n==============================================")
        print(" SISTEM PELAPORAN MASYARAKAT BERBASIS QUEUE")
        print("==============================================")
        print("1. Pelapor")
        print("2. Admin")
        print("3. Keluar")

        pilih = input("Pilih menu: ")

        if pilih == "1":
            menu_pelapor()
        elif pilih == "2":
            login_admin()
        elif pilih == "3":
            print("Terima kasih telah menggunakan program.")
            break
        else:
            print("Pilihan tidak valid.")


# =========================
# PROGRAM DIMULAI
# =========================
baca_csv()
menu_utama()