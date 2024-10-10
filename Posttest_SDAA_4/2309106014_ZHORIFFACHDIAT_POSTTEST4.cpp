#include <iostream>
#include <windows.h>
#include <iomanip>
#include <limits>
#include <string>
#include <sstream>
#include <ctime>
using namespace std;

struct dataJasa {
    string kodeJasa;
    string jenisDesain;
    int hargaAwal;
    dataJasa* next;
};

struct pesanan {
    string usernamePembeli;
    string kodeJasa;
    string status; 
    pesanan* next;
};

struct stackJasa {
    string usernamaPenjual;
    string emailPenjual;
    string kodeJasa;
    string jenisDesain;
    int hargaAwal;
    int sudahDibayar;
    stackJasa* next;
};

struct dataUser {
    string username;
    string password;
    string nama;
    string email;
    bool isEdited = false;
    dataJasa* headJasa;
    dataUser* next;
    pesanan* queuePesananHead = nullptr;
    pesanan* queuePesananTail = nullptr;
    stackJasa* stackHistori = nullptr;
};

dataUser* headUser = nullptr;
int kodeUnikCounter = 1000;

bool isUserDataComplete(dataUser* user) {
    return !user->nama.empty() && !user->email.empty();
}

string kodeJasaDesign() {
    stringstream ss;
    ss << "D" << kodeUnikCounter++;
    return ss.str();
}

void infoUser(dataUser* user) {
    system("cls");
    cout << "\nInformasi Pengguna:\n";
    cout << "Nama: " << (user->nama.empty() ? "Belum Diisi" : user->nama) << endl;
    cout << "Email: " << (user->email.empty() ? "Belum Diisi" : user->email) << endl;

    dataJasa* data = user->headJasa;
    if (!data) {
        cout << "Belum ada jasa yang terdaftar" << endl;
    } else {
        cout << "\nJasa yang terdaftar:\n";
        while (data) {
            cout << "- Kode Jasa: " << data->kodeJasa << ", Spesialis Desain: " << data->jenisDesain << ", Harga awal: $" << data->hargaAwal << endl;
            data = data->next;
        }
    }
}

void editInformasi(dataUser* user) {
    if (user->isEdited) {
        cout << "Anda sudah pernah mengedit informasi. Hanya bisa dilakukan sekali." << endl;
        return;
    }
    cin.clear();
    cin.ignore();
    cout << "\nMasukkan Nama: ";
    getline(cin, user->nama);
    cout << "Masukkan Email: ";
    cin >> user->email;
    cin.clear();
    cin.ignore();
    user->isEdited = true;
    cout << "Informasi berhasil diubah!" << endl;
    sleep(1);
}

void registrasi() {
    system("cls"); 
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
    
    dataUser* newUser = new dataUser{user, pass, "", "", false, nullptr, nullptr, nullptr, nullptr};
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
    system("cls");
    string user, pass;
    cout << "\nLogin Menu\nUsername: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;

    dataUser* data = headUser;
    while (data) {
        if (data->username == user && data->password == pass) {
            cout << "Login Berhasil :D" << endl;
            sleep(1);
            return data;
        }
        data = data->next;
    }

    cout << "Login Gagal :(" << endl;
    sleep(1);
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
        	cin.ignore(INT_MAX, '\n');
        }
    }
    return harga;
}

void addJasa(dataUser* user, bool addAwal) {
    system("cls");
    string spesialis;
    cin.clear();
    cin.ignore();
    cout << "\nSpesialis Desain (contoh: Logo, UI/UX): ";
    getline(cin, spesialis);

    int hargaAwal = inputHarga();
    string kodeJasa = kodeJasaDesign();

    dataJasa* newJasa = new dataJasa{kodeJasa, spesialis, hargaAwal, nullptr};

    if (addAwal || !user->headJasa) {
        newJasa->next = user->headJasa;
        user->headJasa = newJasa;
    } else {
        dataJasa* curr = user->headJasa;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = newJasa;
    }

    cout << "Jasa berhasil ditambahkan" << (addAwal ? " di awal." : " di akhir.") << endl;
}

