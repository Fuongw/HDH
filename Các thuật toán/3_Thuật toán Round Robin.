#include <iostream>
using namespace std;

int main() {
    int n, quantum;
    cout << "Nhap so tien trinh: ";
    cin >> n;

    int pid[n], bt[n], rt[n]; // rt là thời gian còn lại
    for (int i = 0; i < n; i++) {
        pid[i] = i + 1;
        cout << "Nhap burst time cua tien trinh " << pid[i] << ": ";
        cin >> bt[i];
        rt[i] = bt[i];
    }

    cout << "Nhap quantum: ";
    cin >> quantum;

    int time = 0, done = 0, wt[n] = {0}, tat[n] = {0};
    while (done < n) {
        for (int i = 0; i < n; i++) {
            if (rt[i] > 0) {
                int exec = (rt[i] > quantum) ? quantum : rt[i];
                time += exec;
                rt[i] -= exec;
                if (rt[i] == 0) {
                    done++;
                    tat[i] = time;
                    wt[i] = tat[i] - bt[i];
                }
            }
        }
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
