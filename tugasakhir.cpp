#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

// Ukuran kursi kereta
const int JUMLAH_BARIS = 7, JUMLAH_KOLOM = 10;

// ===== Struktur Data =====
struct Kursi {
    string namaPemesan;
};

struct Kereta {
    string namaKereta;
    Kursi kursi[JUMLAH_BARIS][JUMLAH_KOLOM];
    Kereta* berikutnya;
};

Kereta* kepala = nullptr;

// ===== Simpan & Muat Data =====
void simpanDataKeFile() {
    ofstream file("data_kereta.txt");
    Kereta* temp = kepala;
    while (temp) {
        file << temp->namaKereta << endl;
        for (int i = 0; i < JUMLAH_BARIS; i++)
            for (int j = 0; j < JUMLAH_KOLOM; j++)
                file << temp->kursi[i][j].namaPemesan << endl;
        temp = temp->berikutnya;
    }
    file.close();
}

void muatDataDariFile() {
    ifstream file("data_kereta.txt");
    if (!file) return;
    while (!file.eof()) {
        Kereta* baru = new Kereta;
        getline(file, baru->namaKereta);
        if (baru->namaKereta.empty()) {
            delete baru;
            break;
        }
        for (int i = 0; i < JUMLAH_BARIS; i++)
            for (int j = 0; j < JUMLAH_KOLOM; j++)
                getline(file, baru->kursi[i][j].namaPemesan);
        baru->berikutnya = nullptr;
        if (!kepala) kepala = baru;
        else {
            Kereta* temp = kepala;
            while (temp->berikutnya) temp = temp->berikutnya;
            temp->berikutnya = baru;
        }
    }
    file.close();
}

// ===== Login Admin =====
int prosesLogin() {
    system("cls");
    string username, password;
    cout << "=== Login ===\n\n";
    cout << "Selamat Datang di Sistem Pemesanan Kereta Nirel\n\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    if (username == "nicochan" && password == "nicochan123") {
        cout << "\nLogin berhasil.\n";
        cin.ignore(); cin.get();
        return 1;
    }
    cout << "\nLogin gagal. Username atau password salah.\n";
    cin.ignore(); cin.get();
    return 0;
}

// ===== Tambah Kereta =====
void tambahKereta() {
    system("cls");
    Kereta* baru = new Kereta;
    cout << "=== Tambah Kereta ===\n";
    cout << "Nama Kereta: ";
    cin.ignore();
    getline(cin, baru->namaKereta);
    baru->berikutnya = nullptr;

    for (int i = 0; i < JUMLAH_BARIS; i++)
        for (int j = 0; j < JUMLAH_KOLOM; j++)
            baru->kursi[i][j].namaPemesan = "kosong";

    if (!kepala) kepala = baru;
    else {
        Kereta* temp = kepala;
        while (temp->berikutnya) temp = temp->berikutnya;
        temp->berikutnya = baru;
    }

    cout << "\nKereta berhasil ditambahkan.";
    cin.get();
}

// ===== Tampilkan Kursi =====
void tampilkanKursi(Kereta* kereta) {
    cout << "\nKursi Kereta: " << kereta->namaKereta << "\n\n";
    cout << "       ";
    for (int j = 1; j <= JUMLAH_KOLOM; j++) cout << setw(10) << j;
    cout << "\n     " << string(11 * JUMLAH_KOLOM, '-') << "\n";
    for (int i = 1; i <= JUMLAH_BARIS; i++) {
        cout << "Baris " << setw(2) << i << " |";
        for (int j = 1; j <= JUMLAH_KOLOM; j++) {
            cout << setw(10) << kereta->kursi[i - 1][j - 1].namaPemesan;
        }
        cout << "\n";
    }
}

// ===== Pilih Kereta =====
Kereta* pilihKereta() {
    int nomor = 1;
    Kereta* temp = kepala;
    while (temp) {
        cout << nomor++ << ". " << temp->namaKereta << endl;
        temp = temp->berikutnya;
    }

    cout << "\nPilih nomor kereta: ";
    int pilihan;
    cin >> pilihan;

    temp = kepala;
    int indeks = 1;
    while (temp && indeks < pilihan) {
        temp = temp->berikutnya;
        indeks++;
    }
    return temp;
}

// ===== Pesan Tiket =====
void pesanTiket() {
    system("cls");
    cout << "=== Pesan Tiket ===\n";
    Kereta* kereta = pilihKereta();
    if (!kereta) return;

    tampilkanKursi(kereta);

    int baris, kolom;
    cout << "\nPilih Baris (1-" << JUMLAH_BARIS << "): ";
    cin >> baris;
    cout << "Pilih Kolom (1-" << JUMLAH_KOLOM << "): ";
    cin >> kolom;

    if (kereta->kursi[baris - 1][kolom - 1].namaPemesan != "kosong") {
        cout << "Kursi sudah dipesan.\n";
    } else {
        cout << "Masukkan nama pemesan: ";
        string nama;
        cin >> nama;
        kereta->kursi[baris - 1][kolom - 1].namaPemesan = nama;

        ofstream file("tiket_" + nama + ".txt");
        file << "Tiket Kereta\n";
        file << "Nama: " << nama << "\nKereta: " << kereta->namaKereta
             << "\nBaris: " << baris << "\nKolom: " << kolom << endl;
        file.close();

        cout << "Tiket berhasil dipesan dan disimpan di tiket_" << nama << ".txt\n";
    }

    cout << "Tekan Enter untuk kembali...";
    cin.ignore(); cin.get();
}