void delJasa(dataUser* user) {
    system("cls");
    
    dataJasa* data = user->headJasa;
    if (!data) {
        cout << "Anda belum memiliki jasa yang terdaftar." << endl;
        return;
    } else {
        cout << "\nJasa yang terdaftar:\n";
        while (data) {
            cout << "- Kode Jasa: " << data->kodeJasa << ", Spesialis Desain: " << data->jenisDesain << ", Harga awal: $" << data->hargaAwal << endl;
            data = data->next;
        }
    }

    string kode;
    cout << "Masukkan kode jasa yang ingin dihapus: ";
    cin >> kode;

    dataJasa* curr = user->headJasa;
    dataJasa* prev = nullptr;

    while (curr) {
        if (curr->kodeJasa == kode) {
            if (prev) {
                prev->next = curr->next;
            } else {
                user->headJasa = curr->next;
            }
            delete curr;
            cout << "Jasa dengan kode " << kode << " berhasil dihapus." << endl;
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    cout << "Jasa dengan kode " << kode << " tidak ditemukan." << endl;
}

void buyJasa(dataUser* pembeli, dataJasa* jasa, dataUser* penjual) {
    system("cls"); 
    if (jasa->hargaAwal > 20) {
        cout << "Anda perlu membayar DP sebesar 50% dari harga: $" << jasa->hargaAwal / 2 << endl;
    } else {
        cout << "Harga jasa: $" << jasa->hargaAwal << endl;
    }

    pesanan* newPesanan = new pesanan{pembeli->username, jasa->kodeJasa, "Belum diterima", nullptr};
    if (!penjual->queuePesananTail) {
        penjual->queuePesananHead = penjual->queuePesananTail = newPesanan;
    } else {
        penjual->queuePesananTail->next = newPesanan;
        penjual->queuePesananTail = newPesanan;
    }

    int sudahDibayar = jasa->hargaAwal > 20 ? jasa->hargaAwal / 2 : jasa->hargaAwal;
    stackJasa* newStackItem = new stackJasa{penjual->nama, penjual->email, jasa->kodeJasa, jasa->jenisDesain, jasa->hargaAwal, sudahDibayar, pembeli->stackHistori};
    pembeli->stackHistori = newStackItem;

    cout << "Pesanan berhasil dibuat!" << endl;
}

void showJasa() {
    system("cls");
    cout << "\n" << setfill('=') << setw(81) << "=" << setfill(' ') << endl;
    cout << setfill('=') << setw(27) << "=" << setfill(' ') << "      Jual Jasa Desain     " << setfill('=') << setw(27) << "=" << setfill(' ') << endl;
    cout << setfill('=') << setw(81) << "=" << setfill(' ') << endl;
    cout << left << setw(20) << "Nama" << setw(11) << "Kode Jasa" << setw(20) << "Jenis Desain" << setw(20) << "Email" << setw(20) << "Harga Awal" << endl;
    cout << setfill('=') << setw(81) << "=" << setfill(' ') << endl;
    dataUser* userSekarang = headUser;
    while (userSekarang) {
        dataJasa* jasa = userSekarang->headJasa;
        while (jasa) {
            cout << left << setw(20) << userSekarang->nama
                 << setw(11) << jasa->kodeJasa
                 << setw(20) << jasa->jenisDesain
                 << setw(20) << userSekarang->email
                << "$" << jasa->hargaAwal << endl;
            jasa = jasa->next;
        }
        userSekarang = userSekarang->next;
    }
    cout << setfill('=') << setw(81) << "=" << setfill(' ') << endl;
}

// implementasi queue (hanya enqueue dan dequeue untuk menampilkan pesanan dari paling awal)
void showPesanan(dataUser* user) {
    system("cls"); 
    pesanan* curr = user->queuePesananHead;
    if (!curr) {
        cout << "Tidak ada pesanan saat ini" << endl;
        return;
    }
    cout << "Pesanan Anda:\n";
    while (curr) {
        cout << "Kode Jasa: " << curr->kodeJasa << " | Pembeli: " << curr->usernamePembeli << " | Status: " << curr->status << endl;

        int pilihan;
        cout << "1. Terima Pesanan\n2. Tolak Pesanan\nPilihan: ";
        cin >> pilihan;
        if (pilihan == 1) {
            curr->status = "Diterima";
            cout << "Pesanan diterima!" << endl;
        } else if (pilihan == 2) {
            curr->status = "Ditolak";
            cout << "Pesanan ditolak!" << endl;
        } else {
            cout << "Pilihan tidak valid." << endl;
        }

        curr = curr->next;
    }
}

// implementasi stack (hanya push dan pop jadi terbaru akan menjadi nomor paling atas di tabel)
void showHistory(dataUser* user) {
    system("cls");
    stackJasa* curr = user->stackHistori;
    if (!curr) {
        cout << "Tidak ada histori jasa terjual" << endl;
        return;
    }
    cout << left << setw(18) << "Nama Penjual" << setw(25) << "Email Penjual" << setw(16) << "Kode Jasa" << setw(20) << "Jenis Desain" << setw(15) << "Harga Total" << setw(15) << "Sudah Dibayar" << endl;
    cout << setfill('=') << setw(110) << "=" << setfill(' ') << endl;
    while (curr) {
        cout << left << setw(18) << curr->usernamaPenjual
             << setw(25) << curr->emailPenjual
             << setw(16) << curr->kodeJasa
             << setw(20) << curr->jenisDesain
             << "$" << setw(15) << curr->hargaAwal
             << "$" << curr->sudahDibayar << endl;
        curr = curr->next;
    }
    cout << setfill('=') << setw(110) << "=" << setfill(' ') << endl;
}

void menuUtama(dataUser* user) {
    int pilihan, subPilihan;
    do {
        system("cls"); 
        cout << "\nMenu Utama\n1. Informasi User\n2. Lihat Jasa\n3. Beli Jasa\n4. Tambah Jasa\n5. Hapus Jasa\n6. Lihat Pesanan\n7. Lihat Histori\n8. Log Out\nPilihan: ";
        cin.clear();
        cin >> pilihan;
        switch (pilihan) {
            case 1: {
                infoUser(user);
                cout << "1. Edit Informasi\n2. Kembali\nPilihan: ";
                cin >> subPilihan;
                if (subPilihan == 1) {
                    editInformasi(user);
                }
                break;
            }
            case 2:
                showJasa();
                cout << "\nTekan enter untuk melanjutkan...";
                cin.clear();
			    cin.ignore();
			    cin.get(); 
                break;
            case 3: {
                showJasa();
                string kode;
                cout << "Masukkan kode jasa yang ingin dibeli: ";
                cin >> kode;

                dataUser* userSekarang = headUser;
                bool found = false;
                while (userSekarang) {
                    if (userSekarang->username != user->username) { 
                        dataJasa* jasa = userSekarang->headJasa;
                        while (jasa) {
                            if (jasa->kodeJasa == kode) {
                                buyJasa(user, jasa, userSekarang);
                                found = true;
                                break;
                            }
                            jasa = jasa->next;
                        }
                    }
                    if (found) break;
                    userSekarang = userSekarang->next;
                }

                if (!found) {
                    cout << "Kode jasa tidak ditemukan atau Anda tidak dapat membeli jasa Anda sendiri." << endl;
                }
                sleep(1);
                system("cls");
                break;
            }
            case 4:
                if (!isUserDataComplete(user)) {
                    cout << "\nInformasi Anda belum lengkap. Silakan lengkapi informasi Anda terlebih dahulu" << endl;
                    sleep(1);
                	system("cls");
                    break;
                }
                cout << "1. Tambah Jasa di Awal\n2. Tambah Jasa di Akhir\nPilihan: ";
                cin >> subPilihan;
                addJasa(user, subPilihan == 1 ? true : false);
                sleep(1);
                system("cls");
                break;
            case 5:
                delJasa(user);
                sleep(1);
                system("cls");
                break;
            case 6:
                showPesanan(user);
                sleep(1);
                system("cls");
                break;
            case 7:
                showHistory(user);
                cout << "\nTekan enter untuk melanjutkan...";
                cin.clear();
			    cin.ignore();
			    cin.get(); 
                break;
            case 8:
                system("cls");
                return;
            default:
                cout << "Pilihan tidak valid!" << endl;
                sleep(1);
                system("cls");
                break;
        }
    } while (pilihan != 8);
}

void menuLoginUtama() {
    int pilihan;
    do {
        system("cls");
        cout << "\nMenu Login\n1. Registrasi\n2. Login\n3. Keluar\nPilihan: ";
        cin.clear();
        cin >> pilihan;
        switch (pilihan) {
            case 1:
                registrasi();
                sleep(1);
                system("cls");
                break;
            case 2: {
                dataUser* user = menuLogin();
                if (user) {
                    menuUtama(user);
                    sleep(1);
                	system("cls");
                }
                sleep(1);
                system("cls");
                break;
            }
            case 3:
                cout << "Terima kasih telah menggunakan program ini" << endl;
                sleep(1);
                system("cls");
                return;
            default:
                cout << "Pilihan tidak valid" << endl;
                sleep(1);
                system("cls");
                break;
        }
    } while (pilihan != 3);
}

int main() {
    menuLoginUtama();
    return 0;
}
