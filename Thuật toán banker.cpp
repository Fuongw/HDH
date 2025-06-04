/* 
Nhap so tien trinh: 5
Nhap so loai tai nguyen: 3
Nhap ma tran Allocation:
Tien trinh P0: 0 1 0
Tien trinh P1: 2 0 0
Tien trinh P2: 3 0 2
Tien trinh P3: 2 1 1
Tien trinh P4: 0 0 2
Nhap ma tran Max:
Tien trinh P0: 7 5 3
Tien trinh P1: 3 2 2
Tien trinh P2: 9 0 2
Tien trinh P3: 2 2 2
Tien trinh P4: 4 3 3
Nhap mang Available: 3 3 2
*/
#include <iostream>
using namespace std;

const int MAX = 100; // tối đa số tiến trình và loại tài nguyên

int n, m; // n: số tiến trình, m: số loại tài nguyên
int Max[MAX][MAX], Allocation[MAX][MAX], Need[MAX][MAX];
int Available[MAX];
bool Finish[MAX];

void inputData() {
    cout << "Nhap so tien trinh: ";
    cin >> n;
    cout << "Nhap so loai tai nguyen: ";
    cin >> m;

    cout << "Nhap ma tran Allocation:\n";
    for (int i = 0; i < n; i++) {
        cout << "Tien trinh P" << i << ": ";
        for (int j = 0; j < m; j++) {
            cin >> Allocation[i][j];
        }
    }

    cout << "Nhap ma tran Max:\n";
    for (int i = 0; i < n; i++) {
        cout << "Tien trinh P" << i << ": ";
        for (int j = 0; j < m; j++) {
            cin >> Max[i][j];
        }
    }

    cout << "Nhap mang Available: ";
    for (int i = 0; i < m; i++) {
        cin >> Available[i];
    }

    // Tinh Need
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            Need[i][j] = Max[i][j] - Allocation[i][j];
}

bool isSafe() {
    int Work[MAX];
    for (int i = 0; i < m; i++) Work[i] = Available[i];
    for (int i = 0; i < n; i++) Finish[i] = false;

    int safeSeq[MAX];
    int count = 0;

    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (!Finish[i]) {
                bool ok = true;
                for (int j = 0; j < m; j++) {
                    if (Need[i][j] > Work[j]) {
                        ok = false;
                        break;
                    }
                }

                if (ok) {
                    for (int j = 0; j < m; j++)
                        Work[j] += Allocation[i][j];
                    safeSeq[count++] = i;
                    Finish[i] = true;
                    found = true;
                }
            }
        }
        if (!found) return false;
    }

    cout << "He thong o trang thai an toan.\nDay an toan: ";
    for (int i = 0; i < n; i++) cout << "P" << safeSeq[i] << " ";
    cout << endl;
    return true;
}

bool requestResource(int pid, int request[]) {
    for (int i = 0; i < m; i++) {
        if (request[i] > Need[pid][i]) {
            cout << "Yeu cau vuot qua nhu cau cua tien trinh.\n";
            return false;
        }
        if (request[i] > Available[i]) {
            cout << "Khong du tai nguyen. Tien trinh phai doi.\n";
            return false;
        }
    }

    // Giai dinh cap phat
    for (int i = 0; i < m; i++) {
        Available[i] -= request[i];
        Allocation[pid][i] += request[i];
        Need[pid][i] -= request[i];
    }

    if (isSafe()) {
        cout << "Yeu cau duoc chap nhan.\n";
        return true;
    } else {
        // rollback
        for (int i = 0; i < m; i++) {
            Available[i] += request[i];
            Allocation[pid][i] -= request[i];
            Need[pid][i] += request[i];
        }
        cout << "Yeu cau bi tu choi vi khong an toan.\n";
        return false;
    }
}

int main() {
    inputData();

    cout << "\n--- Kiem tra trang thai he thong ---\n";
    if (!isSafe()) {
        cout << "He thong KHONG o trang thai an toan ngay tu dau.\n";
        return 0;
    }

    char choice;
    do {
        int pid;
        int request[MAX];

        cout << "\nNhap ma so tien trinh muon yeu cau (0 -> " << n - 1 << "): ";
        cin >> pid;

        cout << "Nhap mang tai nguyen tien trinh P" << pid << " yeu cau: ";
        for (int i = 0; i < m; i++) cin >> request[i];

        requestResource(pid, request);

        cout << "\nBan co muon tiep tuc (y/n)? ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    return 0;
}
