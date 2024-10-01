#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
using namespace std;

struct dataJasa {
    string jenisDesain;
    int hargaAwal;
    dataJasa* next;
};

struct dataUser {
    string username;
    string password;
    string nama;
    string email; 
    dataJasa* headJasa;
    dataUser* next;
};

dataUser* headUser = nullptr;

bool isUserDataComplete(dataUser* user) {
    return !user->nama.empty() && !user->email.empty();
}

void infoUser(dataUser* user) {
    cout << "\nInformasi Pengguna:\n";
    cout << "Nama: " << (user->nama.empty() ? "Belum Diisi" : user->nama) << endl;
    cout << "Email: " << (user->email.empty() ? "Belum Diisi" : user->email) << endl;

    dataJasa* data = user->headJasa;
    if (!data) {
        cout << "Belum ada jasa yang terdaftar" << endl;
    } else {
        cout << "\nJasa yang terdaftar:\n";
        while (data) {
            cout << "- Spesialis Desain: " << data->jenisDesain << ", Harga awal: $" << data->hargaAwal << endl;
            data = data->next;
        }
    }
}

void editInformasi(dataUser* user) {
    cout << "\nMasukkan Nama: ";
    cin >> user->nama;
    cout << "Masukkan Email: ";
    cin >> user->email;
    cout << "Informasi berhasil diubah!" << endl;
}

void registrasi() {
    string user, pass;
    cout << "Registrasi Akun\nUsername: ";
    cin >> user;
    
    dataUser* data = headUser;
    while (data) {
        if (data->username == user) {
            cout << "Username Telah Dipakai Akun Lain" << endl;
            return;
        }
        data = data->next;
    }

    cout << "Password: ";
    cin >> pass;
    
    dataUser* newUser = new dataUser{user, pass, "", "", nullptr, nullptr};
    if (!headUser) {
        headUser = newUser;
    } else {
        data = headUser;
        while (data->next) {
            data = data->next;
        }
        data->next = newUser;
    }

    cout << "Registrasi Berhasil!" << endl;
}

dataUser* menuLogin() {
    string user, pass;
    cout << "\nLogin Menu\nUsername: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;

    dataUser* data = headUser;
    while (data) {
        if (data->username == user && data->password == pass) {
            cout << "Login Berhasil :D" << endl;
            return data;
        }
        data = data->next;
    }

    cout << "Login Gagal :(" << endl;
    return nullptr;
}

int inputHarga() {
    int harga;
    while (true) {
        cout << "Masukkan harga awal untuk jasa Anda: ";
        cin >> harga;
        if (!cin.fail() && harga > 0) { 
            break;
        } else {
            cout << "Input tidak valid\nMasukkan harga dalam angka positif\n";
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        }
    }
    return harga;
}

void addAwal(dataUser* user) {
    string spesialis;
    cout << "\nSpesialis Desain (contoh: Logo, UI/UX): ";
    cin >> spesialis;

    int hargaAwal = inputHarga();

    dataJasa* newJasa = new dataJasa{spesialis, hargaAwal, nullptr};

    newJasa->next = user->headJasa;
    user->headJasa = newJasa;

    cout << "Jasa berhasil ditambahkan di awal" << endl;
}

void addAkhir(dataUser* user) {
    string spesialis;
    cout << "\nSpesialis Desain (contoh: Logo, UI/UX): ";
    cin >> spesialis;

    int hargaAwal = inputHarga();

    dataJasa* newJasa = new dataJasa{spesialis, hargaAwal, nullptr};

    if (!user->headJasa) {
        user->headJasa = newJasa;
    } else {
        dataJasa* curr = user->headJasa;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = newJasa;
    }
    cout << "Jasa berhasil ditambahkan di akhir" << endl;
}

void hapusAwal(dataUser* user) {
    if (!user->headJasa) {
        cout << "Tidak ada jasa yang bisa dihapus" << endl;
        return;
    }

    dataJasa* temp = user->headJasa;
    user->headJasa = user->headJasa->next;
    delete temp;
    cout << "Jasa paling awal berhasil dihapus" << endl;
}

void hapusAkhir(dataUser* user) {
    if (!user->headJasa) {
        cout << "Tidak ada jasa yang bisa dihapus" << endl;
        return;
    }

    dataJasa* prev = nullptr;
    dataJasa* data = user->headJasa;
    while (data->next) {
        prev = data;
        data = data->next;
    }

    if (prev) {
        prev->next = nullptr;
    } else {
        user->headJasa = nullptr;
    }

    delete data;
    cout << "Jasa paling akhir berhasil dihapus" << endl;
}

