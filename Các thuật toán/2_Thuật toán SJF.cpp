//SJF
#include <iostream>
#include <iomanip> // For std::setw, std::fixed, std::setprecision
#include <algorithm> // For std::min, std::max, std::swap

using namespace std;

int main() {
    int n;
    cout << "Nhap so tien trinh: ";
    cin >> n;

    // Sử dụng mảng tĩnh cho đơn giản.
    // Lưu ý: VLA (Variable Length Array) là một tính năng của C99, không phải C++ chuẩn.
    // Nếu bạn gặp lỗi biên dịch, hãy cân nhắc dùng std::vector hoặc cấp phát động (new int[n]).
    int pid[n];             // ID tiến trình
    int arrivalTime[n];     // Thời gian đến
    int burstTime[n];       // Thời gian xử lý ban đầu
    int remainingTime[n];   // Thời gian còn lại (để đánh dấu hoàn thành)
    bool isCompleted[n];    // Trạng thái hoàn thành của tiến trình

    // Các mảng kết quả
    int completionTime[n];   // Thời gian hoàn thành
    int turnaroundTime[n];   // Thời gian lưu lại trong hệ thống
    int waitingTime[n];      // Thời gian chờ

    // Nhập thông tin cho từng tiến trình
    for (int i = 0; i < n; i++) {
        pid[i] = i + 1; // Gán ID từ 1 đến n
        cout << "Nhap thoi gian den (Arrival Time) cua tien trinh " << pid[i] << ": ";
        cin >> arrivalTime[i];
        cout << "Nhap thoi gian xu ly (Burst Time) cua tien trinh " << pid[i] << ": ";
        cin >> burstTime[i];
        remainingTime[i] = burstTime[i]; // Ban đầu, thời gian còn lại bằng burst time
        isCompleted[i] = false;         // Ban đầu, chưa tiến trình nào hoàn thành
    }

    // --- Thực thi thuật toán SJF (Non-preemptive) ---
    int currentTime = 0;
    int completedProcesses = 0;

    // Vòng lặp chính, tiếp tục cho đến khi tất cả các tiến trình hoàn thành
    while (completedProcesses < n) {
        int shortestJobIndex = -1;
        int minBurst = 999999; // Một giá trị đủ lớn để tìm burst time nhỏ nhất

        // Bước 1: Tìm tiến trình có burst time ngắn nhất đã đến và chưa hoàn thành
        for (int i = 0; i < n; i++) {
            if (arrivalTime[i] <= currentTime && !isCompleted[i]) {
                if (burstTime[i] < minBurst) {
                    minBurst = burstTime[i];
                    shortestJobIndex = i;
                }
            }
        }

        // Bước 2: Nếu tìm thấy một tiến trình để thực thi
        if (shortestJobIndex != -1) {
            // Cập nhật thời gian hiện tại
            currentTime += burstTime[shortestJobIndex]; // SJF non-preemptive: chạy hết burst time

            // Gán các giá trị kết quả cho tiến trình này
            completionTime[shortestJobIndex] = currentTime;
            turnaroundTime[shortestJobIndex] = completionTime[shortestJobIndex] - arrivalTime[shortestJobIndex];
            waitingTime[shortestJobIndex] = turnaroundTime[shortestJobIndex] - burstTime[shortestJobIndex];
            isCompleted[shortestJobIndex] = true; // Đánh dấu là đã hoàn thành
            completedProcesses++;                 // Tăng số tiến trình đã hoàn thành

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

    // In kết quả theo thứ tự PID ban đầu (không theo thứ tự thực thi)
    // Để làm điều này, chúng ta cần một cách để ánh xạ lại các chỉ số sau khi sắp xếp
    // Tùy thuộc vào cách bạn muốn hiển thị:
    // 1. Nếu bạn muốn hiển thị theo thứ tự ID ban đầu, cần lưu lại thông tin gốc.
    // 2. Nếu bạn muốn hiển thị theo thứ tự hoàn thành/thực thi, thì thứ tự hiện tại là đúng.
    // Để giữ đơn giản như yêu cầu, chúng ta in theo thứ tự mà các mảng đã được xử lý (không cần sắp xếp lại để in)
    // Tuy nhiên, việc tính toán SJF đã làm thay đổi thứ tự implicit.
    // Để hiển thị đúng cho từng PID ban đầu, ta cần lưu các giá trị của mỗi PID vào một mảng kết quả cuối cùng.
    // Nhưng vì mình giữ nguyên mảng tĩnh và logic đơn giản, việc này sẽ hơi phức tạp.
    // Vì vậy, mình sẽ in kết quả theo thứ tự mà các tiến trình đã hoàn thành.

    // Để in thông tin đúng theo ID ban đầu sau khi thuật toán chạy,
    // ta cần tạo một mảng Process mới và copy dữ liệu vào để sắp xếp theo ID khi in.
    // Nhưng nếu giữ đơn giản với các mảng riêng lẻ, cách đơn giản nhất là in theo thứ tự mà
    // thuật toán đã hoàn thành chúng.
    // Hoặc, phức tạp hơn một chút, tạo một mảng các cặp (PID, kết quả) để in.
    // Để giữ **thật đơn giản** theo yêu cầu của bạn, mình sẽ giả định là bạn muốn in kết quả
    // của các tiến trình đã được xử lý (tức là theo thứ tự mà SJF tìm và chạy).
    // Nếu bạn muốn in theo thứ tự ID ban đầu, cần phải lưu lại dữ liệu ban đầu hoặc sử dụng cấu trúc `Process`.
    // Với các mảng riêng lẻ, việc này trở nên phức tạp.

    // Mình sẽ in dựa trên các mảng `pid`, `arrivalTime`, v.v.
    // Do SJF không sắp xếp các mảng một lần ở đầu như FCFS, nên các mảng ban đầu vẫn giữ nguyên thứ tự nhập.
    // Các giá trị `completionTime`, `turnaroundTime`, `waitingTime` được điền vào đúng vị trí của PID tương ứng.
    for (int i = 0; i < n; i++) {
        cout << left << setw(5) << pid[i] // ID ban đầu
                  << setw(15) << arrivalTime[i]
                  << setw(15) << burstTime[i]
                  << setw(18) << completionTime[i]
                  << setw(18) << turnaroundTime[i]
                  << setw(15) << waitingTime[i] << endl;
        total_avg_wt += waitingTime[i];
        total_avg_tat += turnaroundTime[i];
    }
    cout << "------------------------------------------------------------------------------------------------\n";

    cout << fixed << setprecision(2);
    cout << "Thoi gian cho trung binh: " << total_avg_wt / n << endl;
    cout << "Thoi gian hoan tat trung binh: " << total_avg_tat / n << endl;

    return 0;
}
