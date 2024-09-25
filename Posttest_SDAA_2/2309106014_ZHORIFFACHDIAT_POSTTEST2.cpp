#include <iostream>
#include <iomanip>
#include <limits>
using namespace std;

int jmlAkun = 1, jmlPenjual = 1;
string user, pass;
bool regisBerhasil = true, loginBerhasil = false;

struct dataUser {
    string username;
    string password;
    string nama;
    string spesialis;
    string noTelp;
    string email;
};

struct dataJasa {
    string userPenjual;
    string jenisDesain;
    string kontak;
    int hargaAwal;
};

dataUser akun[15] = {
    {"jorip", "jorip123", "Zhorif Fachdiat", "Logo", "081234567890", "jorip@unmul.ac.id"}
};

dataJasa jasa[30] = {
    {"jorip", "Logo", "jorip@unmul.ac.id", 5}
};

bool isUserDataComplete(dataUser *user) {
    return !user->nama.empty() && !user->noTelp.empty() && !user->email.empty();
}

void infoUser(dataUser *user) {
    cout << "\nInformasi Pengguna:\n";
    cout << "Nama: " << (user->nama.empty() ? "Belum Diisi" : user->nama) << endl;
    cout << "No Telp: " << (user->noTelp.empty() ? "Belum Diisi" : user->noTelp) << endl;
    cout << "Email: " << (user->email.empty() ? "Belum Diisi" : user->email) << endl;
}

void editInformasi(dataUser *user) {
    cout << "\nMasukkan Nama: ";
    cin >> user->nama;
    cout << "Masukkan No Telp: ";
    cin >> user->noTelp;
    cout << "Masukkan Email: ";
    cin >> user->email;
    cout << "Informasi berhasil diubah!" << endl;
}

void registrasi() {
    cout << "Registrasi Akun\nUsername: ";
    cin >> user;
    for (int i = 0; i < jmlAkun; i++) {
        if (user == akun[i].username) {
            cout << "Username Telah Dipakai Akun Lain" << endl;
            regisBerhasil = false;
            break;
        }
    }
    
    if (regisBerhasil) {
        cout << "Password: ";
        cin >> pass;
        akun[jmlAkun].username = user;
        akun[jmlAkun].password = pass;
        jmlAkun++;
        cout << "Registrasi Berhasil!" << endl;
    } else {
        cout << "Silahkan Registrasi Kembali" << endl;
        regisBerhasil = true;
    }
}

int menuLogin() {
    cout << "\nLogin Menu\nUsername: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;

    for (int i = 0; i < jmlAkun; i++) {
        if (akun[i].username == user && akun[i].password == pass) {
            loginBerhasil = true;
            cout << "Login Berhasil :D" << endl;
            return i;
        }
    }

    cout << "Login Gagal :(" << endl;
    return -1;
}

void showJasa() {
    cout << "\n" << setfill('=') << setw(64) << "=" << setfill(' ') << endl;
    cout << setfill('=') << setw(19) << "=" << setfill(' ') << "     Jual Jasa Desain     " << setfill('=') << setw(19) << "=" << setfill(' ') << endl;
    cout << setfill('=') << setw(64) << "=" << setfill(' ') << endl;
    cout << left << setw(18) << "Nama Desainer"
         << setw(16) << "Spesialis"
         << setw(20) << "Kontak"
         << setw(25) << "Harga awal" << endl;
    cout << setfill('=') << setw(64) << "=" << setfill(' ') << endl;
    for (int i = 0; i < jmlPenjual; i++) {
        cout << left << setw(18) << jasa[i].userPenjual
             << setw(16) << jasa[i].jenisDesain
             << setw(20) << jasa[i].kontak
             << "$" << jasa[i].hargaAwal << endl; 
    }
    cout << setfill('=') << setw(64) << "=" << setfill(' ') << endl;
}

int inputHarga() {
    int harga;
    while (true) {
        cout << "Masukkan harga awal untuk jasa Anda: ";
        cin >> harga;
        if (!cin.fail() && harga > 0) { 
            break;
        } else {
            cout << "Input tidak valid. Masukkan harga dalam angka positif.\n";
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        }
    }
    return harga;
}

void menjadiPenjual(dataUser *user) {
    if (!isUserDataComplete(user)) {
        cout << "\nInformasi Anda belum lengkap. Silakan lengkapi informasi Anda terlebih dahulu." << endl;
        return;
    }
    cout << "\nLengkapi data berikut untuk menjadi penjual jasa:" << endl;
    cout << "Spesialis Desain (contoh: Logo, UI/UX): ";
    cin >> user->spesialis;

    jasa[jmlPenjual].userPenjual = user->nama;
    jasa[jmlPenjual].jenisDesain = user->spesialis;
    jasa[jmlPenjual].kontak = user->email;
    jasa[jmlPenjual].hargaAwal = inputHarga(); 

    jmlPenjual++;
    cout << "Anda telah terdaftar sebagai penjual!" << endl;
}

void menuUtama(dataUser *user) {
    int pilihan;
    do {
        cout << "\nMenu Utama\n1. Informasi User\n2. Lihat Jasa\n3. Menjadi Penjual\n4. Log Out\nPilihan: ";
        cin >> pilihan;
        switch (pilihan) {
            case 1: {
                infoUser(user);
                int subPilihan;
                cout << "1. Edit Informasi\n2. Kembali\nPilihan: ";
                cin >> subPilihan;
                if (subPilihan == 1) {
                    editInformasi(user);
                }
                break;
            }
            case 2:
                showJasa();
                break;
            case 3:
                menjadiPenjual(user);
                break;
            case 4:
                cout << "Log Out Berhasil!" << endl;
                return; 
            default:
                cout << "Pilihan tidak valid!" << endl;
                break;
        }
    } while (pilihan != 4);
}

void menuLoginUtama() {
    int pilihan;
    int indexLogin = -1;
    do {
        cout << "\nMenu Login\n1. Registrasi\n2. Login\n3. Keluar\nPilihan: ";
        cin >> pilihan;
        switch (pilihan) {
            case 1:
                registrasi();
                break;
            case 2:
                indexLogin = menuLogin();
                if (indexLogin >= 0) {
                    menuUtama(&akun[indexLogin]);
                }
                break;
            case 3:
                cout << "Terima kasih telah menggunakan program ini!" << endl;
                return;
            default:
                cout << "Pilihan tidak valid!" << endl;
                break;
        }
    } while (pilihan != 3);
}

int main() {
    menuLoginUtama();
    return 0;
}
