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
#include <iomanip> // Để định dạng output
#include <algorithm> // Dùng cho std::max (đảm bảo currentTime không nhỏ hơn arrivalTime)

using namespace std;

int main() {
    int n;
    cout << "Nhap so tien trinh: ";
    cin >> n;

    // Sử dụng mảng tĩnh, chấp nhận rủi ro VLA (Variable Length Array)
    // trong C++ không chuẩn nếu trình biên dịch không hỗ trợ C99 mode.
    // Đối với các dự án lớn hơn, nên dùng std::vector hoặc cấp phát động.
    int pid[n];         // ID tiến trình
    int arrivalTime[n]; // Thời gian đến
    int burstTime[n];   // Thời gian xử lý
    int completionTime[n]; // Thời gian hoàn thành
    int turnaroundTime[n]; // Thời gian hoàn tất
    int waitingTime[n]; // Thời gian chờ

    // Nhập thông tin cho từng tiến trình
    for (int i = 0; i < n; i++) {
        pid[i] = i + 1; // Gán ID từ 1 đến n
        cout << "Nhap thoi gian den (Arrival Time) cua tien trinh " << pid[i] << ": ";
        cin >> arrivalTime[i];
        cout << "Nhap thoi gian xu ly (Burst Time) cua tien trinh " << pid[i] << ": ";
        cin >> burstTime[i];
    }

    // --- Sắp xếp tiến trình theo thời gian đến (Arrival Time) cho FCFS ---
    // Vì không dùng struct Process, chúng ta phải sắp xếp nhiều mảng cùng lúc.
    // Đây là cách đơn giản nhất (Bubble Sort) để giữ code ngắn gọn.
    // Với số lượng tiến trình lớn, nên dùng thuật toán sắp xếp hiệu quả hơn.
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arrivalTime[j] > arrivalTime[j + 1]) {
                // Hoán đổi Arrival Time
                swap(arrivalTime[j], arrivalTime[j + 1]);
                // Hoán đổi Burst Time
                swap(burstTime[j], burstTime[j + 1]);
                // Hoán đổi PID
                swap(pid[j], pid[j + 1]);
                // Nếu có các thuộc tính khác (priority, etc.), cũng cần hoán đổi
            }
        }
    }

    // --- Tính toán thời gian hoàn thành, chờ, và hoàn tất cho FCFS ---
    int currentTime = 0;

    for (int i = 0; i < n; i++) {
        // Đảm bảo rằng currentTime không nhỏ hơn thời gian đến của tiến trình.
        // Nếu CPU rảnh trước khi tiến trình đến, CPU sẽ chờ.
        currentTime = max(currentTime, arrivalTime[i]);

        // Thời gian hoàn thành
        completionTime[i] = currentTime + burstTime[i];

        // Thời gian lưu lại trong hệ thống (Turnaround Time)
        // Là tổng thời gian từ lúc đến cho đến khi hoàn thành.
        turnaroundTime[i] = completionTime[i] - arrivalTime[i];

        // Thời gian chờ đợi (Waiting Time)
        // Là Turnaround Time trừ đi thời gian thực thi (Burst Time).
        waitingTime[i] = turnaroundTime[i] - burstTime[i];

        // Cập nhật currentTime cho tiến trình tiếp theo
        currentTime = completionTime[i];
    }

    // --- In kết quả và tính toán thời gian trung bình ---
    cout << "\n------------------------------------------------------------------\n";
    cout << left << setw(5) << "ID"
              << setw(15) << "Arrival Time"
              << setw(15) << "Burst Time"
              << setw(18) << "Completion Time"
              << setw(18) << "Turnaround Time"
              << setw(15) << "Waiting Time" << endl;
    cout << "------------------------------------------------------------------\n";

    float total_avg_wt = 0;
    float total_avg_tat = 0;

    for (int i = 0; i < n; i++) {
        cout << left << setw(5) << pid[i]
                  << setw(15) << arrivalTime[i]
                  << setw(15) << burstTime[i]
                  << setw(18) << completionTime[i]
                  << setw(18) << turnaroundTime[i]
                  << setw(15) << waitingTime[i] << endl;
        total_avg_wt += waitingTime[i];
        total_avg_tat += turnaroundTime[i];
    }
    cout << "------------------------------------------------------------------\n";

    cout << fixed << setprecision(2);
    cout << "Thoi gian cho trung binh: " << total_avg_wt / n << endl;
    cout << "Thoi gian hoan tat trung binh: " << total_avg_tat / n << endl;

    return 0;
}
