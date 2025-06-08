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
#include <vector>   // Sử dụng std::vector thay cho mảng cố định để linh hoạt hơn
#include <numeric>  // Cho std::iota
#include <iomanip>  // Cho std::setw

using namespace std;

// Thay vì MAX cố định, sử dụng vector để linh hoạt với kích thước n, m
int n, m; // n: số tiến trình, m: số loại tài nguyên

// Sử dụng vector<vector<int>> cho ma trận, vector<int> cho mảng
vector<vector<int>> Max;
vector<vector<int>> Allocation;
vector<vector<int>> Need;
vector<int> Available;
vector<bool> Finish; // Finish sẽ được khởi tạo trong isSafe()


// Hàm hiển thị một ma trận
void printMatrix(const string& title, const vector<vector<int>>& matrix) {
    cout << "\n--- " << title << " ---\n";
    cout << "     "; // Khoảng trống cho cột Px
    for (int j = 0; j < m; ++j) {
        cout << setw(4) << "R" << j; // Tên tài nguyên (R0, R1, ...)
    }
    cout << endl;
    cout << "--------------------------------\n"; // Dòng phân cách
    for (int i = 0; i < n; ++i) {
        cout << "P" << i << " | ";
        for (int j = 0; j < m; ++j) {
            cout << setw(4) << matrix[i][j];
        }
        cout << endl;
    }
    cout << "--------------------------------\n";
}

// Hàm hiển thị mảng Available
void printAvailable(const string& title, const vector<int>& arr) {
    cout << "\n--- " << title << " ---\n";
    cout << "     ";
    for (int j = 0; j < m; ++j) {
        cout << setw(4) << "R" << j;
    }
    cout << endl;
    cout << "--------------------------------\n";
    cout << "     | ";
    for (int j = 0; j < m; ++j) {
        cout << setw(4) << arr[j];
    }
    cout << endl;
    cout << "--------------------------------\n";
}


void inputData() {
    cout << "Nhap so tien trinh (n): ";
    cin >> n;
    cout << "Nhap so loai tai nguyen (m): ";
    cin >> m;

    // Khởi tạo kích thước cho các vector
    Max.resize(n, vector<int>(m));
    Allocation.resize(n, vector<int>(m));
    Need.resize(n, vector<int>(m));
    Available.resize(m);

    cout << "\n--- Nhap ma tran Allocation ---\n";
    for (int i = 0; i < n; i++) {
        cout << "Tien trinh P" << i << ": ";
        for (int j = 0; j < m; j++) {
            cin >> Allocation[i][j];
        }
    }

    cout << "\n--- Nhap ma tran Max ---\n";
    for (int i = 0; i < n; i++) {
        cout << "Tien trinh P" << i << ": ";
        for (int j = 0; j < m; j++) {
            cin >> Max[i][j];
        }
    }

    cout << "\n--- Nhap mang Available ---\n";
    cout << "Available (cach nhau boi khoang trang): ";
    for (int i = 0; i < m; i++) {
        cin >> Available[i];
    }

    // Tinh Need
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }

    // Hiển thị dữ liệu đã nhập và tính toán
    printMatrix("Ma tran Allocation", Allocation);
    printMatrix("Ma tran Max", Max);
    printMatrix("Ma tran Need (Max - Allocation)", Need);
    printAvailable("Mang Available", Available);
}

// Hàm kiểm tra trạng thái an toàn
// Sử dụng các bản sao (copy) của Work và Finish để không làm thay đổi trạng thái gốc
bool isSafe() {
    vector<int> work = Available; // Bắt đầu với tài nguyên khả dụng hiện tại
    vector<bool> finish(n, false); // Tất cả tiến trình ban đầu chưa hoàn thành

    vector<int> safeSeq(n); // Lưu trữ chuỗi an toàn
    int count = 0; // Đếm số tiến trình đã hoàn thành

    while (count < n) {
        bool found = false; // Biến cờ để kiểm tra xem có tiến trình nào được tìm thấy trong vòng lặp này không
        for (int i = 0; i < n; i++) {
            if (!finish[i]) { // Nếu tiến trình P_i chưa hoàn thành
                bool canExecute = true; // Giả định có thể thực hiện
                for (int j = 0; j < m; j++) {
                    if (Need[i][j] > work[j]) { // Nếu nhu cầu > tài nguyên đang có
                        canExecute = false; // Không thể thực hiện
                        break;
                    }
                }

                if (canExecute) { // Nếu có thể thực hiện tiến trình P_i
                    for (int j = 0; j < m; j++) {
                        work[j] += Allocation[i][j]; // Giải phóng tài nguyên
                    }
                    safeSeq[count++] = i; // Thêm vào chuỗi an toàn
                    finish[i] = true; // Đánh dấu là đã hoàn thành
                    found = true; // Đã tìm thấy một tiến trình
                }
            }
        }
        if (!found) { // Nếu không tìm thấy tiến trình nào có thể thực hiện trong một vòng lặp
            cout << "He thong KHONG o trang thai an toan.\n";
            return false; // Hệ thống không an toàn
        }
    }

    // Nếu tất cả tiến trình đều hoàn thành
    cout << "He thong o trang thai an toan.\nDay an toan: ";
    for (int i = 0; i < n; i++) {
        cout << "P" << safeSeq[i] << (i == n - 1 ? "" : " -> ");
    }
    cout << endl;
    return true;
}

