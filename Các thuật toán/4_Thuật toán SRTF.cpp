//SRTF
#include <iostream>
#include <iomanip> // For std::setw, std::fixed, std::setprecision
#include <algorithm> // For std::min, std::max

using namespace std;

int main() {
    int n;
    cout << "Nhap so tien trinh: ";
    cin >> n;

    // Sử dụng mảng tĩnh. Lưu ý VLA (Variable Length Array) không phải C++ chuẩn.
    // Đối với các dự án lớn hơn, nên dùng std::vector hoặc cấp phát động (new int[n]).
    int pid[n];             // ID tiến trình
    int arrivalTime[n];     // Thời gian đến
    int burstTime[n];       // Thời gian xử lý ban đầu
    int remainingTime[n];   // Thời gian còn lại chưa xử lý

    // Các mảng kết quả
    int completionTime[n];   // Thời điểm hoàn thành
    int turnaroundTime[n];   // Thời gian lưu lại trong hệ thống
    int waitingTime[n];      // Thời gian chờ đợi

    // Trạng thái của tiến trình
    bool isCompleted[n];    // Đánh dấu tiến trình đã hoàn thành

    // Nhập thông tin cho từng tiến trình
    for (int i = 0; i < n; i++) {
        pid[i] = i + 1;
        cout << "Tien trinh " << pid[i] << " - Nhap arrival time va burst time: ";
        cin >> arrivalTime[i] >> burstTime[i];
        remainingTime[i] = burstTime[i]; // Ban đầu, thời gian còn lại = burst time
        isCompleted[i] = false;         // Ban đầu, chưa tiến trình nào hoàn thành
    }

    // --- Thực thi thuật toán SRTF (Shortest Remaining Time First) ---
    int currentTime = 0; // Thời gian hiện tại của CPU
    int completedProcesses = 0; // Số tiến trình đã hoàn thành

    // Biến để theo dõi tiến trình ngắn nhất đang được chọn
    int shortestJobIndex = -1;
    int minRemainingTime = 999999; // Giá trị đủ lớn để tìm min

    // Vòng lặp chính, tiếp tục cho đến khi tất cả các tiến trình hoàn thành
    while (completedProcesses < n) {
        // Reset minRemainingTime và shortestJobIndex ở mỗi bước thời gian
        // để tìm tiến trình ưu tiên mới
        minRemainingTime = 999999;
        shortestJobIndex = -1;

        // Bước 1: Tìm tiến trình có thời gian còn lại ngắn nhất
        // trong số các tiến trình đã đến và chưa hoàn thành
        for (int i = 0; i < n; i++) {
            if (arrivalTime[i] <= currentTime && !isCompleted[i]) {
                if (remainingTime[i] < minRemainingTime) {
                    minRemainingTime = remainingTime[i];
                    shortestJobIndex = i;
                }
            }
        }

        // Bước 2: Xử lý tiến trình được chọn
        if (shortestJobIndex != -1) {
            // Giảm thời gian còn lại của tiến trình được chọn đi 1 đơn vị
            remainingTime[shortestJobIndex]--;

            // Tăng thời gian hiện tại của CPU lên 1 đơn vị
            currentTime++;

            // Kiểm tra nếu tiến trình đã hoàn thành
            if (remainingTime[shortestJobIndex] == 0) {
                isCompleted[shortestJobIndex] = true;
                completedProcesses++;

                // Tính toán thời gian hoàn thành, lưu lại và chờ đợi
                // Thời điểm hoàn thành chính là currentTime hiện tại
                completionTime[shortestJobIndex] = currentTime;
                turnaroundTime[shortestJobIndex] = completionTime[shortestJobIndex] - arrivalTime[shortestJobIndex];
                waitingTime[shortestJobIndex] = turnaroundTime[shortestJobIndex] - burstTime[shortestJobIndex];

                // Đảm bảo thời gian chờ không âm
                if (waitingTime[shortestJobIndex] < 0) {
                    waitingTime[shortestJobIndex] = 0;
                }
            }
        } else {
            // Nếu không có tiến trình nào sẵn sàng (CPU rảnh rỗi hoặc chưa có tiến trình nào đến)
            // Tăng thời gian hiện tại để chờ đợi tiến trình tiếp theo đến
            currentTime++;
        }
    }

    // --- In kết quả và tính toán thời gian trung bình ---
    cout << "\n------------------------------------------------------------------------------------------------\n";
    cout << left << setw(5) << "ID"
              << setw(15) << "Arrival Time"
              << setw(15) << "Burst Time"
              << setw(18) << "Completion Time"
              << setw(18) << "Turnaround Time"
              << setw(15) << "Waiting Time" << endl;
    cout << "------------------------------------------------------------------------------------------------\n";

    float total_avg_wt = 0;
    float total_avg_tat = 0;

    // In kết quả theo thứ tự ID ban đầu
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
    cout << "------------------------------------------------------------------------------------------------\n";

    cout << fixed << setprecision(2); // Định dạng output cho số thập phân
    cout << "Thoi gian cho trung binh: " << total_avg_wt / n << endl;
    cout << "Thoi gian hoan tat trung binh: " << total_avg_tat / n << endl;

    return 0;
}
