#include <iostream>
#include <iomanip> // For std::setw, std::fixed, std::setprecision
#include <algorithm> // For std::min, std::max, std::swap

using namespace std;

int main() {
    int n, quantum;
    cout << "Nhap so tien trinh: ";
    cin >> n;

    // Sử dụng mảng tĩnh. VLA (Variable Length Array) không phải C++ chuẩn.
    int pid[n];             // ID tiến trình
    int arrivalTime[n];     // Thời gian đến
    int burstTime[n];       // Thời gian xử lý ban đầu
    int remainingTime[n];   // Thời gian còn lại chưa xử lý

    // Các mảng kết quả
    int completionTime[n];  // Thời điểm hoàn thành
    int turnaroundTime[n];  // Thời gian lưu lại trong hệ thống
    int waitingTime[n];     // Thời gian chờ đợi

    // Trạng thái của tiến trình (giúp kiểm tra đã hoàn thành hay chưa)
    bool isCompleted[n];

    // Nhập thông tin cho từng tiến trình
    for (int i = 0; i < n; i++) {
        pid[i] = i + 1;
        cout << "Nhap thoi gian den (Arrival Time) cua tien trinh " << pid[i] << ": ";
        cin >> arrivalTime[i];
        cout << "Nhap burst time cua tien trinh " << pid[i] << ": ";
        cin >> burstTime[i];
        remainingTime[i] = burstTime[i]; // Khởi tạo thời gian còn lại
        isCompleted[i] = false;          // Ban đầu chưa hoàn thành
    }

    cout << "Nhap quantum: ";
    cin >> quantum;

    // --- Mô phỏng hàng đợi sẵn sàng (Ready Queue) ---
    // Chúng ta sẽ lưu trữ CHỈ SỐ của tiến trình trong các mảng ban đầu
    // trong hàng đợi để dễ dàng truy cập thông tin của nó.
    int* readyQueue = new int[n]; // Mảng để mô phỏng hàng đợi
    int front = 0; // Chỉ số đầu hàng đợi
    int rear = -1; // Chỉ số cuối hàng đợi

    // Hàm thêm vào hàng đợi (lambda để truy cập các biến cục bộ)
    auto enqueue = [&](int processIndex) {
        if (rear < n - 1) { // Đảm bảo không tràn mảng
            readyQueue[++rear] = processIndex;
        }
    };

    // Hàm lấy ra khỏi hàng đợi
    auto dequeue = [&]() {
        if (front <= rear) {
            return readyQueue[front++];
        }
        return -1; // Hàng đợi rỗng
    };

    // Hàm kiểm tra hàng đợi rỗng
    auto isQueueEmpty = [&]() {
        return front > rear;
    };

    // Mảng để theo dõi xem một tiến trình đã được thêm vào hàng đợi lần đầu tiên hay chưa.
    // Điều này quan trọng để tránh thêm cùng một tiến trình vào hàng đợi nhiều lần khi nó mới đến.
    bool* addedToQueueFirstTime = new bool[n];
    for(int i=0; i<n; ++i) addedToQueueFirstTime[i] = false;

    // --- Thực thi thuật toán Round Robin ---
    int currentTime = 0;
    int completedProcesses = 0;

    // Lặp cho đến khi tất cả các tiến trình hoàn thành
    while (completedProcesses < n) {
        // Bước 1: Đưa các tiến trình đã đến vào hàng đợi sẵn sàng
        // Phải lặp qua tất cả các tiến trình để tìm những tiến trình mới đến
        for (int i = 0; i < n; ++i) {
            if (arrivalTime[i] <= currentTime && !isCompleted[i] && !addedToQueueFirstTime[i]) {
                enqueue(i); // Thêm chỉ số của tiến trình vào hàng đợi
                addedToQueueFirstTime[i] = true;
            }
        }

        int currentProcessIndex = -1;
        if (!isQueueEmpty()) {
            currentProcessIndex = dequeue(); // Lấy tiến trình từ đầu hàng đợi
        }

        // Bước 2: Thực thi tiến trình
        if (currentProcessIndex != -1) {
            // Xác định thời gian thực thi: nhỏ hơn quantum hoặc thời gian còn lại
            int timeToExecute = min(quantum, remainingTime[currentProcessIndex]);

            // Cập nhật thời gian còn lại và thời gian hiện tại
            remainingTime[currentProcessIndex] -= timeToExecute;
            currentTime += timeToExecute;

            // Bước 3: Đưa các tiến trình mới đến TRONG KHI tiến trình hiện tại đang chạy
            // Điều này là quan trọng cho Round Robin preemptive
            for (int i = 0; i < n; ++i) {
                if (arrivalTime[i] <= currentTime && !isCompleted[i] && !addedToQueueFirstTime[i]) {
                    enqueue(i);
                    addedToQueueFirstTime[i] = true;
                }
            }


            // Bước 4: Kiểm tra nếu tiến trình đã hoàn thành
            if (remainingTime[currentProcessIndex] == 0) {
                isCompleted[currentProcessIndex] = true;
                completedProcesses++;

                // Tính toán Completion Time, Turnaround Time, Waiting Time
                completionTime[currentProcessIndex] = currentTime;
                turnaroundTime[currentProcessIndex] = completionTime[currentProcessIndex] - arrivalTime[currentProcessIndex];
                waitingTime[currentProcessIndex] = turnaroundTime[currentProcessIndex] - burstTime[currentProcessIndex];
            } else {
                // Nếu chưa hoàn thành, đưa nó trở lại cuối hàng đợi
                enqueue(currentProcessIndex);
            }
        } else {
            // Nếu hàng đợi rỗng và vẫn còn tiến trình chưa hoàn thành,
            // có nghĩa là chưa có tiến trình nào đến. Tăng thời gian để chờ.
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

    cout << fixed << setprecision(2);
    cout << "Thoi gian cho trung binh: " << total_avg_wt / n << endl;
    cout << "Thoi gian hoan tat trung binh: " << total_avg_tat / n << endl;

    // Giải phóng bộ nhớ động
    delete[] readyQueue;
    delete[] addedToQueueFirstTime;

    return 0;
}