// ===== Batalkan Tiket =====
void batalkanTiket() {
    system("cls");
    cout << "=== Batalkan Tiket ===\n";
    Kereta* kereta = pilihKereta();
    if (!kereta) return;

    tampilkanKursi(kereta);

    int baris, kolom;
    cout << "\nBaris kursi (1-" << JUMLAH_BARIS << "): ";
    cin >> baris;
    cout << "Kolom kursi (1-" << JUMLAH_KOLOM << "): ";
    cin >> kolom;

    if (kereta->kursi[baris - 1][kolom - 1].namaPemesan == "kosong") {
        cout << "Kursi sudah kosong.\n";
    } else {
        cout << "Tiket atas nama " << kereta->kursi[baris - 1][kolom - 1].namaPemesan << " dibatalkan.\n";
        kereta->kursi[baris - 1][kolom - 1].namaPemesan = "kosong";
    }

    cout << "Tekan Enter untuk kembali...";
    cin.ignore(); cin.get();
}

// ===== Reset Kursi =====
void resetSemuaKursi() {
    system("cls");
    cout << "=== Reset Kursi ===\n";
    Kereta* kereta = pilihKereta();
    if (!kereta) return;

    for (int i = 0; i < JUMLAH_BARIS; i++)
        for (int j = 0; j < JUMLAH_KOLOM; j++)
            kereta->kursi[i][j].namaPemesan = "kosong";

    cout << "Semua kursi telah direset.\n";
    cout << "Tekan Enter untuk kembali...";
    cin.ignore(); cin.get();
}

// ===== Cari Pemesan =====
void cariPemesan() {
    system("cls");
    string nama;
    cout << "Masukkan nama yang dicari: ";
    cin >> nama;

    bool ditemukan = false;
    Kereta* temp = kepala;

    while (temp) {
        for (int i = 0; i < JUMLAH_BARIS; i++) {
            for (int j = 0; j < JUMLAH_KOLOM; j++) {
                if (temp->kursi[i][j].namaPemesan == nama) {
                    cout << "Ditemukan di " << temp->namaKereta << " Baris " << i + 1 << " Kolom " << j + 1 << "\n";
                    ditemukan = true;
                }
            }
        }
        temp = temp->berikutnya;
    }

    if (!ditemukan) {
        cout << "Nama tidak ditemukan.\n";
    }

    cout << "Tekan Enter untuk kembali...";
    cin.ignore(); cin.get();
}

// ===== Detail Kursi Kereta =====
void lihatDetailKereta() {
    system("cls");
    cout << "=== Detail Kursi Kereta ===\n";
    Kereta* kereta = pilihKereta();
    if (!kereta) return;

    tampilkanKursi(kereta);

    cout << "Tekan Enter untuk kembali...";
    cin.ignore(); cin.get();
}

// ===== Daftar Kereta Terurut (Bubble Sort tanpa ubah linked list) =====
void tampilkanDaftarKeretaTerurut() {
    system("cls");
    cout << "=== Daftar Kereta ===\n";
    int jumlah = 0;
    Kereta* temp = kepala;
    while (temp) {
        jumlah++;
        temp = temp->berikutnya;
    }

    if (jumlah == 0) {
        cout << "Belum ada kereta.\n";
        cout << "\nTekan Enter untuk kembali...";
        cin.ignore(); cin.get();
        return;
    }

    Kereta** daftar = new Kereta*[jumlah];
    temp = kepala;
    for (int i = 0; i < jumlah; i++) {
        daftar[i] = temp;
        temp = temp->berikutnya;
    }

    for (int i = 0; i < jumlah - 1; i++) {
        for (int j = 0; j < jumlah - i - 1; j++) {
            if (daftar[j]->namaKereta > daftar[j + 1]->namaKereta) {
                swap(daftar[j], daftar[j + 1]);
            }
        }
    }

    for (int i = 0; i < jumlah; i++) {
        cout << i + 1 << ". " << daftar[i]->namaKereta << endl;
    }

    delete[] daftar;

    cout << "\nTekan Enter untuk kembali...";
    cin.ignore(); cin.get();
}

// ===== Menu Utama =====
void menuUtama() {
    int pilihan;
    do {
        system("cls");
        cout << "\n=== MENU UTAMA SISTEM PEMESANAN TIKET KERETA ===\n";
        cout << "1. Tambah Kereta\n";
        cout << "2. Daftar Kereta\n";
        cout << "3. Pesan Tiket\n";
        cout << "4. Batalkan Tiket\n";
        cout << "5. Reset Kursi\n";
        cout << "6. Cari Pemesan\n";
        cout << "7. Lihat Detail Kursi\n";
        cout << "0. Keluar\n";
        cout << "Pilihan Anda: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: tambahKereta(); break;
            case 2: tampilkanDaftarKeretaTerurut(); break;
            case 3: pesanTiket(); break;
            case 4: batalkanTiket(); break;
            case 5: resetSemuaKursi(); break;
            case 6: cariPemesan(); break;
            case 7: lihatDetailKereta(); break;
            case 0: cout << "Terima kasih telah pesan tiket.\n"; break;
            default:
                cout << "Pilihan tidak valid.\n";
                cin.ignore(); cin.get(); break;
        }
    } while (pilihan != 0);
}

// ===== Fungsi Utama =====
int main() {
    if (prosesLogin() != 1) return 0;
    muatDataDariFile();
    menuUtama();
    simpanDataKeFile();
    return 0;
}
