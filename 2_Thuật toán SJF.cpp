#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Nhap so tien trinh: ";
    cin >> n;

    int pid[n], bt[n], wt[n], tat[n];

    for (int i = 0; i < n; i++) {
        pid[i] = i + 1;
        cout << "Nhap burst time cua tien trinh " << pid[i] << ": ";
        cin >> bt[i];
    }

    // Sắp xếp burst time tăng dần
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (bt[i] > bt[j]) {
                swap(bt[i], bt[j]);
                swap(pid[i], pid[j]);
            }
        }
    }

    wt[0] = 0;
    for (int i = 1; i < n; i++) {
        wt[i] = wt[i - 1] + bt[i - 1];
    }

    for (int i = 0; i < n; i++) {
        tat[i] = wt[i] + bt[i];
    }

    cout << "\nTien trinh\tBT\tWT\tTAT\n";
    float avg_wt = 0, avg_tat = 0;
    for (int i = 0; i < n; i++) {
        cout << pid[i] << "\t\t" << bt[i] << "\t" << wt[i] << "\t" << tat[i] << "\n";
        avg_wt += wt[i];
        avg_tat += tat[i];
    }

    cout << "Thoi gian cho trung binh: " << avg_wt / n << endl;
    cout << "Thoi gian hoan thanh trung binh: " << avg_tat / n << endl;
    return 0;
}
