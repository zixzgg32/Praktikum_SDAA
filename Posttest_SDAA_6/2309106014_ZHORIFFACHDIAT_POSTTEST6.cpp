#include <iostream>
#include <windows.h>
#include <iomanip>
#include <limits>
#include <string>
#include <sstream>
#include <ctime>
#include <math.h>

using namespace std;

struct dataJasa {
    string kodeJasa;
    string jenisDesain;
    int harga;
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
    int harga;
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

void splitJasaList(dataJasa* source, dataJasa** frontRef, dataJasa** backRef) {
    dataJasa* fast;
    dataJasa* slow;
    slow = source;
    fast = source->next;

    while (fast != nullptr) {
        fast = fast->next;
        if (fast != nullptr) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *frontRef = source;
    *backRef = slow->next;
    slow->next = nullptr;
}

dataJasa* mergeSortedJasa(dataJasa* a, dataJasa* b) {
    dataJasa* result = nullptr;

    if (a == nullptr)
        return b;
    else if (b == nullptr)
        return a;

    if (a->harga <= b->harga) {
        result = a;
        result->next = mergeSortedJasa(a->next, b);
    } else {
        result = b;
        result->next = mergeSortedJasa(a, b->next);
    }

    return result;
}

void mergeSortJasa(dataJasa** headRef) {
    if (*headRef == nullptr || (*headRef)->next == nullptr)
        return;

    dataJasa* head = *headRef;
    dataJasa* a;
    dataJasa* b;

    splitJasaList(head, &a, &b);

    mergeSortJasa(&a);
    mergeSortJasa(&b);

    *headRef = mergeSortedJasa(a, b);
}

stackJasa* getTail(stackJasa* curr) {
    while (curr && curr->next) {
        curr = curr->next;
    }
    return curr;
}

stackJasa* partition(stackJasa* head, stackJasa* end, stackJasa** newHead, stackJasa** newEnd) {
    stackJasa* pivot = end;
    stackJasa* prev = nullptr;
    stackJasa* curr = head;
    stackJasa* tail = pivot;

    while (curr != pivot) {
        if (curr->harga < pivot->harga) {  // Change comparison for ascending order
            if ((*newHead) == nullptr) {
                (*newHead) = curr;
            }
            prev = curr;
            curr = curr->next;
        } else {
            if (prev != nullptr) {
                prev->next = curr->next;
            }
            stackJasa* temp = curr->next;
            curr->next = nullptr;
            tail->next = curr;
            tail = curr;
            curr = temp;
        }
    }

    if ((*newHead) == nullptr) {
        (*newHead) = pivot;
    }

    (*newEnd) = tail;
    return pivot;
}

stackJasa* quickSortRecur(stackJasa* head, stackJasa* end) {
    if (!head || head == end)
        return head;

    stackJasa* newHead = nullptr;
    stackJasa* newEnd = nullptr;

    stackJasa* pivot = partition(head, end, &newHead, &newEnd);

    if (newHead != pivot) {
        stackJasa* temp = newHead;
        while (temp->next != pivot)
            temp = temp->next;
        temp->next = nullptr;

        newHead = quickSortRecur(newHead, temp);

        temp = getTail(newHead);
        temp->next = pivot;
    }

    pivot->next = quickSortRecur(pivot->next, newEnd);

    return newHead;
}

void quickSortHistori(stackJasa** headRef) {
    (*headRef) = quickSortRecur(*headRef, getTail(*headRef));
}

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
            cout << "- Kode Jasa: " << data->kodeJasa << ", Spesialis Desain: " << data->jenisDesain << ", Harga awal: $" << data->harga << endl;
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
    Sleep(1);
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
            Sleep(1);
            return data;
        }
        data = data->next;
    }

    cout << "Login Gagal :(" << endl;
    Sleep(1);
    return nullptr;
}

