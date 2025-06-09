#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    const int n = 5; // Số tiến trình
    const int m = 3; // Số loại tài nguyên

    int Allocation[n][m], Request[n][m], Available[m], Need[n][m];

    // Nhập Allocation
    cout << "Nhap Allocation matrix (" << n << " tien trinh, " << m << " tai nguyen):\n";
    for (int i = 0; i < n; i++) {
        cout << "Allocation cho P" << i << ": ";
        for (int j = 0; j < m; j++) {
            cin >> Allocation[i][j];
        }
    }

    // Nhập Request
    cout << "Nhap Request matrix:\n";
    for (int i = 0; i < n; i++) {
        cout << "Request cho P" << i << ": ";
        for (int j = 0; j < m; j++) {
            cin >> Request[i][j];
        }
    }

    // Nhập Available
    cout << "Nhap Available vector: ";
    for (int j = 0; j < m; j++) {
        cin >> Available[j];
    }

    // Tính bảng Need = Request - Allocation
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Need[i][j] = Request[i][j] - Allocation[i][j];
        }
    }

    // In bảng Need
    cout << "\nBang NEED (Request - Allocation):\n";
    cout << setw(10) << "Process" << setw(8) << "R1" << setw(8) << "R2" << setw(8) << "R3\n";
    for (int i = 0; i < n; i++) {
        cout << setw(10) << "P" + to_string(i);
        for (int j = 0; j < m; j++) {
            cout << setw(8) << Need[i][j];
        }
        cout << "\n";
    }

    // Bắt đầu thuật toán Banker
    bool Finish[n] = {false};
    int Work[m];
    for (int j = 0; j < m; j++) {
        Work[j] = Available[j];
    }

    int SafeSeq[n], count = 0;

    cout << "\nBang Work sau moi tien trinh:\n";
    cout << setw(10) << "Process" << setw(8) << "R1" << setw(8) << "R2" << setw(8) << "R3\n";

    while (count < n) {
        bool found = false;

        for (int i = 0; i < n; i++) {
            if (!Finish[i]) {
                bool canRun = true;
                for (int j = 0; j < m; j++) {
                    if (Need[i][j] > Work[j]) {
                        canRun = false;
                        break;
                    }
                }

                if (canRun) {
                    // In Work trước khi cộng
                    cout << setw(10) << "P" + to_string(i);
                    for (int j = 0; j < m; j++) {
                        cout << setw(8) << Work[j];
                    }
                    cout << "\n";

                    // Cấp phát lại tài nguyên: Work += Allocation[i]
                    for (int j = 0; j < m; j++) {
                        Work[j] += Allocation[i][j];
                    }

                    Finish[i] = true;
                    SafeSeq[count++] = i;
                    found = true;
                }
            }
        }

        if (!found) {
            cout << "\nHe thong KHONG o trang thai an toan.\n";
            return 0;
        }
    }

    // In Work cuối cùng
    cout << setw(10) << "Cuoi cung";
    for (int j = 0; j < m; j++) {
        cout << setw(8) << Work[j];
    }
    cout << "\n";

    // In chuỗi an toàn
    cout << "\nHe thong o trang thai AN TOAN.\nChuoi an toan: ";
    for (int i = 0; i < n; i++) {
        cout << "P" << SafeSeq[i];
        if (i != n - 1) cout << " -> ";
    }
    cout << "\n";

    return 0;
}
