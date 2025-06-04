//SRTF
#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Nhap so tien trinh: ";
    cin >> n;

    int at[n], bt[n], rt[n], pid[n]; // arrival time, burst time, remaining time, process ID
    int wt[n] = {0}, tat[n] = {0};
    bool completed[n] = {false};

    for (int i = 0; i < n; i++) {
        pid[i] = i + 1;
        cout << "Tien trinh " << pid[i] << " - Nhap arrival time va burst time: ";
        cin >> at[i] >> bt[i];
        rt[i] = bt[i]; // Ban đầu thời gian còn lại = burst time
    }

    int complete = 0, time = 0, min_rt = 1e9, shortest = -1;
    bool found = false;

    while (complete < n) {
        found = false;

        // Tìm tiến trình có thời gian còn lại nhỏ nhất và đã đến
        for (int i = 0; i < n; i++) {
            if (at[i] <= time && !completed[i] && rt[i] < min_rt && rt[i] > 0) {
                min_rt = rt[i];
                shortest = i;
                found = true;
            }
        }

        if (!found) {
            time++; // không có tiến trình đến, tăng thời gian
            continue;
        }

        rt[shortest]--;
        min_rt = rt[shortest]; // cập nhật min

        if (min_rt == 0)
            min_rt = 1e9;

        if (rt[shortest] == 0) {
            completed[shortest] = true;
            complete++;
            int finish_time = time + 1;
            wt[shortest] = finish_time - bt[shortest] - at[shortest];
            if (wt[shortest] < 0) wt[shortest] = 0;
            tat[shortest] = wt[shortest] + bt[shortest];
        }

        time++;
    }

    cout << "\nTien trinh\tAT\tBT\tWT\tTAT\n";
    float avg_wt = 0, avg_tat = 0;
    for (int i = 0; i < n; i++) {
        cout << "P" << pid[i] << "\t\t" << at[i] << "\t" << bt[i] << "\t" << wt[i] << "\t" << tat[i] << "\n";
        avg_wt += wt[i];
        avg_tat += tat[i];
    }

    cout << "Thoi gian cho trung binh: " << avg_wt / n << endl;
    cout << "Thoi gian hoan tat trung binh: " << avg_tat / n << endl;

    return 0;
}