int inputHarga() {
    int harga;
    while (true) {
        cout << "Masukkan harga untuk jasa Anda: ";
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

    int harga = inputHarga();
    string kodeJasa = kodeJasaDesign();

    dataJasa* newJasa = new dataJasa{kodeJasa, spesialis, harga, nullptr};

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
            cout << "- Kode Jasa: " << data->kodeJasa << ", Spesialis Desain: " << data->jenisDesain << ", Harga awal: $" << data->harga << endl;
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
    if (jasa->harga > 20) {
        cout << "Anda perlu membayar DP sebesar 50% dari harga: $" << jasa->harga / 2 << endl;
    } else {
        cout << "Harga jasa: $" << jasa->harga << endl;
    }

    pesanan* newPesanan = new pesanan{pembeli->username, jasa->kodeJasa, "Belum diterima", nullptr};
    if (!penjual->queuePesananTail) {
        penjual->queuePesananHead = penjual->queuePesananTail = newPesanan;
    } else {
        penjual->queuePesananTail->next = newPesanan;
        penjual->queuePesananTail = newPesanan;
    }

    int sudahDibayar = jasa->harga > 20 ? jasa->harga / 2 : jasa->harga;
    stackJasa* newStackItem = new stackJasa{penjual->nama, penjual->email, jasa->kodeJasa, jasa->jenisDesain, jasa->harga, sudahDibayar, pembeli->stackHistori};
    pembeli->stackHistori = newStackItem;

    cout << "Pesanan berhasil dibuat!" << endl;
}

int length(dataJasa* head) {
    int count = 0;
    dataJasa* temp = head;
    while (temp != nullptr) {
        count++;
        temp = temp->next;
    }
    return count;
}

int min(int x, int y) { return (x <= y) ? x : y; }

// Fibonacci Search di implementasikan ke fungsi showJasa mencari harga awal
int fibMonaccianSearch(dataJasa* head, int x, int n) {
    dataJasa* temp = head;
    int fibMMm2 = 0;
    int fibMMm1 = 1;
    int fibM = fibMMm2 + fibMMm1;
    while (fibM < n) {
        fibMMm2 = fibMMm1;
        fibMMm1 = fibM;
        fibM = fibMMm2 + fibMMm1;
    }
    int offset = -1;
    while (fibM > 1) {
        int i = min(offset + fibMMm2, n - 1);
        temp = head;
        for (int j = 0; j < i; j++) {
            temp = temp->next;
        }
        if (temp->harga < x) {
            fibM = fibMMm1;
            fibMMm1 = fibMMm2;
            fibMMm2 = fibM - fibMMm1;
            offset = i;
        } else if (temp->harga > x) {
            fibM = fibMMm2;
            fibMMm1 = fibMMm1 - fibMMm2;
            fibMMm2 = fibM - fibMMm1;
        } else {
            return i;
        }
    }
    temp = head;
    for (int j = 0; j < offset + 1; j++) {
        temp = temp->next;
    }
    if (fibMMm1 && temp->harga == x) {
        return offset + 1;
    }
    return -1;
}

// Boyer-Moore di implementasikan ke fungsi showJasa mencari kode jasa
const int NO_OF_CHARS = 256;
void badCharHeuristic(string str, int size, int badChar[NO_OF_CHARS]) {
    for (int i = 0; i < NO_OF_CHARS; i++) {
        badChar[i] = -1;
    }
    for (int i = 0; i < size; i++) {
        badChar[(int)str[i]] = i;
    }
}

int searchKodeJasa(dataJasa* head, string pat) {
    int m = pat.size();
    dataJasa* temp = head;
    int index = 0;
    while (temp != nullptr) {
        int n = temp->kodeJasa.size();
        int badChar[NO_OF_CHARS];
        badCharHeuristic(pat, m, badChar);
        int s = 0;
        while (s <= (n - m)) {
            int j = m - 1;
            while (j >= 0 && pat[j] == temp->kodeJasa[s + j]) {
                j--;
            }
            if (j < 0) {
                return index;
            } else {
                s += max(1, j - badChar[temp->kodeJasa[s + j]]);
            }
        }
        temp = temp->next;
        index++;
    }
    return -1;
}

// Jump Search di implementasikan ke fungsi showHistori mencari total harga
int findNode(stackJasa* head, int idx) {
    for (int i = 0; i < idx; i++) {
        head = head->next;
    }
    return head->harga;
}

int jumpSearch(stackJasa* head, int x, int n) {
    int step = sqrt(n);
    int prev = 0;
    int idx = min(step, n) - 1;
    while (findNode(head, idx) < x) {
        prev = step;
        step += sqrt(n);
        idx = min(step, n) - 1;
        if (prev >= n) {
            return -1;
        }
    }
    while (findNode(head, prev) < x) {
        prev++;
    }
    if (findNode(head, prev) == x) {
        return prev;
    }
    return -1;
}

// implementasi algoritma merge sort secara ascending pada menampilkan jasa dan yang disorting adalah harga design
void showJasa() {
    system("cls");
    cout << "\n" << setfill('=') << setw(81) << "=" << setfill(' ') << endl;
    cout << setfill('=') << setw(27) << "=" << setfill(' ') << "      Jual Jasa Desain     " << setfill('=') << setw(27) << "=" << setfill(' ') << endl;
    cout << setfill('=') << setw(81) << "=" << setfill(' ') << endl;
    cout << left << setw(20) << "Nama" << setw(11) << "Kode Jasa" << setw(20) << "Jenis Desain" << setw(20) << "Email" << setw(20) << "Harga Awal" << endl;
    cout << setfill('=') << setw(81) << "=" << setfill(' ') << endl;
    
    dataUser* userSekarang = headUser;
    while (userSekarang) {
        mergeSortJasa(&userSekarang->headJasa); 
        dataJasa* jasa = userSekarang->headJasa;
        while (jasa) {
            cout << left << setw(20) << userSekarang->nama
                 << setw(11) << jasa->kodeJasa
                 << setw(20) << jasa->jenisDesain
                 << setw(20) << userSekarang->email
                << "$" << jasa->harga << endl;
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
// implementasi algoritma quick sort secara descending pada menampilkan history dan yang disorting adalah harga design
void showHistory(dataUser* user) {
    system("cls");
    quickSortHistori(&user->stackHistori);  // Tambahkan quick sort di sini
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
             << "$" << setw(15) << curr->harga
             << "$" << curr->sudahDibayar << endl;
        curr = curr->next;
    }
    cout << setfill('=') << setw(110) << "=" << setfill(' ') << endl;
}

void showSearchResult(dataUser* user) {
    system("cls");
    int choice;
    cout << "Pilih metode pencarian:\n1. Cari berdasarkan Kode Jasa\n2. Cari berdasarkan Harga Awal\nPilihan: ";
    cin >> choice;

    if (choice == 1) {
        string kode;
        cout << "Masukkan Kode Jasa: ";
        cin >> kode;
        dataUser* userSekarang = headUser;
        while (userSekarang) {
            int index = searchKodeJasa(userSekarang->headJasa, kode);
            if (index != -1) {
                dataJasa* jasa = userSekarang->headJasa;
                for (int i = 0; i < index; i++) {
                    jasa = jasa->next;
                }
                cout << "Jasa ditemukan:\n";
                cout << "Nama: " << userSekarang->nama << "\nKode Jasa: " << jasa->kodeJasa << "\nJenis Desain: " << jasa->jenisDesain << "\nEmail: " << userSekarang->email << "\nHarga Awal: $" << jasa->harga << "\n" << endl;
                return;
            }
            userSekarang = userSekarang->next;
        }
        cout << "Kode Jasa tidak ditemukan." << endl;
    } else if (choice == 2) {
        int harga;
        cout << "Masukkan Harga Awal: ";
        cin >> harga;
        dataUser* userSekarang = headUser;
        while (userSekarang) {
            int n = length(userSekarang->headJasa);
            int index = fibMonaccianSearch(userSekarang->headJasa, harga, n);
            if (index != -1) {
                dataJasa* jasa = userSekarang->headJasa;
                for (int i = 0; i < index; i++) {
                    jasa = jasa->next;
                }
                cout << "Jasa ditemukan:\n";
                cout << "Nama: " << userSekarang->nama << "\nKode Jasa: " << jasa->kodeJasa << "\nJenis Desain: " << jasa->jenisDesain << "\nEmail: " << userSekarang->email << "\nHarga Awal: $" << jasa->harga << "\n" << endl;
                return;
            }
            userSekarang = userSekarang->next;
        }
        cout << "Harga Awal tidak ditemukan." << endl;
    } else {
        cout << "Pilihan tidak valid." << endl;
    }
}

void showHistorySearch(dataUser* user) {
    system("cls");
    int harga;
    cout << "Masukkan Harga Total: ";
    cin >> harga;
    int n = 0;
    stackJasa* temp = user->stackHistori;
    while (temp) {
        n++;
        temp = temp->next;
    }
    int index = jumpSearch(user->stackHistori, harga, n);
    if (index != -1) {
        stackJasa* curr = user->stackHistori;
        for (int i = 0; i < index; i++) {
            curr = curr->next;
        }
        cout << "Histori ditemukan:\n";
        cout << "Nama Penjual: " << curr->usernamaPenjual << "\nEmail Penjual: " << curr->emailPenjual << "\nKode Jasa: " << curr->kodeJasa << "\nJenis Desain: " << curr->jenisDesain << "\nHarga Total: $" << curr->harga << "\nSudah Dibayar: $" << curr->sudahDibayar << "\n" << endl;
    } else {
        cout << "Harga Total tidak ditemukan." << endl;
    }
}

void showJasaMenu(dataUser* user) {
    int subPilihan;
    do {
        showJasa();
        cout << "\n1. Cari Jasa\n2. Kembali\nPilihan: ";
        cin >> subPilihan;
        if (subPilihan == 1) {
            showSearchResult(user);
            cout << "\nTekan enter untuk melanjutkan...";
            cin.clear();
            cin.ignore();
            cin.get();
        }
    } while (subPilihan != 2);
}

void showHistoryMenu(dataUser* user) {
    int subPilihan;
    do {
        showHistory(user);
        cout << "\n1. Cari Histori\n2. Kembali\nPilihan: ";
        cin >> subPilihan;
        if (subPilihan == 1) {
            showHistorySearch(user);
            cout << "\nTekan enter untuk melanjutkan...";
            cin.clear();
            cin.ignore();
            cin.get();
        }
    } while (subPilihan != 2);
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
                showJasaMenu(user);
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
                Sleep(1);
                system("cls");
                break;
            }
            case 4:
                if (!isUserDataComplete(user)) {
                    cout << "\nInformasi Anda belum lengkap. Silakan lengkapi informasi Anda terlebih dahulu" << endl;
                    Sleep(1);
                	system("cls");
                    break;
                }
                cout << "1. Tambah Jasa di Awal\n2. Tambah Jasa di Akhir\nPilihan: ";
                cin >> subPilihan;
                addJasa(user, subPilihan == 1 ? true : false);
                Sleep(1);
                system("cls");
                break;
            case 5:
                delJasa(user);
                Sleep(1);
                system("cls");
                break;
            case 6:
                showPesanan(user);
                Sleep(1);
                system("cls");
                break;
            case 7:
                showHistoryMenu(user);
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
                Sleep(1);
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
                Sleep(1);
                system("cls");
                break;
            case 2: {
                dataUser* user = menuLogin();
                if (user) {
                    menuUtama(user);
                    Sleep(1);
                	system("cls");
                }
                Sleep(1);
                system("cls");
                break;
            }
            case 3:
                cout << "Terima kasih telah menggunakan program ini" << endl;
                Sleep(1);
                system("cls");
                return;
            default:
                cout << "Pilihan tidak valid" << endl;
                Sleep(1);
                system("cls");
                break;
        }
    } while (pilihan != 3);
}

int main() {
    menuLoginUtama();
    return 0;
}