// Hàm xử lý yêu cầu tài nguyên
bool requestResource(int pid, const vector<int>& request) {
    // 1. Kiểm tra Request_i <= Need_i
    for (int i = 0; i < m; i++) {
        if (request[i] > Need[pid][i]) {
            cout << "Yeu cau tai nguyen vuot qua nhu cau toi da (Need) cua tien trinh P" << pid << ".\n";
            return false;
        }
    }

    // 2. Kiểm tra Request_i <= Available
    for (int i = 0; i < m; i++) {
        if (request[i] > Available[i]) {
            cout << "Khong du tai nguyen hien co trong Available. Tien trinh P" << pid << " phai cho.\n";
            return false;
        }
    }

    // 3. Giả định cấp phát: cập nhật Available, Allocation, Need
    // Tạo bản sao của trạng thái hiện tại để có thể rollback nếu không an toàn
    vector<int> oldAvailable = Available;
    vector<vector<int>> oldAllocation = Allocation;
    vector<vector<int>> oldNeed = Need;

    for (int i = 0; i < m; i++) {
        Available[i] -= request[i];
        Allocation[pid][i] += request[i];
        Need[pid][i] -= request[i];
    }

    // 4. Kiểm tra trạng thái an toàn sau khi giả định cấp phát
    if (isSafe()) {
        cout << "\nYeu cau cua tien trinh P" << pid << " duoc chap nhan.\n";
        // Hiển thị trạng thái mới của hệ thống
        printAvailable("Mang Available moi", Available);
        printMatrix("Ma tran Allocation moi", Allocation);
        printMatrix("Ma tran Need moi", Need);
        return true;
    } else {
        // 5. Nếu không an toàn, rollback trạng thái
        cout << "\nYeu cau cua tien trinh P" << pid << " bi tu choi vi cap phat se lam he thong khong an toan. He thong duoc khoi phuc ve trang thai cu.\n";
        Available = oldAvailable;
        Allocation = oldAllocation;
        Need = oldNeed;
        // Có thể in lại trạng thái cũ nếu muốn
        // printAvailable("Mang Available (khoi phuc)", Available);
        // printMatrix("Ma tran Allocation (khoi phuc)", Allocation);
        // printMatrix("Ma tran Need (khoi phuc)", Need);
        return false;
    }
}

int main() {
    inputData();

    cout << "\n--- Kiem tra trang thai he thong ban dau ---\n";
    isSafe(); // Gọi isSafe() để kiểm tra trạng thái ban đầu

    char choice;
    do {
        int pid;
        vector<int> request(m); // Sử dụng vector để lưu yêu cầu

        cout << "\n--- Yeu cau tai nguyen ---\n";
        cout << "Nhap ma so tien trinh muon yeu cau (0 den " << n - 1 << "): ";
        cin >> pid;

        // Kiểm tra PID hợp lệ
        if (pid < 0 || pid >= n) {
            cout << "Ma so tien trinh khong hop le. Vui long nhap lai.\n";
            continue; // Quay lại vòng lặp do-while
        }

        cout << "Nhap mang tai nguyen tien trinh P" << pid << " yeu cau (cach nhau boi khoang trang):\n";
        for (int i = 0; i < m; i++) {
            cout << "  Tai nguyen R" << i << ": ";
            cin >> request[i];
        }

        requestResource(pid, request);

        cout << "\nBan co muon tiep tuc yeu cau tai nguyen (y/n)? ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    return 0;
}