void hapusAkun(dataUser* user) {
    dataJasa* dataSekarang = user->headJasa;
    while (dataSekarang) {
        dataJasa* temp = dataSekarang;
        dataSekarang = dataSekarang->next;
        delete temp;
    }

    if (headUser == user) {
        headUser = user->next;
    } else {
        dataUser* prevUser = headUser;
        while (prevUser->next != user) {
            prevUser = prevUser->next;
        }
        prevUser->next = user->next;
    }
    delete user;
    cout << "Akun dan jasa terkait telah dihapus" << endl;
}

void showJasa() {
    dataUser* userSekarang = headUser;
    cout << "\n" << setfill('=') << setw(70) << "=" << setfill(' ') << endl;
    cout << setfill('=') << setw(22) << "=" << setfill(' ') << "     Jual Jasa Desain     " << setfill('=') << setw(22) << "=" << setfill(' ') << endl;
    cout << setfill('=') << setw(70) << "=" << setfill(' ') << endl;
    cout << left << setw(18) << "Nama Desainer"
         << setw(16) << "Spesialis"
         << setw(25) << "Kontak"
         << setw(20) << "Harga awal" << endl;
    cout << setfill('=') << setw(70) << "=" << setfill(' ') << endl;

    while (userSekarang) {
        dataJasa* jasaSekarang = userSekarang->headJasa;
        while (jasaSekarang) {
            cout << left << setw(18) << userSekarang->nama
                 << setw(16) << jasaSekarang->jenisDesain
                 << setw(25) << userSekarang->email
                 << "$" << jasaSekarang->hargaAwal << endl;
            jasaSekarang = jasaSekarang->next;
        }
        userSekarang = userSekarang->next;
    }
    cout << setfill('=') << setw(70) << "=" << setfill(' ') << endl;
}

void menuUtama(dataUser* user) {
    int pilihan;
    do {
        cout << "\nMenu Utama\n1. Informasi User\n2. Lihat Jasa\n3. Tambah Jasa\n4. Hapus Jasa\n5. Hapus Akun\n6. Log Out\nPilihan: ";
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
                if (!isUserDataComplete(user)) {
                    cout << "\nInformasi Anda belum lengkap. Silakan lengkapi informasi Anda terlebih dahulu" << endl;
                    break;
                }
                int subPilihanTambah;
                cout << "1. Tambah Jasa di Awal\n2. Tambah Jasa di Akhir\nPilihan: ";
                cin >> subPilihanTambah;
                if (subPilihanTambah == 1) {
                    addAwal(user);
                } else if (subPilihanTambah == 2) {
                    addAkhir(user);
                } else {
                    cout << "Pilihan tidak valid" << endl;
                }
                break;
            case 4:
                if (!user->headJasa) {
                    cout << "Tidak ada jasa yang bisa dihapus" << endl;
                    break;
                }
                int subPilihanHapus;
                cout << "1. Hapus Jasa di Awal\n2. Hapus Jasa di Akhir\nPilihan: ";
                cin >> subPilihanHapus;
                if (subPilihanHapus == 1) {
                    hapusAwal(user);
                } else if (subPilihanHapus == 2) {
                    hapusAkhir(user);
                } else {
                    cout << "Pilihan tidak valid" << endl;
                }
                break;
            case 5:
                hapusAkun(user);
                return;
            case 6:
                cout << "Log Out Berhasil!" << endl;
                return;
            default:
                cout << "Pilihan tidak valid!" << endl;
                break;
        }
    } while (pilihan != 6);
}

void menuLoginUtama() {
    int pilihan;
    do {
        cout << "\nMenu Login\n1. Registrasi\n2. Login\n3. Keluar\nPilihan: ";
        cin >> pilihan;
        switch (pilihan) {
            case 1:
                registrasi();
                break;
            case 2: {
                dataUser* user = menuLogin();
                if (user) {
                    menuUtama(user);
                }
                break;
            }
            case 3:
                cout << "Terima kasih telah menggunakan program ini" << endl;
                return;
            default:
                cout << "Pilihan tidak valid" << endl;
                break;
        }
    } while (pilihan != 3);
}

int main() {
    menuLoginUtama();
    return 0;
}
