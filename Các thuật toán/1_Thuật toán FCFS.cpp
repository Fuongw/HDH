/* 
  pid[] mảng lưu ID của từng tiến trình (1,2,3...)
  bt[]_burst time : xử lý tiến trình
  wt[]_waiting time: thời gian chờ
  tat[] Tổng thời gian từ lúc bắt đầu đến khi hoàn thành tiến trình. 
  Công thức: tat[i] = wt[i] + bt[i].
  pr[]_priority Độ ưu tiên của tiến trình (số càng nhỏ → ưu tiên càng cao).
  rt[]_remaining time: Thời gian còn lại chưa xử lý của tiến trình (dùng trong Round Robin).
  time: thời gian tổng
  done: số tiến trình đã hoàn thành
  */
#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Nhap so tien trinh: ";
    cin >> n;

    int pid[n], bt[n], wt[n], tat[n]; 
  /*
  pid: ID tiến trình
  bt: thời gian xử lý (burst time)
  wt: thời gian chờ
  tat: thời gian hoàn tất
  */
    for (int i = 0; i < n; i++) {
        pid[i] = i + 1;
        cout << "Nhap burst time_thời gian xử lý cua tien trinh " << pid[i] << ": ";
        cin >> bt[i];
    }

    wt[0] = 0;  // tiến trình đầu tiên không phải đợi
// thời gian đợi
    for (int i = 1; i < n; i++) {
        wt[i] = wt[i - 1] + bt[i - 1];
    }
//tat[i] = wt[i] + bt[i]
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
